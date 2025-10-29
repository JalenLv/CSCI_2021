#include <stdlib.h>

#include "linked_list.h"

void list_init(list_t *list) {
    if (list == NULL) {
        return;
    }
    list->size = 0;
    list->head = NULL;
}

int list_add(list_t *list, int value) {
    if (list == NULL) {
        return -1;
    }

    if (list->head == NULL) {
        list->head = malloc(sizeof(node_t));
        list->head->value = value;
        list->head->next = NULL;
        list->size = 1;
        return 0;
    }

    node_t *current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_t));
    current->next->value = value;
    current->next->next = NULL;
    list->size++;
    return 0;
}

int list_get(list_t *list, int index) {
    if (list == NULL) {
        return -1;
    }
    if (index >= list->size || index < 0) {
        return -1;
    }
    node_t *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->value;
}

void list_free(list_t *list) {
    if (list == NULL) {
        return;
    }

    node_t *current = list->head;
    if (current == NULL) {
        return;
    }

    while (current != NULL) {
        node_t *to_free = current;
        current = current->next;
        free(to_free);
    }
    list->size = 0;
    list->head = NULL;
}

int list_find(list_t *list, int value) {
    if (list == NULL) {
        return -1;
    }

    for (int i = 0; i < list->size; i++) {
        if (list_get(list, i) == value) {
            return i;
        }
    }
    return -1;
}

int array_find(int *a, int len, int value) {
    if (a == NULL) {
        return -1;
    }

    for (int i = 0; i < len; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

int validate_results(int *results, int len) {
    for (int i = 0; i < len; i++) {
        if (i % 2 == 0) {
            if (results[i] != i / 2) {
                return 0;
            }
        } else {
            if (results[i] != -1) {
                return 0;
            }
        }
    }
    return 1;
}
