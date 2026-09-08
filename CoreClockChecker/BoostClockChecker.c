#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../Common/bench_time.h"
#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#include "../Common/platform.h"
extern uint64_t clktsctest(uint64_t iterations) BENCH_ABI;

int main(int argc, char *argv[]) {
    bench_require_power9();
    struct timeval startTv, endTv;
    uint64_t iterations = 500000, samples = 100;
    unsigned int sleepSeconds = 5;
    time_t time_diff_ms;

    for (int argIdx = 1; argIdx < argc; argIdx++) {
        if (*(argv[argIdx]) == '-') {
            char *arg = argv[argIdx] + 1;
	    if (strncmp(arg, "samples", 7) == 0) {
	        argIdx++;
		samples = atol(argv[argIdx]);
	    } else if (strncmp(arg, "iterations", 10) == 0) {
	        argIdx++;
		iterations = atol(argv[argIdx]);
            } else if (strncmp(arg, "sleep", 5) == 0) {
	        argIdx++;
		sleepSeconds = atoi(argv[argIdx]);
	    }
	}
    }

    if (!samples || samples > SIZE_MAX / sizeof(uint64_t) || !iterations || iterations % 20) {
        fprintf(stderr, "samples must be positive; iterations must be a positive multiple of 20\n");
        return 1;
    }
    sleep(sleepSeconds);

    uint64_t *measuredTscs = malloc(samples * sizeof(uint64_t));
    if (!measuredTscs) return 1;
    for (uint64_t sampleIdx = 0; sampleIdx < samples; sampleIdx++) {
        uint64_t elapsedTsc = clktsctest(iterations);
	measuredTscs[sampleIdx] = elapsedTsc;
    }

    fprintf(stderr, "Used %lu samples\n", samples);
    fprintf(stderr, "Used %lu iterations\n", iterations);
    // figure out TSC to real time ratio
    fprintf(stderr, "Checking TSC ratio...\n");
    uint64_t iterationsHi = 8e9; // should be a couple seconds at least?
    bench_gettimeofday(&startTv, NULL);
    uint64_t referenceElapsedTsc = clktsctest(iterationsHi);
    bench_gettimeofday(&endTv, NULL);
    time_diff_ms = 1000 * (endTv.tv_sec - startTv.tv_sec) + ((endTv.tv_usec - startTv.tv_usec) / 1000);
    float tsc_per_ms = (float)referenceElapsedTsc / (float)time_diff_ms;
    float tsc_per_ns = tsc_per_ms / 1e6;
    fprintf(stderr, "TSC = %lu, elapsed ms = %lu\n", referenceElapsedTsc, time_diff_ms);
    fprintf(stderr, "TSC per ms: %f, TSC per ns: %f\n", tsc_per_ms, tsc_per_ns);

#ifdef __powerpc64__
    /* A one-cycle dependent add is a hardware assumption, not a TB property. */
    fprintf(stderr, "POWER timebase calibrated against monotonic time; GHz assumes one cycle/add\n");
    printf("Time (ms), Estimated clk (GHz), Timebase ticks\n");
#else
    printf("Time (ms), Clk (GHz), TSC\n");
#endif
    float elapsedTime = 0;
    for (uint64_t sampleIdx = 0; sampleIdx < samples; sampleIdx++) {
	// (tsc / ms) * tsc = 1 / ms
	float elapsedTimeMs = measuredTscs[sampleIdx] / tsc_per_ms;
	elapsedTime += elapsedTimeMs;
	float latency = 1e6 * elapsedTimeMs / (float)iterations;
	float addsPerNs = 1 / latency;
	printf("%f,%f,%lu\n", elapsedTime, addsPerNs, measuredTscs[sampleIdx]);
    }

    free(measuredTscs);
    return 0;
}
