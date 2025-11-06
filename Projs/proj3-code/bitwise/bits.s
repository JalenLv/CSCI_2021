# Read the following instructions carefully
#
# You will provide your solution to this part of the project by
# editing the collection of functions in this source file.
#
# Some rules from the C bitwise puzzles are still in effect for your assembly
# code here:
#  1. No global variables are allowed
#  2. You may not define or call any additional functions in this file
#  3. You may not use any floating-point assembly instructions
#
# You may assume that your machine:
#  1. Uses two's complement, 32-bit representations of integers.
#  2. Has unpredictable behavior when shifting if the shift amount
#     is less than 0 or greater than 31.

# TO AVOID GRADING SURPRISES:
#   Pay attention to the results of the call_cc script, which is run
#   as part of the provided tests.
#
#   This makes sure you have adhered to the x86-64 calling convention
#   in your assembly code. If it reports any errors, make sure to fix
#   them before you submit your code.
#
# YOU WILL RECEIVE NO CREDIT IF YOUR CODE DOES NOT PASS THIS CHECK

# bitXor - Compute x^y
#   Example: bitXor(4, 5) = 1
#   Rating: 1
.global bitXor
bitXor:
    movl %edi, %eax
    xorl %esi, %eax
    ret

# bitAnd - Compute x&y
#   Example: bitAnd(6, 5) = 4
#   Rating: 1
.global bitAnd
bitAnd:
    movl %edi, %eax
    andl %esi, %eax
    ret

# allOddBits - Return 1 if all odd-numbered bits in word set to 1
#   where bits are numbered from 0 (least significant) to 31 (most significant)
#   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
#   Rating: 2
.global allOddBits
allOddBits:
    notl %edi
    testl $0xAAAAAAAA, %edi
    setz %al
    movzbl %al, %eax
    ret

# floatIsEqual - Compute f == g for floating point arguments f and g.
#   Both the arguments are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representations of
#   single-precision floating point values.
#   If either argument is NaN, return 0.
#   +0 and -0 are considered equal.
#   Rating: 2
.global floatIsEqual
floatIsEqual:
    # Check for NaN
    # Extract exponents and fractions
    # %r8d = (uf >> 23) & 0xFF
    movl %edi, %r8d
    shrl $23, %r8d
    andl $0xFF, %r8d
    # %r9d = uf & 0x7FFFFF
    movl %edi, %r9d
    andl $0x7FFFFF, %r9d
    # %r10d = (ug >> 23) & 0xFF
    movl %esi, %r10d
    shrl $23, %r10d
    andl $0xFF, %r10d
    # %r11d = ug & 0x7FFFFF
    movl %esi, %r11d
    andl $0x7FFFFF, %r11d

    # Check for NaN conditions
    cmpl $0xFF, %r8d        # check exp_f == 0xFF
    jne FIE_L1              # if exp_f != 0xFF, check exp_g == 0xFF
    testl %r9d, %r9d        # if exp_f == 0xFF, check frac_f != 0
    jnz FIE_RET_ZERO_4_NAN  # if frac_f != 0, return 0
                            # if frac_f == 0, continue to check g
FIE_L1:
    cmpl $0xFF, %r10d       # check exp_g == 0xFF
    jne FIE_L2              # if exp_g != 0xFF, check pm zero
    testl %r11d, %r11d      # if exp_g == 0xFF, check frac_g != 0
    jnz FIE_RET_ZERO_4_NAN  # if frac_g != 0, return 0
                            # if frac_g == 0, continue to check pm zero
FIE_L2:
    # Check for pm zero
    # %r8d = uf << 1
    movl %edi, %r8d
    shll $1, %r8d
    # %r9d = ug << 1
    movl %esi, %r9d
    shll $1, %r9d
    # Compare shifted values
    testl %r8d, %r8d            # check uf << 1 == 0
    jnz FIE_L3                  # if uf << 1 != 0, continue to compare normally
    testl %r9d, %r9d            # if uf << 1 == 0, check ug << 1 == 0
    jz FIE_RET_ONE_4_PM_ZERO    # if ug << 1 == 0, return 1
                                # if ug << 1 != 0, continue to compare normally
FIE_L3:
    # Normal comparison
    cmpl %esi, %edi     # compare uf and ug
    sete %al
    movzbl %al, %eax
    ret
FIE_RET_ZERO_4_NAN:
    movl $0, %eax
    ret
FIE_RET_ONE_4_PM_ZERO:
    movl $1, %eax
    ret

# anyEvenBit - Return 1 if any even-numbered bit in word set to 1
#   where bits are numbered from 0 (least significant) to 31 (most significant)
#   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
#   Rating: 2
.global anyEvenBit
anyEvenBit:
    andl $0x55555555, %edi
    testl %edi, %edi
    setnz %al
    movzbl %al, %eax
    ret

# isPositive - return 1 if x > 0, return 0 otherwise
#   Example: isPositive(-1) = 0.
#   Rating: 2
.global isPositive
isPositive:
    cmpl $0, %edi
    setg %al
    movzbl %al, %eax
    ret

# replaceByte(x,n,c) - Replace byte n in x with c
#   Bytes numbered from 0 (least significant) to 3 (most significant)
#   Examples: replaceByte(0x12345678, 1, 0xab) = 0x1234ab78
#   You can assume 0 <= n <= 3 and 0 <= c <= 255
#   Rating: 3
.global replaceByte
replaceByte:
    # %edi = x, %esi = n, %edx = c
    # %ecx = shift = n << 3
    movl %esi, %ecx
    sall $3, %ecx
    # Prepare ~(0xFF << shift) & x
    movl $0xFF, %eax
    sall %cl, %eax
    notl %eax
    andl %edi, %eax
    # Prepare (c << shift)
    sall %cl, %edx
    # Combine both parts
    orl %edx, %eax
    ret

# isLess - if x < y  then return 1, else return 0
#   Example: isLess(4,5) = 1.
#   Rating: 3
.global isLess
isLess:
    cmpl %esi, %edi
    setl %al
    movzbl %al, %eax
    ret

# rotateLeft - Rotate x to the left by n
#   Can assume that 0 <= n <= 31
#   Examples: rotateLeft(0x87654321,4) = 0x76543218
#   Rating: 3
.global rotateLeft
rotateLeft:
    movl %edi, %eax
    movl %esi, %ecx
    roll %cl, %eax
    ret

# bitMask - Generate a mask consisting of all 1's
#   between lowbit and highbit positions
#   Examples: bitMask(5,3) = 0x38
#   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
#   If lowbit > highbit, then mask should be all 0's
#   Rating: 3
.global bitMask
bitMask:
    # %edi = highbit, %esi = lowbit
    # %edi = high_mask = ~((~0) << highbit << 1)
    movl %edi, %ecx
    movl $-1, %edi
    sall %cl, %edi
    sall $1, %edi
    notl %edi
    # %esi = low_mask = (~0) << lowbit
    movl %esi, %ecx
    movl $-1, %esi
    sall %cl, %esi
    # Combine
    andl %edi, %esi
    movl %esi, %eax
    ret

# floatScale2 - Return bit-level equivalent of expression 2*f for
#   floating point argument f.
#   Both the argument and result are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representation of
#   single-precision floating point values.
#   When argument is NaN, return argument
#   Rating: 4
.global floatScale2
floatScale2:
    # Extract exponent, fraction
    # %r8d = exp = (uf >> 23) & 0xFF
    movl %edi, %r8d
    shrl $23, %r8d
    andl $0xFF, %r8d
    # %r9d = frac = uf & 0x7FFFFF
    movl %edi, %r9d
    andl $0x7FFFFF, %r9d

    # Check for pm zero
    testl %r8d, %r8d            # check exp == 0
    jnz FS2_L1                  # if exp != 0, continue to check for NaN
    testl %r9d, %r9d            # if exp == 0, check frac == 0
    jnz FS2_L1                  # if frac != 0, continue to check for NaN
    movl %edi, %eax             # if pm zero, return uf
    ret
FS2_L1:
    # Check for NaN
    cmpl $0xFF, %r8d        # check exp == 0xFF
    jne FS2_L2              # if exp != 0xFF, continue to check for denormals
    movl %edi, %eax         # if exp == 0xFF, return uf
    ret
FS2_L2:
    # Check for denormals
    testl %r8d, %r8d        # check exp == 0
    jnz FS2_L3              # if exp != 0, continue to normal case
    testl %r9d, %r9d        # if exp == 0, check frac == 0
    jz FS2_L3               # if frac == 0, continue to normal case
                            # if frac != 0, denormal case
    movl $22, %ecx          # check (frac >> 22) & 1 == 0
    movl %r9d, %eax
    shrl %cl, %eax
    andl $1, %eax
    jz FS2_L4               # if (frac >> 22) & 1 == 0, the result will still be denormal
                            # if (frac >> 22) & 1 == 1, the result will become normal
    movl %edi, %eax         # result = (uf & 0x80000000) | (1 << 23) | ((frac << 1) & 0x7FFFFF)
    andl $0x80000000, %eax
    shll $1, %r9d
    andl $0x7FFFFF, %r9d
    orl $0x800000, %r9d
    orl %r9d, %eax
    ret
FS2_L4:
    shll $1, %r9d           # result = (uf & 0x80000000) | (frac << 1)
    movl %edi, %eax
    andl $0x80000000, %eax
    orl %r9d, %eax
    ret
FS2_L3:
    # Normal case
    incl %r8d               # result = (uf & 0x80000000) | ((exp + 1) << 23) | frac
    shll $23, %r8d
    movl %edi, %eax
    andl $0x80000000, %eax
    orl %r8d, %eax
    orl %r9d, %eax
    ret

# isPower2 - returns 1 if x is a power of 2, and 0 otherwise
#   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
#   Note that no negative number is a power of 2.
#   Rating: 4
.global isPower2
isPower2:
    cmpl $0, %edi
    jle IP2_RET_ZERO
    # %eax = (x - 1) & x
    movl %edi, %eax
    decl %eax
    andl %edi, %eax
    testl %eax, %eax
    jnz IP2_RET_ZERO
    movl $1, %eax
    ret
IP2_RET_ZERO:
    movl $0, %eax
    ret
