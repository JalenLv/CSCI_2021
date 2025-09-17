#include "gradebook.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gradebook_t *create_gradebook(const char *class_name) {
    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    if (new_book == NULL) {
        return NULL;
    }

    strcpy(new_book->class_name, class_name);
    new_book->root = NULL;
    return new_book;
}

const char *get_gradebook_name(const gradebook_t *book) {
    // TODO: Not yet implemented
    return NULL;
}

int add_score(gradebook_t *book, const char *name, int score) {
    // TODO: Not yet implemented
    return -1;
}

int find_score(const gradebook_t *book, const char *name) {
    // TODO: Not yet implemented
    return -1;
}

void print_gradebook(const gradebook_t *book) {
    // TODO: Not yet implemented
}

void free_gradebook(gradebook_t *book) {
    // TODO: Not yet implemented
}

// Helper function to allow recursion for writing out tree
int write_gradebook_to_text_aux(const node_t *current, FILE *f) {
    if (current == NULL) {
        return 0;
    }

    // Each line is formatted as "<name> <score>"
    fprintf(f, "%s %d\n", current->name, current->score);

    if (current->left != NULL) {
        if (write_gradebook_to_text_aux(current->left, f) != 0) {
            return -1;
        }
    }

    if (current->right != NULL) {
        if (write_gradebook_to_text_aux(current->right, f) != 0) {
            return -1;
        }
    }

    return 0;
}

int write_gradebook_to_text(const gradebook_t *book) {
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".txt");

    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        return -1;
    }

    int result = write_gradebook_to_text_aux(book->root, f);
    fclose(f);
    return result;
}

gradebook_t *read_gradebook_from_text(const char *file_name) {
    // TODO: Not yet implemented
    return NULL;
}

int write_gradebook_to_binary(const gradebook_t *book) {
    // TODO: Not yet implemented
    return -1;
}

gradebook_t *read_gradebook_from_binary(const char *file_name) {
    // TODO: Not yet implemented
    return NULL;
}
