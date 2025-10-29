// SPDX-License-Identifier: GPL-3.0-or-later
// Based on Bryant and O'Halloran's original "datalab" assignment

#include <math.h>

int test_bitXor(int x, int y) {
    return x ^ y;
}

int test_bitAnd(int x, int y) {
    return x & y;
}

int test_allOddBits(int x) {
    for (int i = 1; i < 32; i += 2) {
        if ((x & (1 << i)) == 0) {
            return 0;
        }
    }
    return 1;
}

int test_floatIsEqual(unsigned uf, unsigned ug) {
    union unsigned_float {
        unsigned u;
        float f;
    };

    union unsigned_float x;
    x.u = uf;
    union unsigned_float y;
    y.u = ug;
    return x.f == y.f;
}

int test_anyEvenBit(int x) {
    for (int i = 0; i < 32; i += 2) {
        if (x & (1 << i)) {
            return 1;
        }
    }
    return 0;
}

int test_isPositive(int x) {
    return x > 0;
}

int test_replaceByte(int x, int n, int c) {
    switch (n) {
        case 0:
            x = (x & 0xFFFFFF00) | c;
            break;
        case 1:
            x = (x & 0xFFFF00FF) | (c << 8);
            break;
        case 2:
            x = (x & 0xFF00FFFF) | (c << 16);
            break;
        default:
            x = (x & 0x00FFFFFF) | (c << 24);
            break;
    }
    return x;
}

int test_isLess(int x, int y) {
    return x < y;
}

int test_rotateLeft(int x, int n) {
    unsigned u = (unsigned) x;
    for (int i = 0; i < n; i++) {
        unsigned msb = u >> 31;
        unsigned rest = u << 1;
        u = rest | msb;
    }
    return (int) u;
}

int test_bitMask(int highbit, int lowbit) {
    int result = 0;
    for (int i = lowbit; i <= highbit; i++) {
        result |= 1 << i;
    }
    return result;
}

unsigned test_floatScale2(unsigned uf) {
    union unsigned_float {
        unsigned u;
        float f;
    };

    union unsigned_float x;
    x.u = uf;
    x.f *= 2;
    return x.u;
}

int test_isPower2(int x) {
    for (int i = 0; i < 31; i++) {
        if (x == 1 << i)
            return 1;
    }
    return 0;
}

