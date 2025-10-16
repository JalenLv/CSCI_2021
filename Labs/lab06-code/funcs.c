// Returns i if i > j
// Otherwise returns j
// Examples: max(4, 8) returns 8
//           max(16, 15) returns 16
int my_max(int i, int j) {
    if (i > j) {
        return i;
    }
    return j;
}

// Returns base^exp
// Examples: pow(2, 3) returns 8
//           pow(5, 4) returns 625
int my_pow(int base, int exp) {
    int result = base;
    for (int i = 2; i <= exp; i++) {
        result *= base;
    }
    return result;
}
