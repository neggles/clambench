include Common/arch_detect.mk

COMPONENTS = CoherencyLatency MemoryLatency MemoryBandwidth InstructionRate Meshsim CoreClockChecker GpuMemLatency

C_COMPONENTS = CoherencyLatency MemoryLatency MemoryBandwidth InstructionRate CoreClockChecker LoadedMemoryLatency mt_instructionrate

all: c

c: $(C_COMPONENTS)

legacy-all: $(COMPONENTS)

ci: c

legacy-ci:
	@set -e; for COMPONENT in $(COMPONENTS); do $(MAKE) -C $$COMPONENT ci; done

package:
	@sh Common/ci_package.sh

clean-package:
	find . -maxdepth 1 -type d -name "clammarks-*" -exec rm -rf {} \; && rm -f "clammarks.txz"

clean:
	@set -e; for COMPONENT in $(C_COMPONENTS) tests; do $(MAKE) -C $$COMPONENT clean; done

$(sort $(COMPONENTS) $(C_COMPONENTS)): .FORCE
	$(MAKE) -C $@ 

.FORCE:

.PHONY: all c legacy-all legacy-ci ci package clean-package clean

check:
	$(MAKE) -C tests check
.PHONY: check
