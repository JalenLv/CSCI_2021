#include <stdio.h>

// Returns i if i > j
// Otherwise returns j
// Examples: my_max(4, 8) returns 8
//           my_max(16, 15) returns 16
int my_max(int i, int j);

// Returns base^exp
// Examples: my_pow(2, 3) returns 8
//           my_pow(5, 4) returns 625
int my_pow(int base, int exp);

int main() {
    int a = 42;
    int b = 23;
    printf("Max of %d and %d is: %d\n", a, b, my_max(a, b));

    int x = 108;
    int y = 219;
    printf("Max of %d and %d is: %d\n", x, y, my_max(x, y));

    int base1 = 4;
    int exp1 = 3;
    printf("%d^%d = %d\n", base1, exp1, my_pow(base1, exp1));

    int base2 = 7;
    int exp2 = 9;
    printf("%d^%d = %d\n", base2, exp2, my_pow(base2, exp2));

    return 0;
}
