/*
 * age computation program
 * compile: gcc age.c
 * run on linux: ./a.out
 */

#include <stdio.h>

int main(void) {
    int age_years = 0;
    printf("Enter your age in years:\n");
    scanf("%d", &age_years);
    int age_minutes = age_years * 365 * 24 * 60;
    printf("You are %d minutes old.\n", age_minutes);

    if (age_years < 18) {
        printf("You're too young to do anything fun.\n");
    } else if (age_years >= 18 && age_years < 21) {
        printf("You can vote but cannot (legally) imbibe alcohol.\n");
    } else if (age_years >= 21) {
        printf("You may imbibe alcohol but cannot be president\n");
    } else {    // does this print?
        printf("You can vote, drink, and be president.\n");
    }

    return 0;
}
