#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define MAX_CMD_LEN 128

// A helper function to spell check a specific file
// 'file_name': Name of the file to spell check
// 'dict': A dictionary containing correct words
int spell_check_file(const char *file_name, const dictionary_t *dict) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) return 1;

    char word[MAX_WORD_LEN];
    int word_len = 0;
    char c;
    while ((c = fgetc(f))) {
        if (c >= 'a' && c <= 'z') {
            word[word_len++] = c;
        } else if (word_len != 0) {
            word[word_len] = '\0';
            word_len = 0;
            if (!dict_find(dict, word)) printf("[X]");
        }
        if (c == EOF) break;
        putchar(c);
    }

    printf("\n");
    fclose(f);
    return 0;
}

/*
 * This is in general *very* similar to the list_main file seen in lab
 */
int main(int argc, char **argv) {
    dictionary_t *dict = NULL;
    char cmd[MAX_CMD_LEN];

    switch (argc) {
    case 1:
        dict = create_dictionary();
        break;
    case 2:
        if ((dict = read_dict_from_text_file(argv[1])) == NULL) {
            printf("Failed to read dictionary from text file\n");
            return 1;
        } else {
            printf("Dictionary successfully read from text file\n");
        }
        break;
    case 3:
        if ((dict = read_dict_from_text_file(argv[1])) == NULL) {
            printf("Failed to read dictionary from text file\n");
            return 1;
        } else {
            printf("Dictionary successfully read from text file\n");
            if (spell_check_file(argv[2], dict) != 0) {
                printf("Spell check failed\n");
                dict_free(dict);
                return 1;
            }
            dict_free(dict);
            return 0;
        }
        break;
    default:
        break;
    }

    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");

    while (1) {
        printf("spell_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

// Helper macros to read in an argument
#define MAX_ARG_LEN 128
#define GET_ARG()   \
    char arg[MAX_ARG_LEN];  \
    if (scanf("%s", arg) == EOF) {  \
        printf("Error reading argument\n"); \
        continue;   \
    }

        if (strcmp("add", cmd) == 0) {
            GET_ARG();
            dict_insert(dict, arg);
        }

        else if (strcmp("lookup", cmd) == 0) {
            GET_ARG();
            if (dict_find(dict, arg)) {
                printf("'%s' present in dictionary\n", arg);
            } else {
                printf("'%s' not found\n", arg);
            }
        }

        else if (strcmp("print", cmd) == 0) {
            dict_print(dict);
        }

        else if (strcmp("load", cmd) == 0) {
            GET_ARG();

            dictionary_t *new_dict;
            if ((new_dict = read_dict_from_text_file(arg)) == NULL) {
                printf("Failed to read dictionary from text file\n");
                continue;
            }

            if (dict != NULL) dict_free(dict);
            dict = new_dict;
            printf("Dictionary successfully read from text file\n");
        }

        else if (strcmp("save", cmd) == 0) {
            GET_ARG();
            if (write_dict_to_text_file(dict, arg) != 0) {
                printf("Failed to write dictionary to text file\n");
            } else {
                printf("Dictionary successfully written to text file\n");
            }
        }

        else if (strcmp("check", cmd) == 0) {
            GET_ARG();
            if (spell_check_file(arg, dict) != 0) {
                printf("Spell check failed\n");
            }
        }

        else {
            printf("Unknown command %s\n", cmd);
        }
    }

    dict_free(dict);
    return 0;
}
