#ifndef timingincluded
#define timingincluded
#include <stdint.h>
/* Legacy millisecond API retained for the out-of-scope GPU/SVM callers. CPU
   benchmarks use bench_time.h and nanoseconds throughout their calculations. */
void start_timing(void);
unsigned int end_timing(void);
unsigned long long scale_iterations_to_target(unsigned long long count, float elapsed_ms, float target_ms);
uint64_t scale_iterations_to_target_ns(uint64_t count, uint64_t elapsed_ns, uint64_t target_ns);
#endif
