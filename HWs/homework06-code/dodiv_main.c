#include <stdio.h>

// equivalent to
// *quot = numer / denom;
// *rem  = numer % denom;
int dodiv(int numer, int denom, int *quot, int *rem);

int main() {
    int numer = 42;
    int denom = 11;
    int quot;
    int rem;

    int result = dodiv(numer, denom, &quot, &rem);
    // Should be
    //   42 / 11 = 3 rem 9
    // if assembly is correct
    printf("%d / %d = %d rem %d\n", numer, denom, quot, rem);

    return 0;
}
