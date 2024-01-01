// MemoryBandwidth.c : Version for linux (x86 and ARM)
// Mostly the same as the x86-only VS version, but a bit more manual

#pragma GCC diagnostic ignored "-Wattributes"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


#define HUGEPAGE_HACK 1
#undef HUGEPAGE_HACK

#if defined(__x86_64) || defined(__i386)
#define IS_X86
#endif

/* GCC is awesome. */
#define ARRAY_SIZE(arr)                                                                                                \
    (sizeof(arr) / sizeof((arr)[0]) +                                                                                  \
     sizeof(typeof(int[1 - 2 * !!__builtin_types_compatible_p(typeof(arr), typeof(&arr[0]))])) * 0)

// cast macros because they're ugly inline
#define CAST_NOPFUNC(x)  __attribute((fastcall)) void (*x)(size_t)
#define CAST_NOPARRAY(x) ((__attribute((fastcall)) void (*)(size_t))x)

// just a little MIN/MAX macro
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// clang-format off
#define MB(x) ((x) * 1024)
uint32_t default_test_sizes[] = {
    2,          4,          8,          12,         16,         24,         32,         48,
    64,         96,         128,        192,        256,        512,        600,        768,
    MB(1),      1536,       MB(2),      MB(3),      MB(4),      MB(5),      MB(6),      MB(8),
    MB(10),     MB(12),     MB(16),     MB(24),     MB(32),     MB(64),     MB(96),     MB(128),
    MB(256),    MB(384),    MB(512),    MB(1024),   MB(1536),   MB(2048),   MB(3072),   MB(4096),
};
// clang-format on

typedef struct BandwidthTestThreadData {
    uint64_t iterations;
    size_t   arr_length;
    size_t   start;
    float   *arr;
    float    bw;  // written to by the thread
#ifdef NUMA
    cpu_set_t cpuset;  // if numa set, will set affinity
#endif
} BandwidthTestThreadData;


__attribute((fastcall)) float (*bw_func)(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);
__attribute((fastcall)) float scalar_read(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);

__attribute__((fastcall)) extern float asm_read(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);
// __attribute__((fastcall)) extern float asm_write(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);
// __attribute__((fastcall)) extern float asm_copy(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);
// __attribute__((fastcall)) extern float asm_cflip(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);
// __attribute__((fastcall)) extern float asm_add(float *arr, size_t arr_length, uint64_t iterations, uint64_t start);

__attribute((fastcall)) float instr_read(float *arr, size_t arr_length, uint64_t iterations, uint64_t start) {
    CAST_NOPFUNC(nopfunc) = CAST_NOPARRAY(arr);
    for (int iterIdx = 0; iterIdx < iterations; iterIdx++)
        nopfunc(iterations);
    return 1.1f;
}

float    MeasureBw(size_t sizeKb, uint64_t iterations, uint64_t threads, uint32_t shared, uint32_t nopBytes,
                   uint32_t coreNode, uint32_t memNode);
uint64_t GetIterationCount(uint32_t testSize, uint32_t threads);
uint64_t GetMicrosecondTime(void);
void     FillInstructionArray(uintptr_t *nops, uint64_t sizeKb, uint32_t nopSize, uint32_t branchInterval);
void    *ReadBandwidthTestThread(void *param);

void *allocate_memory(size_t bytes, uint32_t threadOffset);

cpu_set_t global_cpuset;
uint32_t  gbToTransfer   = 128;
uint32_t  branchInterval = 0;
uint32_t  hardaffinity   = 0;


int main(int argc, char *argv[]) {
    uint32_t threads = 1, shared = 1, sleepTime = 0, singleSize = 0, autothreads = 0;
    uint64_t nopBytes      = 0;
    uint64_t testSizeCount = ARRAY_SIZE(default_test_sizes);

    bw_func = asm_read;

    for (int argIdx = 1; argIdx < argc; argIdx++) {
        if (*(argv[argIdx]) == '-') {
            char *arg = argv[argIdx] + 1;
            if (strncmp(arg, "threads", 7) == 0) {
                argIdx++;
                threads = atoi(argv[argIdx]);
                fprintf(stderr, "Using %d threads\n", threads);
            } else if (strncmp(arg, "shared", 6) == 0) {
                shared = 1;
                fprintf(stderr, "Using shared array\n");
            } else if (strncmp(arg, "hardaffinity", 12) == 0) {
                hardaffinity = 1;
                CPU_ZERO(&global_cpuset);
                CPU_SET(0, &global_cpuset);
                CPU_SET(1, &global_cpuset);
                sched_setaffinity(gettid(), sizeof(cpu_set_t), &global_cpuset);
                fprintf(stderr, "hardaffinity 0,1\n");
            } else if (strncmp(arg, "sleep", 5) == 0) {
                argIdx++;
                sleepTime = atoi(argv[argIdx]);
                fprintf(stderr, "Sleeping for %d second between tests\n", sleepTime);
            } else if (strncmp(arg, "private", 7) == 0) {
                shared = 0;
                fprintf(stderr, "Using private array for each thread\n");
            } else if (strncmp(arg, "branchinterval", 14) == 0) {
                argIdx++;
                branchInterval = atoi(argv[argIdx]);
                fprintf(stderr, "Will add a branch roughly every %d bytes\n", branchInterval * 8);
            } else if (strncmp(arg, "sizekb", 6) == 0) {
                argIdx++;
                singleSize = atoi(argv[argIdx]);
                fprintf(stderr, "Testing %d KB\n", singleSize);
            } else if (strncmp(arg, "data", 4) == 0) {
                argIdx++;
                gbToTransfer = atoi(argv[argIdx]);
                fprintf(stderr, "Base GB to transfer: %u\n", gbToTransfer);
            } else if (strncmp(arg, "autothreads", 11) == 0) {
                argIdx++;
                autothreads = atoi(argv[argIdx]);
                fprintf(stderr, "Testing bw scaling up to %d threads\n", autothreads);
            } else if (strncmp(arg, "method", 6) == 0) {
                argIdx++;
                if (strncmp(argv[argIdx], "scalar", 6) == 0) {
                    bw_func = scalar_read;
                    fprintf(stderr, "Using scalar C code\n");
                } else if (strncmp(argv[argIdx], "asm", 3) == 0) {
                    bw_func = asm_read;
                    fprintf(stderr, "Using ASM code (AVX or NEON)\n");
                } else if (strncmp(argv[argIdx], "instr8", 6) == 0) {
                    nopBytes = 8;
                    bw_func  = instr_read;
                    fprintf(stderr, "Testing instruction fetch bandwidth with 8 byte instructions.\n");
                } else if (strncmp(argv[argIdx], "instr4", 6) == 0) {
                    nopBytes = 4;
                    bw_func  = instr_read;
                    fprintf(stderr, "Testing instruction fetch bandwidth with 4 byte instructions.\n");
                } else if (strncmp(argv[argIdx], "instr2", 6) == 0) {
                    nopBytes = 2;
                    bw_func  = instr_read;
                    fprintf(stderr, "Testing instruction fetch bandwith with 2 byte instructions.\n");
                }
            }
        } else {
            fprintf(stderr, "Expected - parameter\n");
            fprintf(
                stderr,
                "Usage: [-threads <thread count>] [-private] [-method <scalar/asm/avx512>] [-sleep <time in seconds>] [-sizekb <single test size>]\n");
        }
    }

    if (autothreads > 0) {
        float *threadResults = (float *)malloc(sizeof(float) * autothreads * testSizeCount);
        printf("Auto threads mode, up to %d threads\n", autothreads);
        for (int threadIdx = 1; threadIdx <= autothreads; threadIdx++) {
            if (singleSize != 0) {
                threadResults[threadIdx - 1] =
                    MeasureBw(singleSize, GetIterationCount(singleSize, threadIdx), threadIdx, shared, nopBytes, 0, 0);
                fprintf(stderr, "%d threads: %f GB/s\n", threadIdx, threadResults[threadIdx - 1]);
            } else {
                for (int i = 0; i < testSizeCount; i++) {
                    int currentTestSize = default_test_sizes[i];
                    //fprintf(stderr, "Testing size %d\n", currentTestSize);
                    threadResults[(threadIdx - 1) * testSizeCount + i] =
                        MeasureBw(currentTestSize, GetIterationCount(currentTestSize, threadIdx), threadIdx, shared,
                                  nopBytes, 0, 0);
                    fprintf(stderr, "%d threads, %d KB total: %f GB/s\n", threadIdx, currentTestSize,
                            threadResults[(threadIdx - 1) * testSizeCount + i]);
                }
            }
        }

        if (singleSize != 0) {
            printf("Threads, BW (GB/s)\n");
            for (int i = 0; i < autothreads; i++) {
                printf("%d,%f\n", i + 1, threadResults[i]);
            }
        } else {
            printf("Test size down, threads across, value = GB/s\n");
            for (int sizeIdx = 0; sizeIdx < testSizeCount; sizeIdx++) {
                printf("%d", default_test_sizes[sizeIdx]);
                for (int threadIdx = 1; threadIdx <= autothreads; threadIdx++) {
                    printf(",%f", threadResults[(threadIdx - 1) * testSizeCount + sizeIdx]);
                }

                printf("\n");
            }
        }

        free(threadResults);
    } else {
        printf("Using %d threads\n", threads);
        if (singleSize == 0) {
            for (int i = 0; i < testSizeCount; i++) {
                printf("%d,%f\n", default_test_sizes[i],
                       MeasureBw(default_test_sizes[i], GetIterationCount(default_test_sizes[i], threads), threads,
                                 shared, nopBytes, 0, 0));
                if (sleepTime > 0) sleep(sleepTime);
            }
        } else {
            printf("%d,%f\n", singleSize,
                   MeasureBw(singleSize, GetIterationCount(singleSize, threads), threads, shared, nopBytes, 0, 0));
        }
    }

    return 0;
}

/// <summary>
/// Given test size in KB, return a good iteration count
/// </summary>
/// <param name="testSize">test size in KB</param>
/// <returns>Iterations per thread</returns>
uint64_t GetIterationCount(uint32_t testSize, uint32_t threads) {
    uint32_t scaledGbToTransfer = gbToTransfer;
    uint64_t iterations         = 0;

    if (testSize > 64) scaledGbToTransfer = gbToTransfer / 8;
    iterations = scaledGbToTransfer * 1024 * 1024 / testSize;

    if (iterations % 2 != 0) iterations += 1;  // must be even
    if (iterations < 8) return 8;  // set a minimum to reduce noise
    else return iterations;
}

void FillInstructionArray(size_t *nops, uint64_t sizeKb, uint32_t nopSize, uint32_t branchInterval) {
    int    err      = 0;
    size_t elements = 0, funcLen = 0;

    char nop2b[8]             = { 0x66, 0x90, 0x66, 0x90, 0x66, 0x90, 0x66, 0x90 };
    char nop3b[8]             = { 0x0F, 0x1F, 0x00, 0x00, 0x0F, 0x1F, 0x00, 0x00 };
    char nop8b[8]             = { 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00 };
    char nop4b[8]             = { 0x0F, 0x1F, 0x44, 0x00, 0x0F, 0x1F, 0x44, 0x00 };
    char nop4b_with_branch[8] = { 0x0F, 0x1F, 0x40, 0x00, 0xEB, 0x00, 0x66, 0x90 };

    uintptr_t *nop8bptr = NULL;
    if (nopSize == 8) nop8bptr = (uintptr_t *)(nop8b);
    else if (nopSize == 4) nop8bptr = (uintptr_t *)(nop4b);
    else if (nopSize == 2) nop8bptr = (uintptr_t *)(nop2b);
    else if (nopSize == 3) nop8bptr = (uintptr_t *)(nop3b);
    else {
        fprintf(stderr, "%d byte instruction length isn't supported :(\n", nopSize);
    }

    elements = sizeKb * 1024 / 8 - 1;
    for (size_t nopIdx = 0; nopIdx < elements; nopIdx++) {
        nops[nopIdx]           = *nop8bptr;
        uint64_t *nopBranchPtr = (uint64_t *)nop4b_with_branch;
        if (branchInterval > 1 && nopIdx % branchInterval == 0) nops[nopIdx] = *nopBranchPtr;
    }

    // ret
    unsigned char *functionEnd = (unsigned char *)(nops + elements);

    functionEnd[0]     = 0xC3;
    funcLen            = sizeKb * 1024;
    size_t nopfuncPage = (~0xFFF) & (size_t)(nops);
    size_t mprotectLen = (0xFFF & (size_t)(nops)) + funcLen;

    err = mprotect((void *)nopfuncPage, mprotectLen, PROT_EXEC | PROT_READ | PROT_WRITE);
    if (err < 0) {
        fprintf(stderr, "mprotect failed, errno %d\n", errno);
    }
}

// If coreNode and memNode are set, use the specified numa config
// otherwise if numa is set to stripe or seq, respect that
float MeasureBw(size_t sizeKb, uint64_t iterations, uint64_t threads, uint32_t shared, uint32_t nopBytes,
                uint32_t coreNode, uint32_t memNode) {
    struct timespec start_ts = { 0 }, end_ts = { 0 };
    double          gbytes_transferred = 0, time_diff_ms = 0;
    uint32_t        private_elements = 0, elements = 0;
    float          *testArr = NULL;
    float           bw      = 0;

    // thread data/handles
    pthread_t               *testThreads;
    BandwidthTestThreadData *threadData;

    if (!shared && sizeKb < threads) {
        fprintf(stderr, "Too many threads for this test size\n");
        return 0;
    }
    //fprintf(stderr, "Iterations: %llu\n", iterations);

    // make sure this is divisble by 512 bytes, since the unrolled asm loop depends on that
    // it's hard enough to get close to theoretical L1D BW as is, so we don't want additional cmovs or branches
    // in the hot loop
    elements         = sizeKb * 1024 / sizeof(float);
    private_elements = ceil((double)sizeKb / (double)threads) * 256;
    //fprintf(stderr, "Actual data: %lu B\n", private_elements * 4 * threads);
    //fprintf(stderr, "Data per thread: %lu B\n", private_elements * 4);

    // make array and fill it with something, if shared
    if (shared) {
        //testArr = (float*)aligned_alloc(64, elements * sizeof(float));
        testArr = allocate_memory(elements * sizeof(float), 0);
        if (testArr == NULL) {
            fprintf(stderr, "Could not allocate memory\n");
            return 0;
        }

        if (nopBytes == 0) {
            for (size_t i = 0; i < elements; i++) {
                testArr[i] = i + 0.5f;
            }
        } else FillInstructionArray((size_t *)testArr, sizeKb, nopBytes, branchInterval);
    } else {
        elements = private_elements;  // will fill arrays below, per-thread
    }

    testThreads = calloc(threads, sizeof(pthread_t));
    threadData  = calloc(threads, sizeof(struct BandwidthTestThreadData));

    for (size_t i = 0; i < threads; i++) {
        if (shared) {
            threadData[i].arr        = testArr;
            threadData[i].iterations = iterations;
        } else {
            // Not NUMA aware. Allocate memory normally
            //threadData[i].arr = (float*)aligned_alloc(64, elements * sizeof(float));
            threadData[i].arr = allocate_memory(elements * sizeof(float), i);
            if (threadData[i].arr == NULL) {
                fprintf(stderr, "Could not allocate memory for thread %zd\n", i);
                return 0;
            }

            if (nopBytes == 0) {
                for (size_t arr_idx = 0; arr_idx < elements; arr_idx++) {
                    threadData[i].arr[arr_idx] = arr_idx + i + 0.5f;
                }
            } else
                FillInstructionArray((size_t *)threadData[i].arr, elements * sizeof(float) / 1024, nopBytes,
                                     branchInterval);

            threadData[i].iterations = iterations * threads;
        }

        threadData[i].arr_length = elements;
        threadData[i].bw         = 0;
        threadData[i].start      = 0;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &start_ts);
    for (size_t i = 0; i < threads; i++)
        pthread_create(&testThreads[i], NULL, ReadBandwidthTestThread, (void *)(&threadData[i]));
    for (size_t i = 0; i < threads; i++)
        pthread_join(testThreads[i], NULL);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_ts);

    time_diff_ms = ((end_ts.tv_sec - start_ts.tv_sec) * 1000) + ((end_ts.tv_nsec - start_ts.tv_nsec) / 1000000.0F);
    gbytes_transferred = (iterations * sizeof(float) * elements * threads) / (double)1024 / (double)1024 / (double)1024;
    bw                 = 1000 * gbytes_transferred / (double)time_diff_ms;

    if (!shared) bw *= threads;  // iteration count is divided by thread count if in thread private mode
#ifdef DBG_PRINT
    printf("%4.2f GB, %.2f ms, %.2f GB/sec\n", gbytes_transferred, time_diff_ms, bw);
#endif

    free(testThreads);
    free(testArr);  // should be null in not-shared (private) mode

    if (!shared) {
        for (size_t i = 0; i < threads; i++) {
            free(threadData[i].arr);
        }
    }

    free(threadData);
    return bw;
}

// one place to make memory allocation calls
void *allocate_memory(size_t bytes, uint32_t threadOffset) {
    void *dst               = NULL;
    int   posix_memalign_rc = 0;
    if (posix_memalign_rc != posix_memalign((void **)(&dst), 64, bytes)) {
        fprintf(stderr, "Could not allocate memory: %d\n", posix_memalign_rc);
        return NULL;
    }

    return dst;
}

__attribute__((fastcall)) float scalar_read(float *arr, size_t arr_length, uint64_t iterations, uint64_t start) {
    float sum = 0;
    if (start + 16 >= arr_length) return 0;

    size_t iter_idx = 0, i = start;
    float  s1 = 0, s2 = 1, s3 = 0, s4 = 1, s5 = 0, s6 = 1, s7 = 0, s8 = 1;
    while (iter_idx < iterations) {
        s1 += arr[i];
        s2 *= arr[i + 1];
        s3 += arr[i + 2];
        s4 *= arr[i + 3];
        s5 += arr[i + 4];
        s6 *= arr[i + 5];
        s7 += arr[i + 6];
        s8 *= arr[i + 7];
        i += 8;
        if (i + 7 >= arr_length) i = 0;
        if (i == start) iter_idx++;
    }

    sum += s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8;

    return sum;
}

void *ReadBandwidthTestThread(void *param) {
    BandwidthTestThreadData *bwTestData = (BandwidthTestThreadData *)param;
    if (hardaffinity) sched_setaffinity(gettid(), sizeof(cpu_set_t), &global_cpuset);
    float sum = bw_func(bwTestData->arr, bwTestData->arr_length, bwTestData->iterations, bwTestData->start);
    if (sum == 0.0F) printf("hark, a number: %.02f\n", sum);
    pthread_exit(NULL);
}
