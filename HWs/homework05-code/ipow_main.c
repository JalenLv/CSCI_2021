#include <stdio.h>
#include <stdlib.h>

int ipow(int base, int exp);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: %s <base> <exp>\n", argv[0]);
        return 1;
    }
    int b = atoi(argv[1]);
    int e = atoi(argv[2]);
    int pw = ipow(b, e);
    printf("%d^%d = %d\n", b, e, pw);
    return 0;
}
