include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := BoostClockChecker_amd64
SOURCES_BoostClockChecker_amd64 := BoostClockChecker.c BoostClockChecker_x86.s
PROGRAMS += CoreClockChecker_amd64
SOURCES_CoreClockChecker_amd64 := CoreClockChecker.c CoreClockChecker_x86.s
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := BoostClockChecker_aarch64
SOURCES_BoostClockChecker_aarch64 := BoostClockChecker.c BoostClockChecker_arm.s
endif
ifeq ($(TARGET),ppc64le)
PROGRAMS := BoostClockChecker_ppc64le
SOURCES_BoostClockChecker_ppc64le := BoostClockChecker.c BoostClockChecker_ppc64le.s
endif

CLEAN_PROGRAMS := BoostClockChecker_amd64 CoreClockChecker_amd64 BoostClockChecker_aarch64 BoostClockChecker_ppc64le
WIN_PROGRAM := BoostClockChecker_w64.exe
WIN_SOURCES := BoostClockChecker.c BoostClockChecker_x86.s
include ../Common/c-benchmark.mk
ifeq ($(TARGET),ppc64le)
all:
	@echo 'CoreClockChecker MSR/RAPL test: unsupported on POWER; built BoostClockChecker only.'
endif
