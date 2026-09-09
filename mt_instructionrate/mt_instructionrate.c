#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Common/platform.h"
#include <math.h>
#include <string.h>

#ifndef _MSC_VER
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#ifdef __x86_64
#define SMALLKITTEN __attribute__((ms_abi))
#else
#define SMALLKITTEN
#endif
#define gettid() ((pid_t)syscall(SYS_gettid))
#else 
#include <Windows.h>
#define SMALLKITTEN
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "../Common/timing.h"
#include "../Common/bench_time.h"


struct TestThreadData {
    uint64_t timeNs;  // elapsed nanoseconds for this worker
    uint64_t iterations;
    void *testData;
    int core;     // -1 = don't set affinity. otherwise set affinity to specified core
    uint64_t (*testfunc)(uint64_t, void *) SMALLKITTEN;
};

float measureFunction(uint64_t baseIterations, uint64_t (*testFunc)(uint64_t, void *) SMALLKITTEN, void *data);
void *TestThread(void *param);

int threadCount = 1;
int *coreList = NULL;

#ifdef __aarch64__
#include "arm_mt_instructionrate.c"
#endif 

#ifdef __x86_64
#include "x86_mt_instructionrate.c"
#endif

#ifdef _MSC_VER
#include "x86_mt_instructionrate.c"
#endif


#if defined(__powerpc64__) || defined(__PPC64__)
#include "ppc64_mt_instructionrate.c"
#endif

int main(int argc, char *argv[]) {
    bench_require_power9();
   char parseBuffer[512];
   int parseIndices[64];

   for (int argIdx = 1; argIdx < argc; argIdx++) {
      if (*(argv[argIdx]) == '-') {
        char *arg = argv[argIdx] + 1;
	if (strncmp(arg, "threads", 7) == 0) {
	  argIdx++;
	  threadCount = atoi(argv[argIdx]);
	  fprintf(stderr, "Using first %d cores\n", threadCount);
	} else if (strncmp(arg, "cores", 5) == 0) {
	  argIdx++;
	  
	  // whatever just parse it here
	  snprintf(parseBuffer, sizeof(parseBuffer), "%s", argv[argIdx]);
          parseIndices[0] = 0;
          int indexIdx = 1;
          threadCount = 1;
          for (int i = 0; i < 512 && parseBuffer[i] && indexIdx < 64; i++) {
            if (parseBuffer[i] == ',') {
              parseBuffer[i] = '\0';
              parseIndices[indexIdx] = i + 1;
              indexIdx++;
              threadCount++;
            }
          }

          coreList = malloc(sizeof(int) * threadCount);

          fprintf(stderr, "Using %d cores:", threadCount);
          for (int i = 0;i < threadCount; i++) {
            coreList[i] = atoi(parseBuffer + parseIndices[i]);
            fprintf(stderr, " %d", coreList[i]); 
          }

          fprintf(stderr, "\n");
	}
      }
   }

   if (threadCount <= 0 || threadCount > 1024) {
       fprintf(stderr, "Thread count must be in 1..1024\n");
       return 1;
   }
   RunTests();

   free(coreList);
   return 0;
}

// return billion operations per second
// test function must perform iterations ops
float measureFunction(uint64_t baseIterations, uint64_t (*testFunc)(uint64_t, void *) SMALLKITTEN, void *data){
  int toleranceMet = 0, minTimeMet = 0;
  uint64_t timeNs;
  struct timespec begin, end;
  
  struct TestThreadData *testData = (struct TestThreadData *)malloc(threadCount * sizeof(struct TestThreadData));
  for (int threadIdx = 0; threadIdx < threadCount; threadIdx++) {
    testData[threadIdx].iterations = baseIterations;
    testData[threadIdx].testData = data;
    testData[threadIdx].testfunc = testFunc;
    if (coreList == NULL) testData[threadIdx].core = threadIdx;
    else testData[threadIdx].core = coreList[threadIdx];
  }

#ifndef _MSC_VER
  pthread_t* testThreads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
#else
  HANDLE* testThreads = (HANDLE*)malloc(threadCount * sizeof(HANDLE));
#endif

  do {
    bench_now(&begin);
    for (int threadIdx = 0; threadIdx < threadCount; threadIdx++) {
#ifndef _MSC_VER
      int rc = pthread_create(testThreads + threadIdx, NULL, TestThread, testData + threadIdx);
      if (rc) { fprintf(stderr, "pthread_create: %s\n", strerror(rc)); exit(1); }
#else
      testThreads[threadIdx] = CreateThread(NULL, 0, TestThread, testData + threadIdx, CREATE_SUSPENDED, NULL, NULL);
      SetThreadAffinityMask(testThreads[threadIdx], 1UL << testData[threadIdx].core);
      ResumeThread(testThreads[threadIdx]);
#endif
    }

    uint64_t maxThreadTime = 0, minThreadTime = UINT64_MAX;
    for (int threadIdx = 0; threadIdx < threadCount; threadIdx++) {
#ifndef _MSC_VER
      pthread_join(testThreads[threadIdx], NULL);
#else
      WaitForMultipleObjects((DWORD)threadCount, testThreads, TRUE, INFINITE);
#endif
      fprintf(stderr, "Thread %d took %f ms\n", threadIdx, testData[threadIdx].timeNs / 1e6);
      if (testData[threadIdx].timeNs > maxThreadTime) maxThreadTime = testData[threadIdx].timeNs;
      if (testData[threadIdx].timeNs < minThreadTime) minThreadTime = testData[threadIdx].timeNs;
    }

    bench_now(&end);
    timeNs = bench_elapsed_ns(&begin, &end);
    minTimeMet = timeNs > UINT64_C(2000000000); // see if 2 seconds will work
    toleranceMet = minThreadTime > 0 && ((double)(maxThreadTime - minThreadTime) / minThreadTime) < 0.2; // allow 20% variation

    if (!minTimeMet) {
      // Increase iteration count with 3s target
      baseIterations = scale_iterations_to_target_ns(baseIterations, timeNs, UINT64_C(3000000000));
      for (int threadIdx = 0; threadIdx < threadCount; threadIdx++) {
        testData[threadIdx].iterations = baseIterations;
      }

      fprintf(stderr, "Setting %lu iterations\n", baseIterations);
    } else if (!toleranceMet) {
      for (int threadIdx = 0; threadIdx < threadCount; threadIdx++) {
        testData[threadIdx].iterations = scale_iterations_to_target_ns(
          testData[threadIdx].iterations,
          testData[threadIdx].timeNs,
          maxThreadTime);
        fprintf(stderr, "Thread %d -> %lu iterations\n", threadIdx, testData[threadIdx].iterations); 
      }
    }
  } while ((!toleranceMet) || (!minTimeMet));

  fprintf(stderr, "time elapsed: %.6f ms\n", timeNs / 1e6);

  uint64_t totalIterations = 0;
  for (int threadIdx = 0; threadIdx < threadCount; threadIdx++) {
    totalIterations += testData[threadIdx].iterations;
  }

  free(testData);
  free(testThreads);

  return (double)totalIterations / (double)timeNs;
}

void *TestThread(void *param) {
  struct TestThreadData *testData = (struct TestThreadData *)param;

#ifndef _MSC_VER
  if (testData->core >= 0) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    if (testData->core >= CPU_SETSIZE) { fprintf(stderr, "CPU index too large\n"); exit(1); }
    CPU_SET(testData->core, &cpuset);
    if (sched_setaffinity(gettid(), sizeof(cpu_set_t), &cpuset)) {
        perror("sched_setaffinity (use -cores for CPUs allowed by your cpuset)");
        exit(1);
    }
  }
  
#endif
  struct timespec begin, end;
  bench_now(&begin);
  testData->testfunc(testData->iterations, testData->testData);
  bench_now(&end);
  testData->timeNs = bench_elapsed_ns(&begin, &end);

  return NULL;
}
