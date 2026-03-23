# Read the following instructions carefully
# You will provide your solution to this part of the project by
# editing the collection of functions in this source file.
#
# Some rules from Project 2 are still in effect for your assembly code here:
#  1. No global variables are allowed
#  2. You may not define or call any additional functions in this file
#  3. You may not use any floating-point assembly instructions
# You may assume that your machine:
#  1. Uses two's complement, 32-bit representations of integers.

# isZero - returns 1 if x == 0, and 0 otherwise
#   Argument 1: x
#   Examples: isZero(5) = 0, isZero(0) = 1
#   Rating: 1
.global isZero
isZero:
    testl %edi, %edi
    sete %al
    movzbl %al, %eax
    ret

# bitNor - ~(x|y)
#   Argument 1: x
#   Argument 2: y
#   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
#   Rating: 1
.global bitNor
bitNor:
    movl %edi, %eax
    orl %esi, %eax
    notl %eax
    ret

# distinctNegation - returns 1 if x != -x.
#     and 0 otherwise
#   Argument 1: x
#   Rating: 2
.global distinctNegation
distinctNegation:
    movl %edi, %eax
    negl %eax
    cmpl %eax, %edi
    setne %al
    movzbl %al, %eax
    ret

# dividePower2 - Compute x/(2^n), for 0 <= n <= 30
#  Round toward zero
#   Argument 1: x
#   Argument 2: n
#   Examples: dividePower2(15,1) = 7, dividePower2(-33,4) = -2
#   Rating: 2
.global dividePower2
dividePower2:
    movl %esi, %ecx
    movl $1, %edx
    shll %cl, %edx
    decl %edx
    movl %edi, %eax
    sarl $31, %eax
    andl %edx, %eax
    addl %edi, %eax
    sarl %cl, %eax
    ret

# getByte - Extract byte n from word x
#   Argument 1: x
#   Argument 2: n
#   Bytes numbered from 0 (least significant) to 3 (most significant)
#   Examples: getByte(0x12345678,1) = 0x56
#   Rating: 2
.global getByte
getByte:
    movl %esi, %ecx
    shll $3, %ecx
    movl %edi, %eax
    shrl %cl, %eax
    andl $0xFF, %eax
    ret

# isPositive - return 1 if x > 0, return 0 otherwise
#   Argument 1: x
#   Example: isPositive(-1) = 0.
#   Rating: 2
.global isPositive
isPositive:
    testl %edi, %edi
    setg %al
    movzbl %al, %eax
    ret

# floatNegate - Return bit-level equivalent of expression -f for
#   floating point argument f.
#   Both the argument and result are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representations of
#   single-precision floating point values.
#   When argument is NaN, return argument.
#   Argument 1: f
#   Rating: 2
.global floatNegate
floatNegate:
    movl %edi, %eax
    movl %eax, %edx
    andl $0x7FFFFFFF, %edx
    cmpl $0x7F800000, %edx
    ja .LfloatNegate_ret
    xorl $0x80000000, %eax
.LfloatNegate_ret:
    ret

# isLessOrEqual - if x <= y  then return 1, else return 0
#   Argument 1: x
#   Argument 2: y
#   Example: isLessOrEqual(4,5) = 1.
#   Rating: 3
.global isLessOrEqual
isLessOrEqual:
    cmpl %esi, %edi
    setle %al
    movzbl %al, %eax
    ret

# bitMask - Generate a mask consisting of all 1's between
#   lowbit and highbit
#   Argument 1: highbit
#   Argument 2: lowbit
#   Examples: bitMask(5,3) = 0x38
#   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
#   If lowbit > highbit, then mask should be all 0's
#   Rating: 3
.global bitMask
bitMask:
    movl $-1, %eax
    movl %esi, %ecx
    shll %cl, %eax

    movl $1, %edx
    movl %edi, %ecx
    shll %cl, %edx
    movl $1, %ecx
    shll %cl, %edx
    decl %edx

    andl %edx, %eax
    ret

# addOK - Determine if can compute x+y without overflow
#   Argument 1: x
#   Argument 2: y
#   Example: addOK(0x80000000,0x80000000) = 0,
#            addOK(0x80000000,0x70000000) = 1,
#   Rating: 3
.global addOK
addOK:
    addl %edi, %esi
    setno %al
    movzbl %al, %eax
    ret

