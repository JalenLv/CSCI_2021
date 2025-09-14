// Construct memory diagrams for the blocks below
#include <stdio.h>

int main() {
    // BLOCK A
    int a = 5;
    int b = 7;
    double x = 4.5;
    int *ip = &a;
    ip = &b;
    int c = *ip;
    *ip = 19;
    printf("addr of a=%d\n", 1048);
    printf("addr of b=%ld\n", 1048 - (long) &b + (long) &a);
    printf("addr of x=%ld\n", 1048 - (long) &x + (long) &a);
    printf("addr of ip=%ld\n", 1048 - (long) &ip + (long) &a);
    printf("addr of c=%ld\n", 1048 - (long) &c + (long) &a);
    // DRAW MEMORY HERE

    // BLOCK B
    int arr[4] = {12, 14, 16, 18};
    int *arp = arr;
    int brr = 11;
    arr[1] = 23;
    arp[3] = 29;
    arp = &arr[2];
    *arp = brr;
    // DRAW MEMORY HERE

    // BLOCK C
    char str[8] = "hello";
    str[5] = 'w';
    char *cp = str + 6;
    *cp = '\0';
    str[0] = 'y';
    // DRAW MEMORY HERE
}
