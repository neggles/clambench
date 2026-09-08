.abiversion 2
.text
/* ELFv2, POWER9 baseline. Count is tested instructions (not scalar elements
   or FLOPs), exactly, including a remainder. 32-instruction steady-state body,
   eight independent destinations for throughput and one for latency.
   Only volatile GPR3..12, FPR0..10, VR0..10, CR0 and CTR are modified.
   FP operands stay finite: +0, *1, and accumulator + 1*0. */

.globl add_test
.type add_test,@function
add_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    add 3,3,11
    add 4,4,11
    add 5,5,11
    add 6,6,11
    add 7,7,11
    add 8,8,11
    add 9,9,11
    add 10,10,11
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  add 3,3,11
    bdnz 4b
5:  li 3,0
    blr
.size add_test,.-add_test

.globl add_latency
.type add_latency,@function
add_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    add 3,3,11
    add 3,3,11
    add 3,3,11
    add 3,3,11
    add 3,3,11
    add 3,3,11
    add 3,3,11
    add 3,3,11
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  add 3,3,11
    bdnz 4b
5:  li 3,0
    blr
.size add_latency,.-add_latency

.globl mul_test
.type mul_test,@function
mul_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    mulld 3,3,11
    mulld 4,4,11
    mulld 5,5,11
    mulld 6,6,11
    mulld 7,7,11
    mulld 8,8,11
    mulld 9,9,11
    mulld 10,10,11
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  mulld 3,3,11
    bdnz 4b
5:  li 3,0
    blr
.size mul_test,.-mul_test

.globl mul_latency
.type mul_latency,@function
mul_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    mulld 3,3,11
    mulld 3,3,11
    mulld 3,3,11
    mulld 3,3,11
    mulld 3,3,11
    mulld 3,3,11
    mulld 3,3,11
    mulld 3,3,11
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  mulld 3,3,11
    bdnz 4b
5:  li 3,0
    blr
.size mul_latency,.-mul_latency

.globl xor_test
.type xor_test,@function
xor_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xor 3,3,11
    xor 4,4,11
    xor 5,5,11
    xor 6,6,11
    xor 7,7,11
    xor 8,8,11
    xor 9,9,11
    xor 10,10,11
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xor 3,3,11
    bdnz 4b
5:  li 3,0
    blr
.size xor_test,.-xor_test

.globl rotate_test
.type rotate_test,@function
rotate_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    rotldi 3,3,7
    rotldi 4,4,7
    rotldi 5,5,7
    rotldi 6,6,7
    rotldi 7,7,7
    rotldi 8,8,7
    rotldi 9,9,7
    rotldi 10,10,7
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  rotldi 3,3,7
    bdnz 4b
5:  li 3,0
    blr
.size rotate_test,.-rotate_test

.globl nop_test
.type nop_test,@function
nop_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  nop
    bdnz 4b
5:  li 3,0
    blr
.size nop_test,.-nop_test

.globl branch_test
.type branch_test,@function
branch_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    b 8f
8:
    b 8f
8:
    b 8f
8:
    b 8f
8:
    b 8f
8:
    b 8f
8:
    b 8f
8:
    b 8f
8:
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  b 8f
8:
    bdnz 4b
5:  li 3,0
    blr
.size branch_test,.-branch_test

.globl branch_not_taken_test
.type branch_not_taken_test,@function
branch_not_taken_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    li 11,1
    li 3,1
    li 4,1
    li 5,1
    li 6,1
    li 7,1
    li 8,1
    li 9,1
    li 10,1
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    bne 8f
8:
    bne 8f
8:
    bne 8f
8:
    bne 8f
8:
    bne 8f
8:
    bne 8f
8:
    bne 8f
8:
    bne 8f
8:
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  bne 8f
8:
    bdnz 4b
5:  li 3,0
    blr
.size branch_not_taken_test,.-branch_not_taken_test

.globl vec_int32_add_test
.type vec_int32_add_test,@function
vec_int32_add_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    vadduwm 0,0,8
    vadduwm 1,1,8
    vadduwm 2,2,8
    vadduwm 3,3,8
    vadduwm 4,4,8
    vadduwm 5,5,8
    vadduwm 6,6,8
    vadduwm 7,7,8
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  vadduwm 0,0,8
    bdnz 4b
5:  li 3,0
    blr
.size vec_int32_add_test,.-vec_int32_add_test

.globl vec_int32_add_latency
.type vec_int32_add_latency,@function
vec_int32_add_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    vadduwm 0,0,8
    vadduwm 0,0,8
    vadduwm 0,0,8
    vadduwm 0,0,8
    vadduwm 0,0,8
    vadduwm 0,0,8
    vadduwm 0,0,8
    vadduwm 0,0,8
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  vadduwm 0,0,8
    bdnz 4b
5:  li 3,0
    blr
.size vec_int32_add_latency,.-vec_int32_add_latency

.globl vec_int32_mul_test
.type vec_int32_mul_test,@function
vec_int32_mul_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    vmuluwm 0,0,8
    vmuluwm 1,1,8
    vmuluwm 2,2,8
    vmuluwm 3,3,8
    vmuluwm 4,4,8
    vmuluwm 5,5,8
    vmuluwm 6,6,8
    vmuluwm 7,7,8
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  vmuluwm 0,0,8
    bdnz 4b
5:  li 3,0
    blr
.size vec_int32_mul_test,.-vec_int32_mul_test

.globl vec_int32_mul_latency
.type vec_int32_mul_latency,@function
vec_int32_mul_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    vmuluwm 0,0,8
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  vmuluwm 0,0,8
    bdnz 4b
5:  li 3,0
    blr
.size vec_int32_mul_latency,.-vec_int32_mul_latency

.globl vec_fp32_add_test
.type vec_fp32_add_test,@function
vec_fp32_add_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xvaddsp 32,32,41
    xvaddsp 33,33,41
    xvaddsp 34,34,41
    xvaddsp 35,35,41
    xvaddsp 36,36,41
    xvaddsp 37,37,41
    xvaddsp 38,38,41
    xvaddsp 39,39,41
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xvaddsp 32,32,41
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_add_test,.-vec_fp32_add_test

.globl vec_fp32_add_latency
.type vec_fp32_add_latency,@function
vec_fp32_add_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    xvaddsp 32,32,41
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xvaddsp 32,32,41
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_add_latency,.-vec_fp32_add_latency

.globl vec_fp32_mul_test
.type vec_fp32_mul_test,@function
vec_fp32_mul_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xvmulsp 32,32,42
    xvmulsp 33,33,42
    xvmulsp 34,34,42
    xvmulsp 35,35,42
    xvmulsp 36,36,42
    xvmulsp 37,37,42
    xvmulsp 38,38,42
    xvmulsp 39,39,42
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xvmulsp 32,32,42
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_mul_test,.-vec_fp32_mul_test

.globl vec_fp32_mul_latency
.type vec_fp32_mul_latency,@function
vec_fp32_mul_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    xvmulsp 32,32,42
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xvmulsp 32,32,42
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_mul_latency,.-vec_fp32_mul_latency

.globl vec_fp32_fma_test
.type vec_fp32_fma_test,@function
vec_fp32_fma_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xvmaddasp 32,42,41
    xvmaddasp 33,42,41
    xvmaddasp 34,42,41
    xvmaddasp 35,42,41
    xvmaddasp 36,42,41
    xvmaddasp 37,42,41
    xvmaddasp 38,42,41
    xvmaddasp 39,42,41
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xvmaddasp 32,42,41
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_fma_test,.-vec_fp32_fma_test

.globl vec_fp32_fma_latency
.type vec_fp32_fma_latency,@function
vec_fp32_fma_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    xvmaddasp 32,42,41
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  xvmaddasp 32,42,41
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_fma_latency,.-vec_fp32_fma_latency

.globl vec_fp32_isqrt_test
.type vec_fp32_isqrt_test,@function
vec_fp32_isqrt_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    vspltisw 8,1
    xxlxor 41,41,41
    xvcvsxwsp 42,40
    xxlor 32,42,42
    xxlor 33,42,42
    xxlor 34,42,42
    xxlor 35,42,42
    xxlor 36,42,42
    xxlor 37,42,42
    xxlor 38,42,42
    xxlor 39,42,42
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    vrsqrtefp 0,0
    vrsqrtefp 1,1
    vrsqrtefp 2,2
    vrsqrtefp 3,3
    vrsqrtefp 4,4
    vrsqrtefp 5,5
    vrsqrtefp 6,6
    vrsqrtefp 7,7
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  vrsqrtefp 0,0
    bdnz 4b
5:  li 3,0
    blr
.size vec_fp32_isqrt_test,.-vec_fp32_isqrt_test

.globl fp64_add_test
.type fp64_add_test,@function
fp64_add_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    lfd 10,0(4)
    fsub 9,10,10
    fmr 0,10
    fmr 1,10
    fmr 2,10
    fmr 3,10
    fmr 4,10
    fmr 5,10
    fmr 6,10
    fmr 7,10
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    fadd 0,0,9
    fadd 1,1,9
    fadd 2,2,9
    fadd 3,3,9
    fadd 4,4,9
    fadd 5,5,9
    fadd 6,6,9
    fadd 7,7,9
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  fadd 0,0,9
    bdnz 4b
5:  li 3,0
    blr
.size fp64_add_test,.-fp64_add_test

.globl fp64_add_latency
.type fp64_add_latency,@function
fp64_add_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    lfd 10,0(4)
    fsub 9,10,10
    fmr 0,10
    fmr 1,10
    fmr 2,10
    fmr 3,10
    fmr 4,10
    fmr 5,10
    fmr 6,10
    fmr 7,10
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    fadd 0,0,9
    fadd 0,0,9
    fadd 0,0,9
    fadd 0,0,9
    fadd 0,0,9
    fadd 0,0,9
    fadd 0,0,9
    fadd 0,0,9
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  fadd 0,0,9
    bdnz 4b
5:  li 3,0
    blr
.size fp64_add_latency,.-fp64_add_latency

.globl fp64_mul_test
.type fp64_mul_test,@function
fp64_mul_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    lfd 10,0(4)
    fsub 9,10,10
    fmr 0,10
    fmr 1,10
    fmr 2,10
    fmr 3,10
    fmr 4,10
    fmr 5,10
    fmr 6,10
    fmr 7,10
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    fmul 0,0,10
    fmul 1,1,10
    fmul 2,2,10
    fmul 3,3,10
    fmul 4,4,10
    fmul 5,5,10
    fmul 6,6,10
    fmul 7,7,10
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  fmul 0,0,10
    bdnz 4b
5:  li 3,0
    blr
.size fp64_mul_test,.-fp64_mul_test

.globl fp64_mul_latency
.type fp64_mul_latency,@function
fp64_mul_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    lfd 10,0(4)
    fsub 9,10,10
    fmr 0,10
    fmr 1,10
    fmr 2,10
    fmr 3,10
    fmr 4,10
    fmr 5,10
    fmr 6,10
    fmr 7,10
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    fmul 0,0,10
    fmul 0,0,10
    fmul 0,0,10
    fmul 0,0,10
    fmul 0,0,10
    fmul 0,0,10
    fmul 0,0,10
    fmul 0,0,10
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  fmul 0,0,10
    bdnz 4b
5:  li 3,0
    blr
.size fp64_mul_latency,.-fp64_mul_latency

.globl fp64_fma_test
.type fp64_fma_test,@function
fp64_fma_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    lfd 10,0(4)
    fsub 9,10,10
    fmr 0,10
    fmr 1,10
    fmr 2,10
    fmr 3,10
    fmr 4,10
    fmr 5,10
    fmr 6,10
    fmr 7,10
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    fmadd 0,0,10,9
    fmadd 1,1,10,9
    fmadd 2,2,10,9
    fmadd 3,3,10,9
    fmadd 4,4,10,9
    fmadd 5,5,10,9
    fmadd 6,6,10,9
    fmadd 7,7,10,9
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  fmadd 0,0,10,9
    bdnz 4b
5:  li 3,0
    blr
.size fp64_fma_test,.-fp64_fma_test

.globl fp64_fma_latency
.type fp64_fma_latency,@function
fp64_fma_latency:
    cmpdi 3,0
    beqlr
    mr 12,3
    lfd 10,0(4)
    fsub 9,10,10
    fmr 0,10
    fmr 1,10
    fmr 2,10
    fmr 3,10
    fmr 4,10
    fmr 5,10
    fmr 6,10
    fmr 7,10
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    fmadd 0,0,10,9
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  fmadd 0,0,10,9
    bdnz 4b
5:  li 3,0
    blr
.size fp64_fma_latency,.-fp64_fma_latency

.globl load_test
.type load_test,@function
load_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    mr 11,4
    li 3,1
    xxlxor 32,32,32
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    ld 3,0(11)
    ld 4,0(11)
    ld 5,0(11)
    ld 6,0(11)
    ld 7,0(11)
    ld 8,0(11)
    ld 9,0(11)
    ld 10,0(11)
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  ld 3,0(11)
    bdnz 4b
5:  li 3,0
    blr
.size load_test,.-load_test

.globl vector_load_test
.type vector_load_test,@function
vector_load_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    mr 11,4
    li 3,1
    xxlxor 32,32,32
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    lxv 32,0(11)
    lxv 33,0(11)
    lxv 34,0(11)
    lxv 35,0(11)
    lxv 36,0(11)
    lxv 37,0(11)
    lxv 38,0(11)
    lxv 39,0(11)
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  lxv 32,0(11)
    bdnz 4b
5:  li 3,0
    blr
.size vector_load_test,.-vector_load_test

.globl store_test
.type store_test,@function
store_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    mr 11,4
    li 3,1
    xxlxor 32,32,32
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    std 3,0(11)
    std 3,0(11)
    std 3,0(11)
    std 3,0(11)
    std 3,0(11)
    std 3,0(11)
    std 3,0(11)
    std 3,0(11)
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  std 3,0(11)
    bdnz 4b
5:  li 3,0
    blr
.size store_test,.-store_test

.globl vector_store_test
.type vector_store_test,@function
vector_store_test:
    cmpdi 3,0
    beqlr
    mr 12,3
    mr 11,4
    li 3,1
    xxlxor 32,32,32
    srdi 0,12,5
    cmpdi 0,0
    beq 3f
    mtctr 0
    cmpw 0,0 /* CR0 EQ: not-taken branch test */
    .p2align 5
1:
    .rept 4
    stxv 32,0(11)
    stxv 32,0(11)
    stxv 32,0(11)
    stxv 32,0(11)
    stxv 32,0(11)
    stxv 32,0(11)
    stxv 32,0(11)
    stxv 32,0(11)
    .endr
    bdnz 1b
3:  andi. 12,12,31
    beq 5f
    mtctr 12
    cmpw 0,0
4:  stxv 32,0(11)
    bdnz 4b
5:  li 3,0
    blr
.size vector_store_test,.-vector_store_test
.section .note.GNU-stack,"",@progbits
