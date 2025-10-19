# dodiv_main_asm.s: demonstrate calling function dodiv from main()
# which requires adjustment of the stack to make room for local
# variables so that they have addresses.

.data
.FMT_STRING:
	.string	"%d / %d = %d rem %d\n"

.text
.global	main

main:
	# set up call to dodiv()
	subq    $8, %rsp                  # space for locals + stack alignment to 16-byte boundary
	movl    $42, %edi                 # 1st/2nd arguments to dodiv
	movl    $11, %esi
	movq	%rsp, %rdx                # copy stack pointer to %rdx: address for quotient
	leaq	4(%rsp), %rcx             # 4 bytes off stack pointer: address for remainder
	call	dodiv                     # call dodiv

    # set up call to printf()
	leaq	.FMT_STRING(%rip), %rdi   # format string is first arg to printf
	movl	$42, %esi                 # constants for 2nd/3rd arg
	movl	$11, %edx
	movl	(%rsp), %ecx              # quotient from stack to 4th arg
	movl	4(%rsp), %r8d             # remainder from stack to 5th arg
	movl	$0, %eax                  # required for printf()
	call	printf@PLT

	# restore stack, return success
	addq    $8, %rsp
	movl	$0, %eax
	ret
