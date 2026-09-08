include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := MemoryLatency_amd64
SOURCES_MemoryLatency_amd64 := MemoryLatency.c MemoryLatency_x86.s
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := MemoryLatency_aarch64
SOURCES_MemoryLatency_aarch64 := MemoryLatency.c MemoryLatency_arm.s
endif
ifeq ($(TARGET),ppc64le)
PROGRAMS := MemoryLatency_ppc64le
SOURCES_MemoryLatency_ppc64le := MemoryLatency.c MemoryLatency_ppc64le.s
endif
ifeq ($(TARGET),riscv64)
PROGRAMS := MemoryLatency_riscv64
SOURCES_MemoryLatency_riscv64 := MemoryLatency.c MemoryLatency_riscv.s
endif

ifeq ($(NUMA),1)
BASE_PROGRAMS := $(PROGRAMS)
PROGRAMS := $(subst _$(TARGET),_numa_$(TARGET),$(BASE_PROGRAMS))
$(foreach p,$(BASE_PROGRAMS),$(eval SOURCES_$(subst _$(TARGET),_numa_$(TARGET),$(p)) := $(SOURCES_$(p))))
endif

CLEAN_PROGRAMS := MemoryLatency_amd64 MemoryLatency_aarch64 MemoryLatency_ppc64le MemoryLatency_riscv64
CLEAN_PROGRAMS += MemoryLatency_numa_amd64 MemoryLatency_numa_aarch64 MemoryLatency_numa_ppc64le MemoryLatency_numa_riscv64
WIN_PROGRAM := MemoryLatency_w64.exe
WIN_SOURCES := MemoryLatency.cpp MemoryLatency_x86.s
include ../Common/c-benchmark.mk
