// struct_stride.c: Comparison of struct layouts and cache
// effects. Structs have their fields laid out sequentially in memory.
// This can create strides when visiting the same field in arrays of
// structs. For efficiency, one sometime sees structs where each field
// is an array of values which means all values are stored sequentially.

// Complete the TODO items in this code so that it times each set of
// loops and prints output

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {    // fields are individual ints
    int a;
    int b;
} int_field_t;

typedef struct {    // fields are arrays of ints
    int *a_arr;
    int *b_arr;
} arr_field_t;

typedef struct timespec timespec_t;

// Computes the difference in seconds between two timestamps
double get_elapsed_time_sec(const timespec_t *start, const timespec_t *end) {
    long start_nanos = (long) 1e9 * start->tv_sec + start->tv_nsec;
    long end_nanos = (long) 1e9 * end->tv_sec + end->tv_nsec;
    return (double) (end_nanos - start_nanos) / 1e9;
}

// Use this format string for printf() invocations so that the output is
// identical for each timing run
const char *FORMAT = "method: %14s CPU time: %.4e sec sum: %d\n";
//                      |             |               +-> sum computed in loop, should be 0
//                      |             |                   each time
//                      |             +-> CPU time computed over nested summing loops
//                      +-> string describing one of the methods used to print;
//                          One of: "int_field_base"  "arr_field_base"
//                                  "int_field_optm"  "arr_field_optm"
//
// Example: printf(FORMAT, "int_field_optm", some_time, total);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: %s <arr_length> <num_iters>\n", argv[0]);
        return 1;
    }
    int length = atoi(argv[1]);
    int max_iter = atoi(argv[2]);

    // TODO Uncomment these declarations and use these variables below
    // Variables for timing
    // timespec_t start;
    // timespec_t end;
    // double cpu_time;

    // sums of elements in structs
    int sum_a;
    int sum_b;

    ////////////////////////////////////////////////////////////////////////////
    // int_field_base approach

    // First allocate and then initialize array elements
    int_field_t *int_field_arr1 = malloc(sizeof(int_field_t) * length);
    for (int i = 0; i < length; i++) {
        int_field_arr1[i].a = i;
        int_field_arr1[i].b = -i;
    }

    // TODO: START TIMING HERE

    sum_a = 0;    // Initialize sum variables
    sum_b = 0;
    for (int iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < length; i++) {    // sum a fields
            sum_a += int_field_arr1[i].a;
        }
        for (int i = 0; i < length; i++) {    // sum b fields
            sum_b += int_field_arr1[i].b;
        }
    }
    sum_a += sum_b;
    // TODO: STOP TIMING HERE

    // TODO: COMPUTE CPU TIME AND PRINT IT using the provided FORMAT string

    free(int_field_arr1);    // free memory

    ////////////////////////////////////////////////////////////////////////////
    // arr_field_base approach

    arr_field_t arr_field1 = {
        // allocate/initialize arr_field1 struct
        .a_arr = malloc(sizeof(int) * length),
        .b_arr = malloc(sizeof(int) * length),
    };
    for (int i = 0; i < length; i++) {
        arr_field1.a_arr[i] = i;
        arr_field1.b_arr[i] = -i;
    }

    // TODO: START TIMING HERE

    sum_a = 0;    // Initialize sum variables
    sum_b = 0;
    for (int iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < length; i++) {    // sum a fields
            sum_a += arr_field1.a_arr[i];
        }
        for (int i = 0; i < length; i++) {    // sum b fields
            sum_b += arr_field1.b_arr[i];
        }
    }
    sum_a += sum_b;

    // TODO: STOP TIMING HERE

    // TODO: COMPUTE CPU TIME AND PRINT IT using the provided FORMAT string

    free(arr_field1.a_arr);    // free memory
    free(arr_field1.b_arr);

    ////////////////////////////////////////////////////////////////////////////////
    // int_field_optm OPTIMIZED approach

    // Allocate and initialize array
    int_field_t *int_field_arr2 = malloc(sizeof(int_field_t) * length);
    for (int i = 0; i < length; i++) {
        int_field_arr2[i].a = i;
        int_field_arr2[i].b = -i;
    }

    // TODO: START TIMING HERE

    sum_a = 0;    // Initialize sum variables
    sum_b = 0;
    for (int iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < length; i++) {    // sum both fields
            sum_a += int_field_arr2[i].a;
            sum_b += int_field_arr2[i].b;
        }
    }
    sum_a += sum_b;

    // TODO: STOP TIMING HERE

    // TODO: COMPUTE CPU TIME AND PRINT IT using the provided FORMAT string

    free(int_field_arr2);    // free memory

    ////////////////////////////////////////////////////////////////////////////////
    // arr_field_optm OPTIMIZED approach

    arr_field_t arr_field2 = {
        // allocate/initialize arr_field2 struct
        .a_arr = malloc(sizeof(int) * length),
        .b_arr = malloc(sizeof(int) * length),
    };
    for (int i = 0; i < length; i++) {
        arr_field2.a_arr[i] = i;
        arr_field2.b_arr[i] = -i;
    }

    // TODO: START TIMING HERE

    sum_a = 0;    // Initialize sum variables
    sum_b = 0;
    for (int iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < length; i++) {    // sum both fields
            sum_a += arr_field2.a_arr[i];
            sum_b += arr_field2.b_arr[i];
        }
    }
    sum_a += sum_b;

    // TODO: STOP TIMING HERE

    // TODO: COMPUTE CPU TIME AND PRINT IT using the provided FORMAT string

    free(arr_field2.a_arr);    // free memory
    free(arr_field2.b_arr);

    return 0;
}
