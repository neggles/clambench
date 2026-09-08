#ifndef CLAMBENCH_BENCH_TIME_H
#define CLAMBENCH_BENCH_TIME_H
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
/* Long-running kernels need elapsed wall time, not core-cycle counts. Keep
   timeval at legacy call sites but use monotonic time; NTP/calendar steps must
   not change measured latency. Calls delimit opaque assembly functions; no
   fence is injected in the measured loops. This is not a per-instruction timer. Empty asm is a compiler memory barrier only. */
static inline int bench_gettimeofday(struct timeval *tv, void *unused)
{
#ifdef __MINGW32__
    /* Preserve the legacy MinGW clock interface; Linux uses monotonic time. */
    return gettimeofday(tv, unused);
#else
    (void)unused;
    __asm__ volatile("" ::: "memory");
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts)) { perror("clock_gettime"); exit(1); }
    tv->tv_sec = ts.tv_sec;
    tv->tv_usec = ts.tv_nsec / 1000;
    __asm__ volatile("" ::: "memory");
    return 0;
#endif
}
#endif
