#include "timing.h"
#include "bench_time.h"

static struct timespec legacy_start;
void start_timing(void) { bench_now(&legacy_start); }
unsigned int end_timing(void) {
    struct timespec end;
    bench_now(&end);
    return (unsigned int)(bench_elapsed_ns(&legacy_start, &end) / 1000000);
}

unsigned long long scale_iterations_to_target(unsigned long long count, float elapsed_ms, float target_ms) {
    if (elapsed_ms < 50) return count * 2;
    return count * (target_ms / elapsed_ms);
}

uint64_t scale_iterations_to_target_ns(uint64_t count, uint64_t elapsed_ns, uint64_t target_ns) {
    /* Preserve the old 50 ms calibration threshold without rounding samples. */
    long double scaled = elapsed_ns < UINT64_C(50000000)
        ? (long double)count * 2
        : (long double)count * target_ns / elapsed_ns;
    if (scaled >= UINT64_MAX) return UINT64_MAX;
    return scaled < 1 ? 1 : (uint64_t)scaled;
}
