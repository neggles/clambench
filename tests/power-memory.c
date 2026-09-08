#define _GNU_SOURCE
#undef NDEBUG /* Correctness checks must survive release-mode cross compilers. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include "../Common/platform.h"
#include "../Common/power_kernels.h"

extern void preplatencyarr(uint64_t *, uint64_t);
extern uint32_t latencytest(uint64_t, uint64_t *);
extern uint32_t longpatternlatencytest(uint64_t, uint64_t *);
extern void stlftest(uint64_t, char *), stlftest32(uint64_t, char *);
extern void matchedstlftest(uint64_t, char *), stlftest128(uint64_t, char *);
extern uint64_t clktsctest(uint64_t);
extern float asm_read(float *, uint64_t, uint64_t, uint64_t);
extern float asm_write(float *, uint64_t, uint64_t, uint64_t);
extern float asm_copy(float *, uint64_t, uint64_t, uint64_t);
extern float asm_add(float *, uint64_t, uint64_t, uint64_t);
extern float asm_cflip(float *, uint64_t, uint64_t, uint64_t);

int main(void) {
    bench_require_power9();
    uint64_t ring[64] __attribute__((aligned(128)));
    for (int i = 0; i < 64; i++) ring[i] = (i + 1) % 64;
    preplatencyarr(ring, 64);
    for (int i = 0; i < 64; i++) assert(ring[i] == (uintptr_t)&ring[(i+1)%64]);
    for (int n = 1; n < 130; n++)
        assert(latencytest(n, ring) == (uint32_t)(uintptr_t)&ring[(n+1)%64]);
    for (int lane = 0; lane < 16; lane++)
        for (int line = 0; line < 4; line++)
            ring[16*line+lane] = (uintptr_t)&ring[16*((line+1)%4)+lane];
    uint32_t observed = longpatternlatencytest(3, ring);
    if (observed != 8) fprintf(stderr, "longpattern: base=%p next=%lx/%lx/%lx/%lx result=%u\n",
                             (void *)ring,ring[0],ring[16],ring[32],ring[48],observed);
    assert(observed == 8);
    assert(longpatternlatencytest(7, ring) == 16);

    /* Buffer ends at a protected page: catches one-chunk streaming overruns. */
    size_t page = BENCH_PAGE_SIZE;
    char *mem = mmap(NULL, 2*page, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    assert(mem != MAP_FAILED);
    assert(!mprotect(mem+page, page, PROT_NONE));
    float *a = (float *)(mem+page-1024);
    for (int i=0; i<256; i++) a[i] = i+1;
    asm_read(a,256,2,0);
    for (int i=0; i<256; i++) assert(a[i] == i+1);
    asm_copy(a,256,2,0);
    for (int i=0; i<128; i++) assert(a[i+128] == a[i]);
    asm_write(a,256,1,0);
    for (int i=0; i<256; i++) assert(a[i] == 1);
    asm_add(a,256,2,0); /* one read/write pass */
    for (int i=0; i<256; i++) assert(a[i] == 2);
    for (int i=0; i<256; i++) a[i] = i+1;
    asm_cflip(a,256,2,0);
    for (int i=0; i<256; i++) assert(a[i] == (i/32)*32 + (7-(i%32)/4)*4 + i%4 + 1);
    asm_cflip(a,256,2,0);
    for (int i=0; i<256; i++) assert(a[i] == i+1);

    void (*forward[])(uint64_t,char *) = {stlftest, stlftest32, matchedstlftest, stlftest128};
    char *buf = mem+page-512;
    for (unsigned f=0; f<4; f++)
        for (uint32_t st=0; st<128; st++)
            for (uint32_t ld=0; ld<128; ld++) {
                memset(buf, 0, 512);
                ((uint32_t *)buf)[0] = st;
                ((uint32_t *)buf)[1] = ld;
                forward[f](2, buf);
            }
    double fp[16] __attribute__((aligned(128)));
    for (int i=0;i<16;i++) fp[i]=1;
    uint64_t counts[] = {0,1,31,32,33,65};
    for (unsigned i=0;i<sizeof(counts)/sizeof(*counts);i++) {
        add_test(counts[i],fp); add_latency(counts[i],fp);
        mul_test(counts[i],fp); mul_latency(counts[i],fp);
        branch_test(counts[i],fp); branch_not_taken_test(counts[i],fp);
        fp64_fma_test(counts[i],fp); vec_fp32_fma_test(counts[i],fp);
    }
    assert(clktsctest(0) == 0);
    assert(clktsctest(20000) > 0);
    munmap(mem,2*page);
    puts("POWER memory kernels: PASS");
}
