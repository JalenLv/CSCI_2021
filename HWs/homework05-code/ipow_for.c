int ipow(int base, int exp) {
    int pow = 1;
    for (int i = 0; i < exp; i++) {
        pow = pow * base;
    }
    return pow;
}
