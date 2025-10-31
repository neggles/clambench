include ../Common/arch_detect.mk

CFLAGS := $(CFLAGS) -O3 -funroll-loops -ffast-math -fomit-frame-pointer -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-strict-aliasing
MARCH ?= native
LDFLAGS := $(LDFLAGS) -lm

all: $(TARGET)

amd64:
	$(CC) $(CFLAGS) -march=$(MARCH) x86_instructionrate.s x86_instructionrate.c -o InstructionRate_amd64 $(LDFLAGS)

aarch64:
	$(CC) $(CFLAGS) -march=$(MARCH) -pthread arm_instructionrate.s arm_instructionrate.c -o InstructionRate_aarch64 $(LDFLAGS)

riscv64:
	$(CC) $(CFLAGS) -march=rv64gc -pthread riscv_instructionrate.s riscv_instructionrate.c -o InstructionRate_riscv64 $(LDFLAGS)

termux:
	clang -march=$(MARCH) arm_instructionrate.s arm_instructionrate.c -o InstructionRate_aarch64 $(LDFLAGS)

amd64_fusion:
	$(CC) $(CFLAGS) -march=$(MARCH) x86_fusion.s x86_fusion.c -o InstructionRateFusion_amd64 $(LDFLAGS)

w64:
	$(CC) $(CFLAGS) -march=$(MARCH) x86_instructionrate.c x86_instructionrate.s -o InstructionRate_w64.exe $(LDFLAGS)

ci: amd64 amd64_fusion aarch64 riscv64 w64

clean:
	rm -f *.o && find . -type f -executable -delete

.PHONY: all ci clean
