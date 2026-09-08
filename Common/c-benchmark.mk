# Component sets PROGRAMS and SOURCES_<program>; compile flags remain overridable.
.DEFAULT_GOAL := all
CFLAGS ?= -O3
BENCH_FLAGS := -std=gnu11 -pthread
ifeq ($(TARGET),w64)
PROGRAMS := $(WIN_PROGRAM)
SOURCES_$(WIN_PROGRAM) := $(WIN_SOURCES)
ifneq ($(filter %.cpp,$(WIN_SOURCES)),)
BENCH_FLAGS := -pthread
endif
endif
CLEAN_PROGRAMS += $(WIN_PROGRAM)
LDLIBS += -lm
ifeq ($(TARGET),ppc64le)
POWER_CPU ?= power9
BENCH_FLAGS += -mcpu=$(POWER_CPU)
endif
ifeq ($(NUMA),1)
CPPFLAGS += -DNUMA
LDLIBS += -lnuma
endif
ifeq ($(strip $(PROGRAMS)),)
all:
	@echo 'Unsupported C benchmark target: $(TARGET)' >&2; exit 1
else
all: $(PROGRAMS)
endif

define build_program
$(1): FORCE $$(SOURCES_$(1)) $$(wildcard ../Common/*.h) Makefile ../Common/c-benchmark.mk ../Common/arch_detect.mk
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) $$(BENCH_FLAGS) $$(SOURCES_$(1)) $$(LDFLAGS) $$(LDLIBS) -o $$@
endef
$(foreach program,$(PROGRAMS),$(eval $(call build_program,$(program))))

amd64 aarch64 riscv64 ppc64le w64:
	$(MAKE) TARGET=$@ all
amd64-numa aarch64-numa ppc64le-numa:
	$(MAKE) TARGET=$(patsubst %-numa,%,$@) NUMA=1 all
ci: all
clean:
	rm -f $(CLEAN_PROGRAMS) *.o
.PHONY: all clean ci amd64 aarch64 riscv64 ppc64le w64 amd64-numa aarch64-numa ppc64le-numa

# As in the legacy makefiles, recompile on invocation. This also prevents an
# existing binary being reused after CC/CFLAGS/POWER_CPU changes.
FORCE:
.PHONY: FORCE
