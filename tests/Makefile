include ../Common/arch_detect.mk

ASM_amd64 := ../MemoryLatency/MemoryLatency_x86.s
ASM_ppc64le := ../MemoryLatency/MemoryLatency_ppc64le.s
PROGRAMS := patterns_$(TARGET)
SOURCES_patterns_$(TARGET) := patterns.c memory-harness_$(TARGET).o $(ASM_$(TARGET))
ifeq ($(TARGET),ppc64le)
PROGRAMS += power-memory power-loaded
SOURCES_power-memory := power-memory.c ../MemoryLatency/MemoryLatency_ppc64le.s ../MemoryBandwidth/MemoryBandwidth_ppc64le.s ../CoreClockChecker/BoostClockChecker_ppc64le.s ../mt_instructionrate/ppc64_mt_instructionrate.s
SOURCES_power-loaded := power-loaded.c ../LoadedMemoryLatency/LoadedMemoryLatency_ppc64le.s
endif
CLEAN_PROGRAMS := patterns_amd64 patterns_ppc64le power-memory power-loaded
include ../Common/c-benchmark.mk

memory-harness_$(TARGET).o: ../MemoryLatency/MemoryLatency.c FORCE $(wildcard ../Common/*.h)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(BENCH_FLAGS) -Dmain=benchmark_main -c $< -o $@

# Cross-build with `all`; run with e.g. RUNNER='qemu-ppc64le -L /sysroot'.
check: all
	@set -e; for test in $(PROGRAMS); do $(RUNNER) ./$$test; done
.PHONY: check
