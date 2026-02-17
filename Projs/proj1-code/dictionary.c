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

/**
 * Recursively insert a node and balance the tree at every level.
 * Returns the new root of the subtree (may change due to rotations).
 */
node_t *avl_insert(node_t *root, node_t *node) {
    // Base case: empty subtree, insert here
    if (root == NULL) {
        return node;
    }

    int cmp = strcmp(node->word, root->word);
    if (cmp < 0) {
        root->left = avl_insert(root->left, node);
    } else if (cmp > 0) {
        root->right = avl_insert(root->right, node);
    } else {
        free(node);
        return root;
    }

    // Update height
    int left_height = (root->left != NULL) ? root->left->height : 0;
    int right_height = (root->right != NULL) ? root->right->height : 0;
    root->height = max(left_height, right_height) + 1;

    int balance = left_height - right_height;
    // Left Left Case
    if (balance > 1 && strcmp(node->word, root->left->word) < 0) {
        return rotate_right(root);
    }
    // Left Right Case
    if (balance > 1 && strcmp(node->word, root->left->word) > 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    // Right Right Case
    if (balance < -1 && strcmp(node->word, root->right->word) > 0) {
        return rotate_left(root);
    }
    // Right Left Case
    if (balance < -1 && strcmp(node->word, root->right->word) < 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

int dict_insert(dictionary_t *dict, const char *word) {
    node_t *node = malloc(sizeof(node_t));
    new_node(node, word);
    dict->root = avl_insert(dict->root, node);
    dict->size++;
    return 0;
}

/**
 * Helper function to search for a word in the tree.
 * Returns 1 if found, 0 otherwise.
 */
int search_node(node_t *root, const char *query) {
    if (root == NULL) return 0;
    if (strcmp(root->word, query) == 0) return 1;
    if (strcmp(root->word, query) < 0) return search_node(root->right, query);
    if (strcmp(root->word, query) > 0) return search_node(root->left, query);
    return 0;
}

/**
 * Dynamic array macros.
 */
#include <stdlib.h>
#include <string.h>
// Define the fields of a dynamic array for a given type
// Usage: typedef struct { DA_FIELDS(int); } IntArray;
#define DA_FIELDS(type) \
	int count; \
	int capacity; \
	type *items
// Free the dynamic array
// Use this only when the elements do not need to be freed individually
#define DA_FREE(type, arr) do { \
	if ((arr)->items != NULL) { \
		free((arr)->items); \
		(arr)->items = NULL; \
		(arr)->count = 0; \
		(arr)->capacity = 0; \
	} \
} while(0)
// Push an item to the dynamic array
#define DA_PUSH(type, arr, item) do { \
	if ((arr)->count == (arr)->capacity) { \
		(arr)->capacity = (arr)->capacity ? (arr)->capacity * 2 : 1; \
		(arr)->items = (type *)realloc((arr)->items, (arr)->capacity * sizeof(type)); \
	} \
	(arr)->items[(arr)->count++] = item; \
} while(0)
// Pop an item from the dynamic array
#define DA_POP(type, arr) ((arr)->count > 0 ? (type)(arr)->items[--(arr)->count] : (type){0})
// Get an item from the dynamic array by index
#define DA_GET(type, arr, index) (((index) < (arr)->count && (index) >= 0) ? (type)(arr)->items[index] : (type){0})
// Get a pointer to an item from the dynamic array by index
#define DA_GET_PTR(type, arr, index) (((index) < (arr)->count && (index) >= 0) ? (type)&((arr)->items[index]) : NULL)
// Remove an item from the dynamic array by index
#define DA_REMOVE(type, arr, index) do { \
	if ((index) < (arr)->count && (index) >= 0) { \
		memmove(&(arr)->items[index], &(arr)->items[index + 1], ((arr)->count - (index) - 1) * sizeof(type)); \
		(arr)->count--; \
	} \
} while(0)
// Remove a range of items from the dynamic array [start, end)
#define DA_REMOVE_RANGE(type, arr, start, end) do { \
	if ((start) >= 0 && (end) <= (arr)->count && (start) < (end)) { \
		memmove(&(arr)->items[start], &(arr)->items[end], ((arr)->count - (end)) * sizeof(type)); \
		(arr)->count -= ((end) - (start)); \
	} \
} while(0)
// Insert a DA `arr2insert` into another DA `arr` at a given index
#define DA_INSERT(type, arr, index, arr2insert) do { \
	if ((arr2insert)->count > 0 && (index) >= 0 && (index) <= (arr)->count) { \
		while ((arr)->count + (arr2insert)->count > (arr)->capacity) { \
			(arr)->capacity = (arr)->capacity ? (arr)->capacity * 2 : 1; \
			(arr)->items = (type *)realloc((arr)->items, (arr)->capacity * sizeof(type)); \
		} \
		memmove(&(arr)->items[(index) + (arr2insert)->count], &(arr)->items[index], ((arr)->count - (index)) * sizeof(type)); \
		memcpy(&(arr)->items[index], (arr2insert)->items, (arr2insert)->count * sizeof(type)); \
		(arr)->count += (arr2insert)->count; \
	} \
} while(0)

/**
 * Helper string builder implemented using dynamic array
 */
typedef struct {
    DA_FIELDS(char);
} StringBuilder;

void push_zero(StringBuilder *sb) { DA_PUSH(char, sb, '\0'); }

void sb_init(StringBuilder *sb) {
    sb->count = sb->capacity = 0;
    sb->items = NULL;
    push_zero(sb);
}

void sb_free(StringBuilder *sb) {
    DA_FREE(char, sb);
}

void drop_zero(StringBuilder *sb) { if (sb->count > 0) sb->count--; }

void sb_append_n(StringBuilder *sb, const char *s, size_t n) {
    drop_zero(sb);
    for (size_t i = 0; i < n; i++) DA_PUSH(char, sb, s[i]);
    push_zero(sb);
}

void sb_append_c(StringBuilder *sb, char c) { sb_append_n(sb, &c, 1); }
void sb_append_s(StringBuilder *sb, const char *s) { sb_append_n(sb, s, strlen(s)); }

const char *sb_c_str(const StringBuilder *sb) { return sb->items; }

size_t sb_length(const StringBuilder *sb) { return sb->count > 0 ? sb->count - 1 : 0; }

int dict_find(const dictionary_t *dict, const char *query) {
    return search_node(dict->root, query);
}

void tree_to_sb(node_t *root, StringBuilder *sb) {
    if (root->left != NULL) tree_to_sb(root->left, sb);
    sb_append_s(sb, root->word);
    sb_append_c(sb, '\n');
    if (root->right != NULL) tree_to_sb(root->right, sb);
}

void dict_print(const dictionary_t *dict) {
    if (dict->root == NULL) return;

    StringBuilder sb;
    sb_init(&sb);
    tree_to_sb(dict->root, &sb);
    printf("%s", sb_c_str(&sb));
    sb_free(&sb);
}

void free_tree(node_t *root) {
    if (root->left != NULL) free_tree(root->left);
    if (root->right != NULL) free_tree(root->right);
    free(root);
}

void dict_free(dictionary_t *dict) {
    if (dict->root != NULL) free_tree(dict->root);
    free(dict);
}

dictionary_t *read_dict_from_text_file(const char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) return NULL;

    dictionary_t *dict = create_dictionary();
    if (dict == NULL) return NULL;

    char word[MAX_WORD_LEN];
    while (fscanf(f, "%s", word) == 1) {
        dict_insert(dict, word);
    }

    fclose(f);
    return dict;
}

int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    if (dict->root == NULL) return 0;

    StringBuilder sb;
    sb_init(&sb);
    tree_to_sb(dict->root, &sb);

    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        sb_free(&sb);
        return 1;
    }

    if (fwrite(sb_c_str(&sb), sizeof(char), sb_length(&sb), f) != sb_length(&sb)) {
        fclose(f);
        sb_free(&sb);
        return 1;
    }

    fclose(f);
    sb_free(&sb);
    return 0;
}
