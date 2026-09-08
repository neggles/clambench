#undef NDEBUG /* Correctness checks must survive release-mode cross compilers. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Common/platform.h"
void FillPageByPage(uint32_t *, uint32_t, uint32_t);
void FillPageByPage64(uint64_t *, uint32_t, uint32_t);
int main(void) {
    size_t sizes[] = {1024,2048,BENCH_PAGE_SIZE,BENCH_PAGE_SIZE+1024,3*BENCH_PAGE_SIZE+4096};
    for (unsigned s=0;s<sizeof(sizes)/sizeof(*sizes);s++) {
        size_t bytes=sizes[s], lines=bytes/CACHELINE_SIZE;
        uint32_t *a=calloc(1,bytes);
        uint64_t *b=calloc(1,bytes);
        unsigned char *seen=calloc(lines,1);
        assert(a && b && seen);
        FillPageByPage(a,bytes/4,CACHELINE_SIZE);
        FillPageByPage64(b,bytes/8,CACHELINE_SIZE);
        uint32_t pos=0;
        for (size_t i=0;i<lines;i++) {
            assert(pos<bytes/4 && pos%(CACHELINE_SIZE/4)==0);
            assert(!seen[pos/(CACHELINE_SIZE/4)]++);
            pos=a[pos];
        }
        assert(!pos);
        for (unsigned lane=0;lane<CACHELINE_SIZE/8;lane++) {
            for (size_t i=0;i<lines;i++) seen[i]=0;
            uint64_t p=lane;
            for (size_t i=0;i<lines;i++) {
                assert(p<bytes/8 && p%(CACHELINE_SIZE/8)==lane);
                assert(!seen[p/(CACHELINE_SIZE/8)]++);
                p=b[p];
            }
            assert(p==lane);
        }
        free(a);free(b);free(seen);
    }
    puts("page patterns: PASS");
}
