include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := x86_mt_instructionrate
SOURCES_x86_mt_instructionrate := mt_instructionrate.c x86_mt_instructionrate.s ../Common/timing.c
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := arm_mt_instructionrate
SOURCES_arm_mt_instructionrate := mt_instructionrate.c arm_mt_instructionrate.s ../Common/timing.c
endif

CLEAN_PROGRAMS := x86_mt_instructionrate arm_mt_instructionrate ppc64_mt_instructionrate
include ../Common/c-benchmark.mk

# Textually included architecture test tables are rebuild dependencies.
$(PROGRAMS): $(wildcard *_mt_instructionrate.c)
x86: amd64
ppc64: ppc64le
.PHONY: x86 ppc64
