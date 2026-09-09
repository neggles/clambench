#define _GNU_SOURCE
#undef NDEBUG
#include <assert.h>
#include "../Common/bench_time.h"
#include "../Common/timing.h"

int main(void) {
    /* A large absolute timestamp must not swallow a sub-microsecond delta. */
    struct timespec start = { .tv_sec = 2000000000, .tv_nsec = 123456789 };
    struct timespec end = { .tv_sec = 2000000000, .tv_nsec = 123456790 };
    assert(bench_elapsed_ns(&start, &end) == 1);
    end = start;
    assert(bench_elapsed_ns(&start, &end) == 0);
    start.tv_nsec = 999999950;
    end.tv_sec++;
    end.tv_nsec = 25;
    assert(bench_elapsed_ns(&start, &end) == 75);
    end.tv_sec += 5;
    assert(bench_elapsed_ns(&start, &end) == UINT64_C(5000000075));

    assert(scale_iterations_to_target_ns(1000, 0, 3000000000) == 2000);
    assert(scale_iterations_to_target_ns(1000, 49999999, 3000000000) == 2000);
    assert(scale_iterations_to_target_ns(1000, 50000000, 3000000000) == 60000);
    assert(scale_iterations_to_target_ns(1000, 50123456, 3000000000)
           == UINT64_C(3000000000000) / 50123456);
    assert(scale_iterations_to_target_ns(UINT64_MAX, 1, 3000000000) == UINT64_MAX);

    struct timespec before, sample, after;
    assert(!clock_gettime(CLOCK_MONOTONIC_RAW, &before));
    bench_now(&sample);
    assert(!clock_gettime(CLOCK_MONOTONIC_RAW, &after));
    assert(sample.tv_sec > before.tv_sec ||
           (sample.tv_sec == before.tv_sec && sample.tv_nsec >= before.tv_nsec));
    assert(after.tv_sec > sample.tv_sec ||
           (after.tv_sec == sample.tv_sec && after.tv_nsec >= sample.tv_nsec));
    assert(sample.tv_nsec >= 0 && sample.tv_nsec < 1000000000L);
    puts("raw timespec timing: PASS");
}
