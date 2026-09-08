.abiversion 2
.text
.globl clktsctest
.type clktsctest,@function
/* Short samples require TB ticks, not millisecond wall time. TB frequency is
   independent of core frequency; the harness calibrates it against monotonic
   time. isync before mftb completes the preceding instruction stream; the
   compare/branch dependent on its result followed by isync prevents following
   instructions executing ahead of that read. The final add result similarly
   feeds a branch/isync before the end read. This is an ALU timing boundary,
   not a general-purpose store drain: memory benchmarks use monotonic time.
   Serialization overhead is included and not subtracted. No compiler can
   move instructions within this external assembly function. */
clktsctest:
    li 4,0
    cmpdi 3,0
    beqlr
    isync
    mftb 5
    cmpd 5,5
    bne- 1f
1:  isync
2:
    .rept 20
    addi 4,4,1
    .endr
    addic. 3,3,-20
    bgt 2b
    cmpd 4,4
    bne- 3f
3:  isync
    mftb 3
    subf 3,5,3
    blr
.size clktsctest,.-clktsctest
.section .note.GNU-stack,"",@progbits
