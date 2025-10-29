#include <stdlib.h>

#include "linked_list.h"

int list_find_student(list_t *list, int value) {
    node_t *curr = list->head;
    int idx = 0;
    while (curr != NULL) {
        if (curr->value == value)
            return idx;
        curr = curr->next;
        idx++;
    }
    return -1;
}
