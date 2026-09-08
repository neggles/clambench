.abiversion 2
.text
/* ELFv2 leaf functions: only volatile GPRs, CR0 and CTR. No TOC required. */
.globl preplatencyarr
.type preplatencyarr,@function
preplatencyarr:
    cmpdi 4,0
    beqlr
    mtctr 4
    mr 5,3
1:  ld 6,0(5)
    sldi 6,6,3
    add 6,6,3
    std 6,0(5)
    addi 5,5,8
    bdnz 1b
    blr
.size preplatencyarr,.-preplatencyarr

.globl latencytest
.type latencytest,@function
latencytest:
    cmpdi 3,0
    beqlr
    mtctr 3
    ld 4,0(4)
1:  ld 4,0(4)
    bdnz 1b
    clrldi 3,4,32 /* uint32_t result: ELFv2 requires zero extension */
    blr
.size latencytest,.-latencytest

.globl longpatternlatencytest
.type longpatternlatencytest,@function
longpatternlatencytest:
    cmpdi 3,0
    beqlr
    mtctr 3
    li 5,0
    clrrdi 6,4,7 /* 128-byte POWER9/10 cache line */
    ld 4,0(4)
1:  ld 4,0(4)
    clrrdi 7,4,7
    cmpld 7,6
    bne 2f
    addi 5,5,8
    andi. 5,5,127
    add 4,7,5
2:  bdnz 1b
    mr 3,5 /* offset within first cache line */
    blr
.size longpatternlatencytest,.-longpatternlatencytest

/* Byte-granular offsets deliberately include misalignment/overlap. Indexed
   forms avoid DS-form displacement restrictions. Each load feeds next store;
   no fences inside the sequence: forwarding, not store completion, is timed. */
.macro stlf name, store, load
.globl \name
.type \name,@function
\name:
    cmpdi 3,0
    beqlr
    lwz 5,0(4)
    lwz 6,4(4)
    add 5,5,4
    add 6,6,4
    li 7,0
    mtctr 3
1:  \store 7,0,5
    \load 7,0,6
    bdnz 1b
    blr
.size \name,.-\name
.endm
stlf stlftest,stdx,lwzx
stlf stlftest32,stwx,lhzx
stlf matchedstlftest,stdx,ldx

.globl stlftest128
.type stlftest128,@function
stlftest128:
    cmpdi 3,0
    beqlr
    lwz 5,0(4)
    lwz 6,4(4)
    add 5,5,4
    add 6,6,4
    xxlxor 0,0,0
    mtctr 3
1:  stxvd2x 0,0,5
    lxsdx 0,0,6
    bdnz 1b
    blr
.size stlftest128,.-stlftest128
.section .note.GNU-stack,"",@progbits
