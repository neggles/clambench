.PHONY: all ci clean print

# default target selection if not specified
ifeq ($(TARGET),)
    ifeq ($(OS),Windows_NT)
        TARGET := w64
    else
        UNAME_S := $(shell uname -s)
        ifeq ($(UNAME_S),Darwin)
            LLVM ?= 1
            TARGET := darwin
        else ifeq ($(UNAME_S),Linux)
            TARGET := linux
        endif
    endif
endif

# numa?
ifneq ($(filter %-numa,$(TARGET)),)
    $(info Compiling with NUMA-awareness)
    SUFFIX	+= -numa
    CPPFLAGS	+= -DNUMA
    LDFLAGS	+= -lnuma
endif

# pick asm arch based on target if not specified
ifeq ($(TARGET),$(filter $(TARGET),linux darwin))
    ARCH ?= $(shell uname -m)
    ifeq ($(ARCH),x86_64)
        ASM_ARCH ?= x86
        CROSS_COMPILE := x86_64-linux-gnu-gcc
    else ifneq ($(filter %86,$(ARCH)),)
        ASM_ARCH ?= i686
        CROSS_COMPILE ?= i686-linux-gnu-
    else ifneq ($(filter aarch64%,$(ARCH)),)
        ASM_ARCH ?= arm
        CROSS_COMPILE ?= aarch64-linux-gnu-
    else ifneq ($(filter riscv64%,$(ARCH)),)
        ASM_ARCH ?= riscv64
        CROSS_COMPILE ?= riscv64-linux-gnu-
        CFLAGS += -march=rv64gcv0p7
    endif
else ifeq ($(TARGET),$(filter $(TARGET),w64))
    SUFFIX += .exe
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        ASM_ARCH ?= x86
    else ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        ASM_ARCH ?= i686
    endif
    ifneq ($(OS),Windows_NT)
	echo "Cross-compiling for Windows on non-Windows host"
        CROSS_COMPILE ?= $(ASM_ARCH)-w64-mingw32-
    endif
else
    error "Unknown target: $(TARGET)"
endif

# Make variables (CC, etc...)
ifneq ($(LLVM),)
    $(info Using LLVM)
    CC		= $(LLVM_PREFIX)clang$(LLVM_SUFFIX)
    CXX		= $(LLVM_PREFIX)clang++$(LLVM_SUFFIX)
    LD		= $(LLVM_PREFIX)ld.lld$(LLVM_SUFFIX)
    AS		= $(LLVM_PREFIX)llvm-as$(LLVM_SUFFIX)
    STRIP	= $(LLVM_PREFIX)llvm-strip$(LLVM_SUFFIX)
else
    CC		?= $(CROSS_COMPILE)gcc
    CXX		?= $(CROSS_COMPILE)g++
    LD		?= $(CROSS_COMPILE)ld
    AS		?= $(CROSS_COMPILE)as
    STRIP	?= $(CROSS_COMPILE)strip
endif
CPP		= $(CC) $(CPPFLAGS)

# Build rules
linux: $(BENCHMARK)_$(ASM_ARCH)$(SUFFIX)
darwin: $(BENCHMARK)_$(ASM_ARCH)$(SUFFIX)
w64: $(BENCHMARK)_$(ASM_ARCH)$(SUFFIX).exe

# numa-aware builds
%-numa:
	$(MAKE) $(subst -numa,,$@) TARGET=$@

# actual build rules
$(BENCHMARK)_$(ASM_ARCH)$(SUFFIX): $(BENCHMARK).c $(BENCHMARK)_$(ASM_ARCH).s
	$(CC) $(CFLAGS) $(BENCHMARK).c $(BENCHMARK)_$(ASM_ARCH).s -o $(BENCHMARK)_$(ASM_ARCH)$(SUFFIX) $(LDFLAGS)

$(BENCHMARK)_$(ASM_ARCH)$(SUFFIX).exe: $(BENCHMARK).c $(BENCHMARK)_$(ASM_ARCH).s
	$(CXX) $(CFLAGS) $(BENCHMARK).cpp $(BENCHMARK)_$(ASM_ARCH).s -o $(BENCHMARK)_$(ASM_ARCH)$(SUFFIX) $(LDFLAGS)

# all the targets
all: linux linux-numa darwin w64
# all the targets for CI
ci: linux linux-numa darwin w64

# clean up
clean:
	rm -f *.o && find . -type f -executable -delete
# print variables
print:
	$(foreach v, $(.VARIABLES), $(if $(filter file,$(origin $(v))), $(info $(v)=$($(v)))))

# print build info
ifeq (,$(filter clean,$(MAKECMDGOALS)))
    $(info Building for $(TARGET) with $(CC) and $(ASM_ARCH) assembly)
endif
