#define _GNU_SOURCE
#undef NDEBUG /* Correctness checks must survive release-mode cross compilers. */
#include <assert.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "../Common/platform.h"
extern uint64_t asm_read(char *, uint64_t, volatile int *, int);
extern uint64_t asm_add(char *, uint64_t, volatile int *, int);
static volatile int stop;
static uint32_t *data;
static uint64_t bytes;
static int rmw;
static void *worker(void *unused) {
    (void)unused;
    bytes = (rmw ? asm_add : asm_read)((char *)data,128,&stop,3);
    return NULL;
}
int main(void) {
    size_t page=BENCH_PAGE_SIZE;
    char *mem=mmap(NULL,2*page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    assert(mem != MAP_FAILED);
    assert(!mprotect(mem+page,page,PROT_NONE));
    data=(uint32_t *)(mem+page-128);
    for (rmw=0; rmw<2; rmw++) {
        for (int i=0;i<32;i++) data[i]=0;
        __atomic_store_n(&stop,0,__ATOMIC_RELAXED);
        pthread_t t;
        assert(!pthread_create(&t,NULL,worker,NULL));
        usleep(50000);
        __atomic_store_n(&stop,1,__ATOMIC_RELAXED);
        assert(!pthread_join(t,NULL));
        assert(bytes && bytes % (rmw ? 256 : 128) == 0);
        for (int i=0;i<32;i++) assert(data[i] == (rmw ? bytes/256 : 0));
        assert(!asm_read((char *)data,128,&stop,0));
    }
    munmap(mem,2*page);
    puts("POWER loaded kernels: PASS");
}
