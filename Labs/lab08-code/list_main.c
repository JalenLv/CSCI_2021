#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linked_list.h"

typedef struct timespec timespec_t;

double get_elapsed_time_sec(const timespec_t *start, const timespec_t *end) {
    long start_nanos = (long) 1e9 * start->tv_sec + start->tv_nsec;
    long end_nanos = (long) 1e9 * end->tv_sec + end->tv_nsec;
    return (double) (end_nanos - start_nanos) / 1e9;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <mode> <num_elements>\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    int num_elems = atoi(argv[2]);
    int *results = malloc(sizeof(int) * num_elems * 2);
    if (results == NULL) {
        fprintf(stderr, "malloc failed, bailing out\n");
        return 1;
    }

    timespec_t start;
    timespec_t end;
    double time;

    // Linked list case
    if ((strcmp(mode, "list_find") == 0) || (strcmp(mode, "list_find_student") == 0)) {
        // Set up list with sample data to test out search speed
        list_t list;
        list_init(&list);
        for (int i = 0; i < num_elems; i++) {
            list_add(&list, i * 2);
        }

        // C function pointer
        // Lets us call either list_find or list_find student below, depending on mode
        int (*chosen_find_func)(list_t *, int);
        if (strcmp(mode, "list_find") == 0) {
            chosen_find_func = list_find;
        } else {
            chosen_find_func = list_find_student;
        }

        // Start timing here
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        for (int i = 0; i < 2 * num_elems; i++) {
            results[i] = chosen_find_func(&list, i);
        }

        // Stop timing here
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

        list_free(&list);
    }

    // Array case
    else if (strcmp(mode, "array_find") == 0) {
        // Set up array with sample data to test out search speed
        int *arr = malloc(sizeof(int) * num_elems);
        for (int i = 0; i < num_elems; i++) {
            arr[i] = i * 2;
        }

        // Start timing here
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        for (int i = 0; i < 2 * num_elems; i++) {
            results[i] = array_find(arr, num_elems, i);
        }

        // Stop timing here
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

        free(arr);
    }

    else {    // Invalid mode
        printf("Error: Invalid mode %s\n", mode);
        printf("mode must be 'list_find', 'list_find_student', or 'array_find'.\n");
        free(results);
        return 1;
    }

    // Validate results and print out timing data
    if (validate_results(results, num_elems * 2) == 0) {
        printf("Error: Incorrect search results.\n");
        free(results);
        return 1;
    }

    free(results);
    time = get_elapsed_time_sec(&start, &end);
    printf("Total elapsed time: %.4e sec\n", time);
    return 0;
}
