#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Common/platform.h"
#include <string.h>
#include <sched.h>
#include "../Common/bench_time.h"
#include "../Common/power_kernels.h"

/* No dependency-chain frequency guess: timebase/wall clock are NOT core
   cycles. Report instruction throughput and dependency latency in ns. */
int main(int argc, char **argv)
{
    bench_require_power9();
    uint64_t iterations = 32000000;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-iterations") && i + 1 < argc) {
            char *end;
            iterations = strtoull(argv[++i], &end, 10);
            if (*end || !iterations || argv[i][0] == '-') return 1;
        } else if (!strcmp(argv[i], "-affinity") && i + 1 < argc) {
            int cpu = atoi(argv[++i]);
            if (cpu < 0 || cpu >= CPU_SETSIZE) return 1;
            cpu_set_t cpus;
            CPU_ZERO(&cpus); CPU_SET(cpu, &cpus);
            if (sched_setaffinity(0, sizeof(cpus), &cpus)) { perror("affinity"); return 1; }
        } else {
            fprintf(stderr, "Usage: %s [-iterations N] [-affinity CPU]\n", argv[0]);
            return 1;
        }
    }
    struct test { const char *name; uint64_t (*fn)(uint64_t, void *); } tests[] = {
        {"add_test", add_test},
        {"add_latency", add_latency},
        {"mul_test", mul_test},
        {"mul_latency", mul_latency},
        {"xor_test", xor_test},
        {"rotate_test", rotate_test},
        {"nop_test", nop_test},
        {"branch_test", branch_test},
        {"branch_not_taken_test", branch_not_taken_test},
        {"vec_int32_add_test", vec_int32_add_test},
        {"vec_int32_add_latency", vec_int32_add_latency},
        {"vec_int32_mul_test", vec_int32_mul_test},
        {"vec_int32_mul_latency", vec_int32_mul_latency},
        {"vec_fp32_add_test", vec_fp32_add_test},
        {"vec_fp32_add_latency", vec_fp32_add_latency},
        {"vec_fp32_mul_test", vec_fp32_mul_test},
        {"vec_fp32_mul_latency", vec_fp32_mul_latency},
        {"vec_fp32_fma_test", vec_fp32_fma_test},
        {"vec_fp32_fma_latency", vec_fp32_fma_latency},
        {"vec_fp32_isqrt_test", vec_fp32_isqrt_test},
        {"fp64_add_test", fp64_add_test},
        {"fp64_add_latency", fp64_add_latency},
        {"fp64_mul_test", fp64_mul_test},
        {"fp64_mul_latency", fp64_mul_latency},
        {"fp64_fma_test", fp64_fma_test},
        {"fp64_fma_latency", fp64_fma_latency},
        {"load_test", load_test},
        {"vector_load_test", vector_load_test},
        {"store_test", store_test},
        {"vector_store_test", vector_store_test},
    };
    double data[16] __attribute__((aligned(128)));
    printf("Test,Instructions,Ginstructions/s,ns/instruction\n");
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        for (int j = 0; j < 16; j++) data[j] = 1.0;
        struct timespec begin, end;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        tests[i].fn(iterations, data);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double ns = (end.tv_sec - begin.tv_sec) * 1e9 + end.tv_nsec - begin.tv_nsec;
        if (ns <= 0) {
            fprintf(stderr, "Sample below timer resolution; increase -iterations\n");
            return 1;
        }
        printf("%s,%lu,%.6f,%.6f\n", tests[i].name, iterations, iterations/ns, ns/iterations);
    }
    return 0;
}
