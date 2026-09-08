include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := InstructionRate_amd64
SOURCES_InstructionRate_amd64 := x86_instructionrate.c x86_instructionrate.s
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := InstructionRate_aarch64
SOURCES_InstructionRate_aarch64 := arm_instructionrate.c arm_instructionrate.s
endif
ifeq ($(TARGET),ppc64le)
PROGRAMS := InstructionRate_ppc64le
SOURCES_InstructionRate_ppc64le := ppc64le_instructionrate.c ../mt_instructionrate/ppc64_mt_instructionrate.s
endif
ifeq ($(TARGET),riscv64)
PROGRAMS := InstructionRate_riscv64
SOURCES_InstructionRate_riscv64 := riscv_instructionrate.c riscv_instructionrate.s
endif
ifeq ($(TARGET),amd64)
PROGRAMS += InstructionRateFusion_amd64
SOURCES_InstructionRateFusion_amd64 := x86_fusion.c x86_fusion.s
endif
amd64_fusion: InstructionRateFusion_amd64

CLEAN_PROGRAMS := InstructionRate_amd64 InstructionRate_aarch64 InstructionRate_ppc64le InstructionRate_riscv64 InstructionRateFusion_amd64
WIN_PROGRAM := InstructionRate_w64.exe
WIN_SOURCES := x86_instructionrate.c x86_instructionrate.s
include ../Common/c-benchmark.mk
