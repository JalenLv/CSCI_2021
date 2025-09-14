// main function for a linked list manager
#include <stdio.h>
#include <string.h>

#include "list.h"

int main(int argc, char *argv[]) {
    printf("Linked List Demo\n");
    printf("Commands:\n");
    printf("  print:          shows the current contents of the list\n");
    printf("  clear:          eliminates all elements from the list\n");
    printf("  exit:           exit the program\n");
    printf("  insert thing:   inserts the given string into the list\n");
    printf("  get index:      get the item at the given index\n");
    printf("  contains thing: determine if the given thing is in the list\n");
    printf("                  (NOT YET IMPLEMENTED)\n");

    char cmd[128];
    list_t list;
    int success;
    list_init(&list);

    while (1) {
        printf("list> ");              // print prompt
        success = scanf("%s", cmd);    // read command
        if (success == EOF) {          // check for end of input
            printf("\n");              // found end of input
            break;                     // break from loop
        }

        if (strcmp("exit", cmd) == 0) {    // check for exit command
            break;                         // break from loop
        }

        else if (strcmp("insert", cmd) == 0) {    // insertion
            scanf("%s", cmd);                     // read string to insert

            success = list_insert(&list, cmd);    // call list function
            if (!success) {                       // check for success
                printf("insert failed\n");
            }
        }

        else if (strcmp("get", cmd) == 0) {    // get command
            int index;
            scanf("%d", &index);    // read an integer

            char *ith = list_get(&list, index);    // call list function
            if (ith == NULL) {                     // check for success
                printf("out of bounds\n");
            } else {
                printf("%d: %s\n", index, ith);
            }
        }

        else if (strcmp("clear", cmd) == 0) {    // clear command
            list_clear(&list);
        }

        else if (strcmp("print", cmd) == 0) {    // print command
            list_print_all(&list);
        }

        // add case for "contains" command which uses the list_contains() function
        // For example, the user searches for "Mario" by typing in "contains Mario"
        // If "Mario" is present in the list, print out "'Mario' is present"
        // Otherwise, print "not found"
        // See the test files for the exact expected formats

        else if (strcmp("contains", cmd) == 0) {
            char query[128];
            scanf("%s", query);
            int found = list_contains(&list, query);
            if (found) {
                printf("'%s' is present\n", query);
            } else {
                printf("not found\n");
            }
        }

        else {    // unknown command
            printf("unknown command %s\n", cmd);
        }
    }

    // end main while loop
    list_clear(&list);    // clean up the list
    return 0;
}
