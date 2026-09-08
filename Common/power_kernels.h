#ifndef CLAMBENCH_POWER_KERNELS_H
#define CLAMBENCH_POWER_KERNELS_H
#include <stdint.h>
/* count is instructions; data points to >=128 writable bytes. FP64 requires
   data[0] as double = 1.0. Calls do not return a performance measurement. */
uint64_t add_test(uint64_t count, void *data);
uint64_t add_latency(uint64_t count, void *data);
uint64_t mul_test(uint64_t count, void *data);
uint64_t mul_latency(uint64_t count, void *data);
uint64_t xor_test(uint64_t count, void *data);
uint64_t rotate_test(uint64_t count, void *data);
uint64_t nop_test(uint64_t count, void *data);
uint64_t branch_test(uint64_t count, void *data);
uint64_t branch_not_taken_test(uint64_t count, void *data);
uint64_t vec_int32_add_test(uint64_t count, void *data);
uint64_t vec_int32_add_latency(uint64_t count, void *data);
uint64_t vec_int32_mul_test(uint64_t count, void *data);
uint64_t vec_int32_mul_latency(uint64_t count, void *data);
uint64_t vec_fp32_add_test(uint64_t count, void *data);
uint64_t vec_fp32_add_latency(uint64_t count, void *data);
uint64_t vec_fp32_mul_test(uint64_t count, void *data);
uint64_t vec_fp32_mul_latency(uint64_t count, void *data);
uint64_t vec_fp32_fma_test(uint64_t count, void *data);
uint64_t vec_fp32_fma_latency(uint64_t count, void *data);
uint64_t vec_fp32_isqrt_test(uint64_t count, void *data);
uint64_t fp64_add_test(uint64_t count, void *data);
uint64_t fp64_add_latency(uint64_t count, void *data);
uint64_t fp64_mul_test(uint64_t count, void *data);
uint64_t fp64_mul_latency(uint64_t count, void *data);
uint64_t fp64_fma_test(uint64_t count, void *data);
uint64_t fp64_fma_latency(uint64_t count, void *data);
uint64_t load_test(uint64_t count, void *data);
uint64_t vector_load_test(uint64_t count, void *data);
uint64_t store_test(uint64_t count, void *data);
uint64_t vector_store_test(uint64_t count, void *data);
#endif
