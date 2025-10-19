#include <stdio.h>

int verify(char *buf);    // defined in provided binary

int main() {
    printf("Complete this sentence by C creator Dennis Ritchie:\n");
    printf("C has the power of assembly language and the convenience of ...\n");
    char buf[1024];
    fgets(buf, 1024, stdin);

    int ret = verify(buf);
    if (ret == 0) {
        printf("Correct!\n");
    } else {
        printf("Have a nice tall glass of ... NOPE.\n");
    }
    return 0;
}
