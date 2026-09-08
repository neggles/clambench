.abiversion 2
.text
/* r3=array, r4=bytes, r5=stop flag, r6=throttle. Return actual traffic.
   lwz observes a relaxed atomic stop flag; it publishes no other data.
   Throttle is a counted branch delay, not a POWER priority-changing hint. */

.globl asm_read
.type asm_read,@function
asm_read:
    li 7,0
    li 8,0
    cmpdi 4,128
    blt 9f
1:  lwz 10,0(5)
    cmpwi 10,0
    bne 9f
    add 9,3,7
    lxv 32,0(9)
    lxv 33,16(9)
    lxv 34,32(9)
    lxv 35,48(9)
    lxv 36,64(9)
    lxv 37,80(9)
    lxv 38,96(9)
    lxv 39,112(9)
    addi 8,8,128
    cmpdi 6,0
    beq 3f
    mtctr 6
2:  bdnz 2b
3:  addi 7,7,128
    cmpld 7,4
    blt 1b
    li 7,0
    b 1b
9:  mr 3,8
    blr
.size asm_read,.-asm_read

.globl asm_add
.type asm_add,@function
asm_add:
    li 7,0
    li 8,0
    cmpdi 4,128
    blt 9f
    vspltisw 8,1
1:  lwz 10,0(5)
    cmpwi 10,0
    bne 9f
    add 9,3,7
    lxv 32,0(9)
    lxv 33,16(9)
    lxv 34,32(9)
    lxv 35,48(9)
    lxv 36,64(9)
    lxv 37,80(9)
    lxv 38,96(9)
    lxv 39,112(9)
    vadduwm 0,0,8
    vadduwm 1,1,8
    vadduwm 2,2,8
    vadduwm 3,3,8
    vadduwm 4,4,8
    vadduwm 5,5,8
    vadduwm 6,6,8
    vadduwm 7,7,8
    stxv 32,0(9)
    stxv 33,16(9)
    stxv 34,32(9)
    stxv 35,48(9)
    stxv 36,64(9)
    stxv 37,80(9)
    stxv 38,96(9)
    stxv 39,112(9)
    addi 8,8,256
    cmpdi 6,0
    beq 3f
    mtctr 6
2:  bdnz 2b
3:  addi 7,7,128
    cmpld 7,4
    blt 1b
    li 7,0
    b 1b
9:  mr 3,8
    blr
.size asm_add,.-asm_add
.section .note.GNU-stack,"",@progbits
