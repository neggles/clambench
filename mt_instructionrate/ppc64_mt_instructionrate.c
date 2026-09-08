#include "../Common/power_kernels.h"

void RunTests(void) {
    /* measureFunction counts instructions; vector results multiply by lanes.
       FMA is one instruction/two FLOPs per lane, rsqrt is an estimate. */
    double data[16] __attribute__((aligned(128)));
    struct test { const char *name; uint64_t (*fn)(uint64_t, void *); int lanes; } tests[] = {
        {"INT32 Add", vec_int32_add_test, 4},
        {"INT32 Multiply", vec_int32_mul_test, 4},
        {"FP32 Add", vec_fp32_add_test, 4},
        {"FP32 FMA (2 FLOPs/op)", vec_fp32_fma_test, 4},
        {"FP32 Reciprocal sqrt estimate", vec_fp32_isqrt_test, 4},
        {"FP64 Add", fp64_add_test, 1},
        {"FP64 FMA (2 FLOPs/op)", fp64_fma_test, 1},
    };
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        for (int j = 0; j < 16; j++) data[j] = 1.0;
        float rate = measureFunction(32000000, tests[i].fn, data);
        printf("%s: %f GOPS/s\n", tests[i].name, tests[i].lanes * rate);
    }
}
