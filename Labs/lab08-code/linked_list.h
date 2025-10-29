/* Struct for a node in our singly linked list */
typedef struct node {
    int value;
    struct node *next;
} node_t;

/*
 * Struct for a singly-linked list.
 * Stores the size of the list and pointer to the head node
 */
typedef struct list {
    int size;
    node_t *head;
} list_t;

/*
 * Sets up a new linked list where size is 0 and head is NULL
 * Arguments:
 *   list: Linked list to initialize
 */
void list_init(list_t *list);

/*
 * Adds a new node to the end of a linked list
 * Arguments:
 *   list: Pointer to list to add to
 *   value: Data to store in new node
 * Returns 0 on success, -1 on failure
 */
int list_add(list_t *list, int value);

/*
 * Retrieves the value stored at a particular index within a linked list
 * Arguments:
 *     list: Pointer to linked list to retrieve from
 *     index: Index of node with value to retrieve
 * Returns: The integer stored at the index, or -1 if index is out of bounds
 */
int list_get(list_t *list, int index);

/*
 * Frees the memory for all the nodes in the list.
 * Arguments:
 *   list: Pointer to list to free
 */
void list_free(list_t *list);

/*
 * Searches for the index at which a particular value is stored.  This
 * implementation calls "get" to retrieve the value at index 0, then calls get
 * to retrieve the value at index 1, and continues in this way until it either
 * finds the value or hits the end of the list.
 * Arguments:
 *     list: Pointer to list to search within
 *     value: The value to search for
 * Returns: Index where the value is stored, or -1 if value is not found
 */
int list_find(list_t *list, int value);

/* A more efficient version of list_find to be written by YOU */
int list_find_student(list_t *list, int value);

/*
 * Searches for the occurrence of a particular value in an array.
 * Arguments:
 *   a: The array to search
 *   len: The length of the array
 *   value: The value to search for
 * Returns: Index where the value is stored, or -1 if value is not found
 */
int array_find(int *a, int len, int value);

/* Checks the data set to make sure the correct results were returned */
int validate_results(int *results, int len);
