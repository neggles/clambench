#ifndef CLAMBENCH_BENCH_TIME_H
#define CLAMBENCH_BENCH_TIME_H
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#include <intrin.h>
#endif
#endif

/* Elapsed time, not core cycles. RAW avoids clock-rate adjustments by NTP.
   These are compiler memory barriers, not CPU fences or store drains. Exact
   instruction ordering remains the responsibility of the assembly kernels. */
static inline void bench_now(struct timespec *ts)
{
#ifdef _MSC_VER
    _ReadWriteBarrier();
#else
    __asm__ volatile("" ::: "memory");
#endif
#ifdef _WIN32
    /* Windows has no CLOCK_MONOTONIC_RAW; retain monotonic timing via QPC. */
    LARGE_INTEGER counter, frequency;
    if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&counter)) {
        fputs("QueryPerformanceCounter failed\n", stderr); exit(1);
    }
    ts->tv_sec = counter.QuadPart / frequency.QuadPart;
    ts->tv_nsec = (long)((counter.QuadPart % frequency.QuadPart) * 1000000000LL / frequency.QuadPart);
#else
    if (clock_gettime(CLOCK_MONOTONIC_RAW, ts)) {
        perror("clock_gettime(CLOCK_MONOTONIC_RAW)"); exit(1);
    }
#endif
#ifdef _MSC_VER
    _ReadWriteBarrier();
#else
    __asm__ volatile("" ::: "memory");
#endif
}

/* Subtract before converting to floating point, including a nanosecond borrow
   across a second boundary. Requires end >= start from the same clock. */
static inline uint64_t bench_elapsed_ns(const struct timespec *start,
                                       const struct timespec *end)
{
    time_t seconds = end->tv_sec - start->tv_sec;
    long nanos = end->tv_nsec - start->tv_nsec;
    if (nanos < 0) { --seconds; nanos += 1000000000L; }
    return (uint64_t)seconds * UINT64_C(1000000000) + (uint64_t)nanos;
}
#endif
