int ipow(int base, int exp) {
    int pow = 1;
    int i = 0;
    while (i < exp) {
        pow = pow * base;
        i++;
    }
    return pow;
}
