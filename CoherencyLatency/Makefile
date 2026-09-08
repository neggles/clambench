include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := CoherencyLatency_amd64
SOURCES_CoherencyLatency_amd64 := PThreadsCoherencyLatency.c
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := CoherencyLatency_aarch64
SOURCES_CoherencyLatency_aarch64 := PThreadsCoherencyLatency.c
endif
ifeq ($(TARGET),ppc64le)
PROGRAMS := CoherencyLatency_ppc64le
SOURCES_CoherencyLatency_ppc64le := PThreadsCoherencyLatency.c
endif
ifeq ($(TARGET),riscv64)
PROGRAMS := CoherencyLatency_riscv64
SOURCES_CoherencyLatency_riscv64 := PThreadsCoherencyLatency.c
endif

CLEAN_PROGRAMS := CoherencyLatency_amd64 CoherencyLatency_aarch64 CoherencyLatency_ppc64le CoherencyLatency_riscv64
WIN_PROGRAM := CoherencyLatency_w64.exe
WIN_SOURCES := CoherencyLatency.cpp
include ../Common/c-benchmark.mk
