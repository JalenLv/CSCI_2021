#if 0

STEP 1: Read the following instructions carefully.

You will provide your solution to this part of the project by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function.

  Each expression in your code can use ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xFFFFFFFF.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  You are not restricted to one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operators, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int. This implies that you
     cannot use arrays, structs, or unions.

  You may assume that your machine:
  1. Uses twos complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.

FLOATING POINT CODING RULES

  For the problems that require you to implement floating-point operations,
  the coding rules are less strict.  You are allowed to use looping and
  conditional control.  You are allowed to use both int and unsigned variables.
  You can use arbitrary integer and unsigned constants. You can use any arithmetic,
  logical, or comparison operations on int or unsigned data.

  You are expressly forbidden to:
    1. Define or use any macros.
    2. Define any additional functions in this file.
    3. Call any functions.
    4. Use any form of casting.
    5. Use any data type other than int or unsigned.  This means that you
       cannot use arrays, structs, or unions.
    6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the ndlc program (described in the spec) to check the legality of
     your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function. The max operator count is checked by ndlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the spec and in this file, consider
     this file the authoritative source.

STEP 2: Modify the following functions according the coding rules.

IMPORTANT. TO AVOID GRADING SURPRISES:
  1. Use the ndlc tool to check that your solutions conform to the coding rules.

YOU WILL RECEIVE NO CREDIT IF YOUR CODE DOES NOT PASS THIS CHECK.

  2. Run the provided tests to check that your solutions achieve the
     desired results.

#endif

/*
 * bitXor - Compute x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    return (~(x & y)) & (~(~x & ~y));
}

/*
 * bitAnd - Compute x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
    return ~(~x | ~y);
}

/*
 * allOddBits - Return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    int mask = 0xAA + (0xAA << 8);
    mask = mask + (mask << 16);
    // `mask` is used to retain only odd-numbered bits of `x`,
    // and meanwhile serves as the comparison target.
    return !((x & mask) ^ mask);
}

/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
    // Get exponent and fraction parts
    // The sign bit is not needed for NaN/zero check
    int exp_f = (uf >> 23) & 0xFF;
    int frac_f = uf & 0x7FFFFF;
    int exp_g = (ug >> 23) & 0xFF;
    int frac_g = ug & 0x7FFFFF;

    // Check if either uf or ug is NaN
    if ((exp_f == 0xFF && frac_f != 0) || (exp_g == 0xFF && frac_g != 0)) {
        return 0;
    }

    // Check for +0 and -0
    if ((uf << 1) == 0 && (ug << 1) == 0) {
        return 1;
    }

    return uf == ug;
}

/*
 * anyEvenBit - Return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
    int mask = 0x55 + (0x55 << 8);
    mask = mask + (mask << 16);
    // Similar to allOddBits but checks "any" instead of "all"
    return !!(x & mask);
}

/*
 * isPositive - return 1 if x > 0, return 0 otherwise
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int isPositive(int x) {
    // Check sign bit and whether x is zero
    return (!(x >> 31)) & (!!x);
}

/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (least significant) to 3 (most significant)
 *   Examples: replaceByte(0x12345678, 1, 0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    int shift = n << 3;
    return (x & ~(0xFF << shift)) | (c << shift);
}

/*
 * isLess - if x < y  then return 1, else return 0
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
    int sign_x = (x >> 31) & 1;
    int sign_y = (y >> 31) & 1;
    int diff_sign = sign_x ^ sign_y;

    // If signs are different, x < y if x is negative
    // If signs are the same, x < y if (y - x) is positive
    int diff_sign_result = diff_sign & sign_x;
    int same_sign_result = (!diff_sign) & ((x + (~y + 1)) >> 31) & 1;

    return diff_sign_result | same_sign_result;
}

/*
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3
 */
int rotateLeft(int x, int n) {
    int left = x << n;
    int right = (x >> (32 + (~n + 1))) & ((1 << n) + (~1 + 1));
    return left | right;
}

/*
 * bitMask - Generate a mask consisting of all 1's
 *   between lowbit and highbit positions
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
    int high_mask = ~((~0) << highbit << 1);
    int low_mask = (~0) << lowbit;
    return high_mask & low_mask;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
    int exp_f = (uf >> 23) & 0xFF;
    int frac_f = uf & 0x7FFFFF;

    // pm zero
    if ((uf << 1) == 0) {
        return uf;
    }

    // pm inf and NaN
    if (exp_f == 0xFF) {
        return uf;
    }

    // Denormalized
    if (exp_f == 0 && frac_f != 0) {
        if ((frac_f >> 22) & 1) {
            // Will become normalized
            return (uf & 0x80000000) | (1 << 23) | ((frac_f << 1) & 0x7FFFFF);
        } else {
            // Still denormalized
            return (uf & 0x80000000) | (frac_f << 1);
        }
    }

    // Normalized
    if (exp_f + 1 == 0xFF) {
        // Will become pm inf
        return (uf & 0x80000000) | (0xFF << 23);
    } else {
        // Still normalized
        return (uf & 0x80000000) | ((exp_f + 1) << 23) | frac_f;
    }
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
    return (!!x) & (!(x >> 31)) & !(x & (x + (~1 + 1)));
}
