#ifndef CLAMBENCH_PLATFORM_H
#define CLAMBENCH_PLATFORM_H
#include <stdint.h>
#include <unistd.h>
#if defined(__powerpc64__)
#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__ || !defined(_CALL_ELF) || _CALL_ELF != 2
#error "POWER benchmarks require little-endian ELFv2"
#endif
#include <sys/auxv.h>
#include <asm/cputable.h>
#include <stdio.h>
#include <stdlib.h>
#define CACHELINE_SIZE 128
static inline void bench_require_power9(void)
{
    if (!(getauxval(AT_HWCAP2) & PPC_FEATURE2_ARCH_3_00) ||
        !(getauxval(AT_HWCAP) & PPC_FEATURE_HAS_VSX)) {
        fprintf(stderr, "This build requires POWER9/ISA 3.0 with VSX enabled\n");
        exit(1);
    }
}
#else
#define CACHELINE_SIZE 64
static inline void bench_require_power9(void) {}
#endif
#if defined(__x86_64__)
#define BENCH_ABI __attribute__((ms_abi))
#else
#define BENCH_ABI
#endif
/* Page size is a kernel property (POWER Linux commonly uses 64K pages). */
#ifdef __MINGW32__
#define BENCH_PAGE_SIZE ((size_t)4096)
#else
#define BENCH_PAGE_SIZE ((size_t)sysconf(_SC_PAGESIZE))
#endif
#endif
