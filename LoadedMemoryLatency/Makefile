include ../Common/arch_detect.mk

ifeq ($(TARGET),amd64)
PROGRAMS := loadedlat_amd64
SOURCES_loadedlat_amd64 := LoadedMemoryLatency.c LoadedMemoryLatency_amd64.s
endif
ifeq ($(TARGET),aarch64)
PROGRAMS := loadedlat_aarch64
SOURCES_loadedlat_aarch64 := LoadedMemoryLatency.c LoadedMemoryLatency_arm.s
endif
ifeq ($(TARGET),ppc64le)
PROGRAMS := loadedlat_ppc64le
SOURCES_loadedlat_ppc64le := LoadedMemoryLatency.c LoadedMemoryLatency_ppc64le.s
endif

CLEAN_PROGRAMS := loadedlat_amd64 loadedlat_aarch64 loadedlat_ppc64le
include ../Common/c-benchmark.mk
