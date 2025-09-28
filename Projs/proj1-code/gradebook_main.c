#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gradebook.h"

#define MAX_CMD_LEN 128

/**
 * Helper function to get the extension of the file name.
 */
char *get_ext(char *file_name) {
    char *ext = strdup(file_name);

    char *end = ext + strlen(ext);
    while (end != ext && *end != '.')
        --end;

    if (end == ext) {
        free(ext);
        return NULL;
    }

    char *old_ext = ext;
    ext = strdup(end + 1);
    free(old_ext);
    return ext;
}

// This is in general similar to the list_main file seen in lab
// One big difference is the notion of switching between gradebooks in one
// run of the program.
// You have to create or load a gradebook from a file before you can do things
// like add, lookup, or write.
// Also, the user must explicitly clear the current gradebook before
// they can create or load in a new one.
int main(int argc, char **argv) {
    gradebook_t *book = NULL;

    if (argc == 2) {
        char *file_name = argv[1];
        char *ext = get_ext(file_name);
        if (ext == NULL) {
            return -1;
        } else if (strcmp("txt", ext) == 0) {
            book = read_gradebook_from_text(file_name);
            if (book != NULL)
                printf("Gradebook loaded from text file\n");
            else
                printf("Failed to read gradebook from text file\n");
        } else if (strcmp("bin", ext) == 0) {
            book = read_gradebook_from_binary(file_name);
            if (book != NULL)
                printf("Gradebook loaded from binary file\n");
            else
                printf("Failed to read gradebook from binary file\n");
        } else {
            printf("Error: Unknown gradebook file extension\n");
        }
        if (ext != NULL)
            free(ext);
    } else if (argc > 2) {
        return -1;
    }

    printf("CSCI 2021 Gradebook System\n");
    printf("Commands:\n");
    printf("  create <name>:          creates a new class with specified name\n");
    printf("  class:                  shows the name of the class\n");
    printf("  add <name> <score>:     adds a new score\n");
    printf("  lookup <name>:          searches for a score by student name\n");
    printf("  clear:                  resets current gradebook\n");
    printf("  print:                  shows all scores, sorted by student name\n");
    printf("  write_text:             saves all scores to text file\n");
    printf("  read_text <file_name>:  loads scores from text file\n");
    printf("  write_bin:              saves all scores to binary file\n");
    printf("  read_bin <file_name>:   loads scores from binary file\n");
    printf("  exit:                   exits the program\n");

    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("gradebook> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

        else if (strcmp("create", cmd) == 0) {
            scanf("%s", cmd);

            if (book != NULL) {
                printf("Error: You already have a gradebook.\n");
                printf("You can remove it with the \'clear\' command\n");
            } else {
                book = create_gradebook(cmd);
                if (book == NULL) {
                    printf("Gradebook creation failed\n");
                }
            }
        }

        else if (strcmp("class", cmd) == 0) {
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
                printf("%s\n", book->class_name);
            }
        }

        else if (strcmp("add", cmd) == 0) {
            char name[MAX_NAME_LEN];
            int score;
            scanf("%s %d", name, &score);

            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else if (score <= 0) {
                printf("Error: You must enter a score in the valid range (0 <= score)\n");
            } else if (add_score(book, name, score)) {
                printf("Error: failed to add score to gradebook\n");
            }
        }

        else if (strcmp("lookup", cmd) == 0) {
            scanf("%s", cmd);
            int score;

            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else if ((score = find_score(book, cmd)) == -1) {
                printf("No score for '%s' found\n", cmd);
            } else {
                printf("%s: %d\n", cmd, score);
            }
        }

        else if (strcmp("clear", cmd) == 0) {
            if (book == NULL) {
                printf("Error: No gradebook to clear\n");
            } else {
                free_gradebook(book);
                book = NULL;
            }
        }

        else if (strcmp("print", cmd) == 0) {
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
                print_gradebook(book);
            }
        }

        else if (strcmp("write_text", cmd) == 0) {
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else if (write_gradebook_to_text(book)) {
                printf("Failed to write gradebook to text file\n");
            } else {
                printf("Gradebook successfully written to %s.txt\n", book->class_name);
            }
        }

        else if (strcmp("read_text", cmd) == 0) {
            scanf("%s", cmd);

            if (book != NULL) {
                printf("Error: You must clear current gradebook first\n");
            } else if ((book = read_gradebook_from_text(cmd)) == NULL) {
                printf("Failed to read gradebook from text file\n");
            } else {
                printf("Gradebook loaded from text file\n");
            }
        }

        else if (strcmp("write_bin", cmd) == 0) {
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else if (write_gradebook_to_binary(book)) {
                printf("Failed to write gradebook to binary file\n");
            } else {
                printf("Gradebook successfully written to %s.bin\n", book->class_name);
            }
        }

        else if (strcmp("read_bin", cmd) == 0) {
            scanf("%s", cmd);

            if (book != NULL) {
                printf("Error: You must clear current gradebook first\n");
            } else if ((book = read_gradebook_from_binary(cmd)) == NULL) {
                printf("Failed to read gradebook from binary file\n");
            } else {
                printf("Gradebook loaded from binary file\n");
            }
        }

        else {
            printf("Unknown command %s\n", cmd);
        }
    }

    free_gradebook(book);
    return 0;
}
