#include <stdio.h>
#include <time.h>
#include "../Common/bench_time.h"
#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#include "../Common/platform.h"
extern uint64_t clktsctest(uint64_t iterations) BENCH_ABI;

int main(int argc, char *argv[]) {
    bench_require_power9();
    struct timespec startTv, endTv;
    uint64_t iterations = 500000, samples = 100;
    unsigned int sleepSeconds = 5;
    uint64_t elapsed_ns;

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
    bench_now(&startTv);
    uint64_t referenceElapsedTsc = clktsctest(iterationsHi);
    bench_now(&endTv);
    elapsed_ns = bench_elapsed_ns(&startTv, &endTv);
    if (!elapsed_ns || !referenceElapsedTsc) {
        fprintf(stderr, "Reference sample has zero elapsed time or counter ticks\n");
        return 1;
    }
    double tsc_per_ns = (double)referenceElapsedTsc / elapsed_ns;
    fprintf(stderr, "TSC = %lu, elapsed ns = %lu\n", referenceElapsedTsc, elapsed_ns);
    fprintf(stderr, "TSC per ns: %f\n", tsc_per_ns);

#ifdef __powerpc64__
    /* A one-cycle dependent add is a hardware assumption, not a TB property. */
    fprintf(stderr, "POWER timebase calibrated against CLOCK_MONOTONIC_RAW; GHz assumes one cycle/add\n");
    printf("Time (ms), Estimated clk (GHz), Timebase ticks\n");
#else
    printf("Time (ms), Clk (GHz), TSC\n");
#endif
    double elapsedTime = 0;
    for (uint64_t sampleIdx = 0; sampleIdx < samples; sampleIdx++) {
	double elapsedTimeNs = measuredTscs[sampleIdx] / tsc_per_ns;
	double elapsedTimeMs = elapsedTimeNs / 1e6;
	elapsedTime += elapsedTimeMs;
	double latency = elapsedTimeNs / (double)iterations;
	double addsPerNs = 1 / latency;
	printf("%f,%f,%lu\n", elapsedTime, addsPerNs, measuredTscs[sampleIdx]);
    }

    free(measuredTscs);
    return 0;
}
