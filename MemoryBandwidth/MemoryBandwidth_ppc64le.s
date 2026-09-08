.abiversion 2
.text
/* r3=array, r4=float elements, r5=byte-equivalent passes, r6=start element.
   Each read/write pass transfers 4*elements bytes; RMW passes count twice.
   Copy reads first half and writes second half (same total byte traffic).
   All loops use 128-byte chunks and caller-volatile VSX32..40 (v0..v8).
   No prefetch or flush: measure ordinary cacheable streaming throughput. */

.globl asm_read
.type asm_read,@function
asm_read:
    cmpdi 5,0
    beq 9f
    sldi 4,4,2
    sldi 6,6,2
    cmpdi 4,128
    blt 9f
    mr 7,6
1:  add 9,3,7
    lxv 32,0(9)
    lxv 33,16(9)
    lxv 34,32(9)
    lxv 35,48(9)
    lxv 36,64(9)
    lxv 37,80(9)
    lxv 38,96(9)
    lxv 39,112(9)
    addi 7,7,128
    cmpld 7,4
    blt 2f
    li 7,0
2:  cmpld 7,6
    bne 1b
    addic. 5,5,-1
    bne 1b
9:  lfs 1,0(3)
    blr
.size asm_read,.-asm_read

.globl asm_write
.type asm_write,@function
asm_write:
    cmpdi 5,0
    beq 9f
    sldi 4,4,2
    sldi 6,6,2
    cmpdi 4,128
    blt 9f
    mr 7,6
    vspltisw 0,1
    xvcvsxwsp 32,32
1:  add 9,3,7
    stxv 32,0(9)
    stxv 32,16(9)
    stxv 32,32(9)
    stxv 32,48(9)
    stxv 32,64(9)
    stxv 32,80(9)
    stxv 32,96(9)
    stxv 32,112(9)
    addi 7,7,128
    cmpld 7,4
    blt 2f
    li 7,0
2:  cmpld 7,6
    bne 1b
    addic. 5,5,-1
    bne 1b
9:  lfs 1,0(3)
    blr
.size asm_write,.-asm_write

.globl asm_copy
.type asm_copy,@function
asm_copy:
    cmpdi 5,0
    beq 9f
    sldi 4,4,2
    sldi 6,6,2
    srdi 4,4,1
    add 8,3,4
    cmpdi 4,128
    blt 9f
    mr 7,6
1:  add 9,3,7
    add 10,8,7
    lxv 32,0(9)
    lxv 33,16(9)
    lxv 34,32(9)
    lxv 35,48(9)
    lxv 36,64(9)
    lxv 37,80(9)
    lxv 38,96(9)
    lxv 39,112(9)
    stxv 32,0(10)
    stxv 33,16(10)
    stxv 34,32(10)
    stxv 35,48(10)
    stxv 36,64(10)
    stxv 37,80(10)
    stxv 38,96(10)
    stxv 39,112(10)
    addi 7,7,128
    cmpld 7,4
    blt 2f
    li 7,0
2:  cmpld 7,6
    bne 1b
    addic. 5,5,-1
    bne 1b
9:  lfs 1,0(3)
    blr
.size asm_copy,.-asm_copy

.globl asm_add
.type asm_add,@function
asm_add:
    cmpdi 5,0
    beq 9f
    sldi 4,4,2
    sldi 6,6,2
    srdi 5,5,1
    cmpdi 5,0
    beq 9f
    cmpdi 4,128
    blt 9f
    mr 7,6
    vspltisw 8,1
    xvcvsxwsp 40,40
1:  add 9,3,7
    lxv 32,0(9)
    lxv 33,16(9)
    lxv 34,32(9)
    lxv 35,48(9)
    lxv 36,64(9)
    lxv 37,80(9)
    lxv 38,96(9)
    lxv 39,112(9)
    xvaddsp 32,32,40
    xvaddsp 33,33,40
    xvaddsp 34,34,40
    xvaddsp 35,35,40
    xvaddsp 36,36,40
    xvaddsp 37,37,40
    xvaddsp 38,38,40
    xvaddsp 39,39,40
    stxv 32,0(9)
    stxv 33,16(9)
    stxv 34,32(9)
    stxv 35,48(9)
    stxv 36,64(9)
    stxv 37,80(9)
    stxv 38,96(9)
    stxv 39,112(9)
    addi 7,7,128
    cmpld 7,4
    blt 2f
    li 7,0
2:  cmpld 7,6
    bne 1b
    addic. 5,5,-1
    bne 1b
9:  lfs 1,0(3)
    blr
.size asm_add,.-asm_add

.globl asm_cflip
.type asm_cflip,@function
asm_cflip:
    cmpdi 5,0
    beq 9f
    sldi 4,4,2
    sldi 6,6,2
    srdi 5,5,1
    cmpdi 5,0
    beq 9f
    cmpdi 4,128
    blt 9f
    mr 7,6
1:  add 9,3,7
    lxv 32,0(9)
    lxv 33,16(9)
    lxv 34,32(9)
    lxv 35,48(9)
    lxv 36,64(9)
    lxv 37,80(9)
    lxv 38,96(9)
    lxv 39,112(9)
    stxv 39,0(9)
    stxv 38,16(9)
    stxv 37,32(9)
    stxv 36,48(9)
    stxv 35,64(9)
    stxv 34,80(9)
    stxv 33,96(9)
    stxv 32,112(9)
    addi 7,7,128
    cmpld 7,4
    blt 2f
    li 7,0
2:  cmpld 7,6
    bne 1b
    addic. 5,5,-1
    bne 1b
9:  lfs 1,0(3)
    blr
.size asm_cflip,.-asm_cflip
.section .note.GNU-stack,"",@progbits
