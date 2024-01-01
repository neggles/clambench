.text

.global asm_read

# %ecx = ptr to float array
# %edx = arr length
# [%esp + 4] = iterations, put this into %ebp
# [%esp + 8] = start offset, put this into %ebx (not used)

asm_read:
	push %ebp
	push %edi
	push %esi
	push %ebx
	mov  0x14(%esp), %ebp # put iterations into %ebp
	mov  0x18(%esp), %ebx # put start offset into %ebx

	subl $32, %edx  # last iteration: %esi == %edx. %esi > %edx = break
	xor  %esi, %esi  # reset index
	lea  (%ecx, %esi, 4), %edi # reset pointer

asm_read_pass_loop:
	mov (%edi), %eax
	mov 4(%edi), %ebx
	mov 8(%edi), %eax
	mov 12(%edi), %ebx
	mov 16(%edi), %eax
	mov 20(%edi), %ebx
	mov 24(%edi), %eax
	mov 28(%edi), %ebx
	mov 32(%edi), %eax
	mov 36(%edi), %ebx
	mov 40(%edi), %eax
	mov 44(%edi), %ebx
	mov 48(%edi), %eax
	mov 52(%edi), %ebx
	mov 56(%edi), %eax
	mov 60(%edi), %ebx
	mov 64(%edi), %eax
	mov 68(%edi), %ebx
	mov 72(%edi), %eax
	mov 76(%edi), %ebx
	mov 80(%edi), %eax
	mov 84(%edi), %ebx
	mov 88(%edi), %eax
	mov 92(%edi), %ebx
	mov 96(%edi), %eax
	mov 100(%edi), %ebx
	mov 104(%edi), %eax
	mov 108(%edi), %ebx
	mov 112(%edi), %eax
	mov 116(%edi), %ebx
	mov 120(%edi), %eax
	mov 124(%edi), %ebx

	addl $32, %esi # increment index
	addl $128, %edi # increment pointer
	cmp %esi, %edx # if %esi == %edx, break
	jge asm_read_pass_loop

	xor %esi, %esi # zero out the index
	lea (%ecx, %esi, 4), %edi # reset pointer
	sub $1, %ebp # decrement iterations
	jnz asm_read_pass_loop # if iterations != 0, loop

	// restore registers
	pop %ebx
	pop %esi
	pop %edi
	pop %ebp

	mov (%esp), %eax
	mov %eax, 20(%esp)
	add $20, %esp
	fld1
	ret $0x8
