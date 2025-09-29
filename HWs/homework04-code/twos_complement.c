// Demonstrate two's complement in C
#include <stdio.h>

int main() {
    char num = 0b01011100;    // Initial signed positive number (92)
    char neg = (~num) + 1;    // negate: invert bits (~ operator) and add 1
    char pos = (~neg) + 1;    // positive: invert then add 1

    printf("%d %d %d\n", num, neg, pos);    // print number, negated version, and positive
    return 0;
}
