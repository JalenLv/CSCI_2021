# dodiv_badtype.s: provides a division function which computes
# both quotient and remainder.
#
# This version has a memory problem which leads to incorrect results
#
# int dodiv(int numer, int denom, int *quot, int *rem);
# args:         1:edi      2:esi      3:rdx     4:rcx

.text
.global	dodiv

dodiv:
	cmpl $0, %esi            # check denom (arg2) for 0
    je ERROR_ZERO

    movl %edi, %eax         # copy arg1 to eax for division
    movq %rdx, %r8          # copy arg3 to r8 as rdx is used in division
    cltq
    cqto                    # set up division by copying to edx
    idivl %esi              # divide by arg2, eax has quot, edx has rem
    movq %rax, (%r8)        # write quot to arg3
    movq %rdx, (%rcx)       # write rem to arg4
    movl $0, %eax           # return 0 on success
	ret

ERROR_ZERO:
        movl $1, %eax            # return 1 on failure
        ret
