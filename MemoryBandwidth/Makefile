include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := MemoryBandwidth_amd64
SOURCES_MemoryBandwidth_amd64 := MemoryBandwidth.c MemoryBandwidth_x86.s
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := MemoryBandwidth_aarch64
SOURCES_MemoryBandwidth_aarch64 := MemoryBandwidth.c MemoryBandwidth_arm.s
endif
ifeq ($(TARGET),ppc64le)
PROGRAMS := MemoryBandwidth_ppc64le
SOURCES_MemoryBandwidth_ppc64le := MemoryBandwidth.c MemoryBandwidth_ppc64le.s
endif
ifeq ($(TARGET),riscv64)
PROGRAMS := MemoryBandwidth_riscv64
SOURCES_MemoryBandwidth_riscv64 := MemoryBandwidth.c MemoryBandwidth_riscv.s
endif

ifeq ($(NUMA),1)
BASE_PROGRAMS := $(PROGRAMS)
PROGRAMS := $(subst _$(TARGET),_numa_$(TARGET),$(BASE_PROGRAMS))
$(foreach p,$(BASE_PROGRAMS),$(eval SOURCES_$(subst _$(TARGET),_numa_$(TARGET),$(p)) := $(SOURCES_$(p))))
endif

CLEAN_PROGRAMS := MemoryBandwidth_amd64 MemoryBandwidth_aarch64 MemoryBandwidth_ppc64le MemoryBandwidth_riscv64
CLEAN_PROGRAMS += MemoryBandwidth_numa_amd64 MemoryBandwidth_numa_aarch64 MemoryBandwidth_numa_ppc64le MemoryBandwidth_numa_riscv64
WIN_PROGRAM := MemoryBandwidth_w64.exe
WIN_SOURCES := MemoryBandwidth.c MemoryBandwidth_x86.s
include ../Common/c-benchmark.mk
