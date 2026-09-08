# Detect the compiler target, not the build host. Honour CC and TARGET overrides.
COMPILER_TRIPLE := $(shell $(CC) -dumpmachine 2>/dev/null)
ifndef TARGET
ifneq ($(findstring mingw,$(COMPILER_TRIPLE)),)
TARGET := w64
else ifneq ($(findstring darwin,$(COMPILER_TRIPLE)),)
TARGET := darwin
else ifneq ($(filter x86_64%,$(COMPILER_TRIPLE)),)
TARGET := amd64
else ifneq ($(filter powerpc64le% ppc64le%,$(COMPILER_TRIPLE)),)
TARGET := ppc64le
else ifneq ($(filter aarch64% arm64%,$(COMPILER_TRIPLE)),)
TARGET := aarch64
else ifneq ($(filter riscv64%,$(COMPILER_TRIPLE)),)
TARGET := riscv64
else
TARGET := unsupported
endif
endif
