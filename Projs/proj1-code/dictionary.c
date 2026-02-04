#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

dictionary_t *create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));
    if (dict == NULL) {
        return NULL;
    }
    dict->root = NULL;
    dict->size = 0;
    return dict;
}

// Helper to prepare a new node. Notice the
// lack of a malloc() call in this function.
// This code is correct and should not be modified.
void new_node(node_t* node, const char *key) {
    strcpy(node->word, key);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
}

// You may find writing your own helper functions
// to be useful.

// Helper: Larger of two numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// Helper to perform the "right" rotation.
// This code is correct and you should not modify it.
node_t *rotate_right(node_t *y)
{
    node_t *x = y->left;
    node_t *t = x->right;
    x->right = y;
    y->left = t;
    // y->height = max(height(y->left), height(y->right)) + 1;
    // x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max((y->left != NULL) ? y->left->height : 0, 
                    (y->right != NULL) ? y->right->height : 0) + 1;
    x->height = max((x->left != NULL) ? x->left->height : 0, 
                    (x->right != NULL) ? x->right->height : 0) + 1;
    return x;
}

// Helper to perform the "left" rotation.
// This code is correct and you should not modify it.
node_t *rotate_left(node_t *x)
{
    node_t *y = x->right;
    node_t *t = y->left;
    y->left = x;
    x->right = t;
    x->height = max((x->left != NULL) ? x->left->height : 0, 
                    (x->right != NULL) ? x->right->height : 0) + 1;
    y->height = max((y->left != NULL) ? y->left->height : 0, 
                    (y->right != NULL) ? y->right->height : 0) + 1;
    return y;
}

int dict_insert(dictionary_t *dict, const char *word) {
    // TODO Not yet implemented
    // Please see the project documentation for guidance
    // on implementing this function. It might be a lot!
    return 0;
}

int dict_find(const dictionary_t *dict, const char *query) {
    // TODO Not yet implemented
    return 0;
}

void dict_print(const dictionary_t *dict) {
    // TODO Not yet implemented
}

void dict_free(dictionary_t *dict) {
    // TODO Not yet implemented
}

dictionary_t *read_dict_from_text_file(const char *file_name) {
    // TODO Not yet implemented
    return NULL;
}

int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    // TODO Not yet implemented
    return 0;
}
