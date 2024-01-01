BENCHMARK := MemoryBandwidth
CFLAGS = -pthread -O2 -std=gnu17 -march=native
LDFLAGS= -lm

include ../Common/arch_detect.mk

termux:
	$(CC) $(CFLAGS) $(BENCHMARK).c $(BENCHMARK)_arm.s -o $(BENCHMARK)_termux_aarch64 $(LDFLAGS)


clam: $(BENCHMARK).c $(BENCHMARK)_$(ASM_ARCH).s
	$(CC) $(CFLAGS) $(BENCHMARK)_clam.c $(BENCHMARK)_$(ASM_ARCH).s -o $(BENCHMARK)_$(ASM_ARCH)_clam $(LDFLAGS)
