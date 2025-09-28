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
    return book->class_name;
}

/**
 * Helper function for `add_score`.
 */
int add_score_recursive(node_t *sub_tree, node_t *new_node) {
    int cmp = strcmp(new_node->name, sub_tree->name);
    if (cmp < 0) {
        // Add to the left
        if (sub_tree->left == NULL) {
            sub_tree->left = new_node;
            return 0;
        } else {
            return add_score_recursive(sub_tree->left, new_node);
        }
    } else if (cmp > 0) {
        // Add to the right
        if (sub_tree->right == NULL) {
            sub_tree->right = new_node;
            return 0;
        } else {
            return add_score_recursive(sub_tree->right, new_node);
        }
    } else {
        // We assume that each student has a unique name
        return -1;
    }
}

int add_score(gradebook_t *book, const char *name, int score) {
    // Allocate a new node
    node_t *new_node = malloc(sizeof(node_t));
    strcpy(new_node->name, name);
    new_node->score = score;
    new_node->left = new_node->right = NULL;

    // If the tree is empty, the new node is the root
    if (book->root == NULL) {
        book->root = new_node;
        return 0;
    }

    // Or find the leaf node to add the new node
    return add_score_recursive(book->root, new_node);
}

/**
 * Helper function for `find_score`.
 */
int find_score_recursive(node_t *sub_tree, const char *name) {
    if (sub_tree == NULL)
        return -1;

    int cmp = strcmp(name, sub_tree->name);
    if (cmp < 0) {
        return find_score_recursive(sub_tree->left, name);
    } else if (cmp > 0) {
        return find_score_recursive(sub_tree->right, name);
    } else {
        return sub_tree->score;
    }
}

int find_score(const gradebook_t *book, const char *name) {
    return find_score_recursive(book->root, name);
}

void print_gradebook_recursive(node_t *sub_tree) {
    if (sub_tree == NULL)
        return;

    print_gradebook_recursive(sub_tree->left);
    printf("%s: %d\n", sub_tree->name, sub_tree->score);
    print_gradebook_recursive(sub_tree->right);
}

void print_gradebook(const gradebook_t *book) {
    printf("Scores for all students in %s:\n", book->class_name);
    print_gradebook_recursive(book->root);
}

void free_tree(node_t *sub_tree) {
    if (sub_tree == NULL)
        return;

    free_tree(sub_tree->left);
    free_tree(sub_tree->right);
    free(sub_tree);
}

void free_gradebook(gradebook_t *book) {
    if (book == NULL)
        return;

    free_tree(book->root);
    free(book);
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

/**
 * Helper function to get the stem of the file name.
 */
char *strip_ext(const char *file_name) {
    char *stem = strdup(file_name);

    char *end = stem + strlen(stem);
    while (end != stem && *end != '.')
        --end;

    if (end == stem) {
        free(stem);
        return NULL;
    }

    *end = '\0';
    return stem;
}

gradebook_t *read_gradebook_from_text(const char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
        return NULL;

    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    char *class_name = strip_ext(file_name);
    if (class_name == NULL) {
        fclose(f);
        free(new_book);
        return NULL;
    }
    strcpy(new_book->class_name, class_name);
    new_book->root = NULL;

    char name[MAX_NAME_LEN];
    int score;
    while (fscanf(f, "%s %d", name, &score) != EOF)
        add_score(new_book, name, score);

    fclose(f);
    free(class_name);
    return new_book;
}

/**
 * Simple wraper around fwrite to enable result checking.
 */
int write_binaries(void *ptr, size_t size, size_t n, FILE *f) {
    if (fwrite(ptr, size, n, f) != n)
        return -1;
    return 0;
}

/**
 * Helper function to implement `write_gradebook_to_binary`.
 */
int write_gradebook_to_binary_recursive(node_t *sub_tree, FILE *f) {
    if (sub_tree == NULL)
        return 0;

    unsigned len = strlen(sub_tree->name);
    if (write_binaries(&len, sizeof(unsigned), 1, f) != 0)
        return -1;
    if (write_binaries(sub_tree->name, sizeof(char), len, f) != 0)
        return -1;
    if (write_binaries(&sub_tree->score, sizeof(int), 1, f) != 0)
        return -1;

    write_gradebook_to_binary_recursive(sub_tree->left, f);
    write_gradebook_to_binary_recursive(sub_tree->right, f);
    return 0;
}

int write_gradebook_to_binary(const gradebook_t *book) {
    char file_name[MAX_NAME_LEN + strlen(".bin")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".bin");

    FILE *f = fopen(file_name, "wb");
    if (f == NULL) {
        return -1;
    }

    int result = write_gradebook_to_binary_recursive(book->root, f);
    fclose(f);
    return result;
}

/**
 * Simple wraper around fread to enable result checking.
 * Returns -1 if EOF is encountered, 1 if error occurs.
 */
int read_binaries(void *ptr, size_t size, size_t n, FILE *f) {
    if (fread(ptr, size, n, f) != n) {
        if (feof(f))
            return -1;
        else if (ferror(f))
            return 1;
    }
    return 0;
}

/**
 * Helper function to read from a line the name and the score.
 */
int read_line(char name[MAX_NAME_LEN], int *score, FILE *f) {
    int result;
    unsigned len;
    result = read_binaries(&len, sizeof(unsigned), 1, f);
    if (result != 0)
        return result;

    result = read_binaries(name, sizeof(char), len, f);
    if (result != 0)
        return result;
    name[len] = '\0';

    result = read_binaries(score, sizeof(int), 1, f);
    if (result != 0)
        return result;

    return 0;
}

gradebook_t *read_gradebook_from_binary(const char *file_name) {
    FILE *f = fopen(file_name, "rb");
    if (f == NULL)
        return NULL;

    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    char *class_name = strip_ext(file_name);
    if (class_name == NULL) {
        fclose(f);
        free(new_book);
        return NULL;
    }
    strcpy(new_book->class_name, class_name);
    new_book->root = NULL;

    char name[MAX_NAME_LEN];
    memset(name, 0, sizeof(name));
    int score = 0;
    int result;
    while ((result = read_line(name, &score, f)) == 0)
        add_score(new_book, name, score);

    if (result == 1) {
        fclose(f);
        free_gradebook(new_book);
        free(class_name);
        return NULL;
    }

    fclose(f);
    free(class_name);
    return new_book;
}
