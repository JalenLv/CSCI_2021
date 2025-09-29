// Demonstrate a conversion routine
#include <stdbool.h>
#include <stdio.h>

int convert(char *str, int *num) {
    bool negate = false;    // whether to negate
    int total = 0;          // running total
    for (int pos = 0; str[pos] != '\0'; pos++) {
        int value = 0;
        char digit = str[pos];

        switch (digit) {
            case '0':
                value = 0;
                break;
            case '1':
                value = 1;
                break;
            case '2':
                value = 2;
                break;
            case '3':
                value = 3;
                break;
            case '4':
                value = 4;
                break;
            case '5':
                value = 5;
                break;
            case '6':
                value = 6;
                break;
            case '7':
                value = 7;
                break;
            case '8':
                value = 8;
                break;
            case '9':
                value = 9;
                break;
            case '-':
                if (pos == 0) {
                    // Negative sign at beginning
                    negate = true;
                    break;
                }    // else fall through to default
            default:
                printf("'%c' at pos %d not valid for integers\n", digit, pos);
                return -1;
        }

        total = total * 10 + value;    // update total
    }

    if (negate) {          // check for negation found
        total = -total;    // do negation
    }
    *num = total;    // set num to total
    return 0;        // return success
}

// main invokes convert() on various data
int main() {
    char *str;
    int num;
    int err;

    str = "124";
    num = 0;
    err = convert(str, &num);    // run conversion
    printf("convert(\"%s\", &num): num = %d, err = %d\n", str, num, err);

    str = "59217";
    num = 0;
    err = convert(str, &num);    // run conversion
    printf("convert(\"%s\", &num): num = %d, err = %d\n", str, num, err);

    str = "-212";
    num = 0;
    err = convert(str, &num);    // run conversion
    printf("convert(\"%s\", &num): num = %d, err = %d\n", str, num, err);

    str = "51a3";
    num = 0;
    err = convert(str, &num);    // run conversion
    printf("convert(\"%s\",&num): num = %d, err = %d\n", str, num, err);

    str = "513-7";
    num = 0;
    err = convert(str, &num);    // run conversion
    printf("convert(\"%s\",&num): num = %d, err = %d\n", str, num, err);

    return 0;
}
