# detect target
DEF_TARGET := amd64
ifeq ($(OS),Windows_NT)
    DEF_TARGET := w64
else
    UNAME_M := $(shell uname -m)
    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_M),x86_64)
        DEF_TARGET := amd64
    else ifeq ($(UNAME_M),aarch64)
        DEF_TARGET := aarch64
    else ifeq ($(UNAME_M),riscv64)
        DEF_TARGET := riscv64
    else ifeq ($(UNAME_M),powerpc64)
        DEF_TARGET := ppc64
    else ifeq ($(UNAME_M),ppc64)
        DEF_TARGET := ppc64
    endif

    ifeq ($(UNAME_S),Darwin)
        DEF_TARGET := darwin
    endif
endif

# only set TARGET if the user did not specify one
TARGET ?= $(DEF_TARGET)

amd64: CC := x86_64-linux-gnu-gcc
amd64_numa: CC := x86_64-linux-gnu-gcc
aarch64: CC := aarch64-linux-gnu-gcc
aarch64_numa: CC := aarch64-linux-gnu-gcc
riscv64: CC := riscv64-linux-gnu-gcc
w64: CC := x86_64-w64-mingw32-gcc
darwin: CC := clang
ppc64: CC := powerpc64-linux-gnu-gcc
