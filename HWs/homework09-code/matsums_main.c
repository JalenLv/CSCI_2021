// Comparison of row-wise and column-wise summing

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matvec.h"

typedef struct timespec timespec_t;

double get_elapsed_time_sec(const timespec_t *start, const timespec_t *end) {
    long start_nanos = (long) 1e9 * start->tv_sec + start->tv_nsec;
    long end_nanos = (long) 1e9 * end->tv_sec + end->tv_nsec;
    return (double) (end_nanos - start_nanos) / 1e9;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    // Allocate the matrix and fill it in with 1, 2, 3, 4...
    matrix_t mat;
    int ret = matrix_init(&mat, rows, cols);
    if (ret != 0) {
        return ret;
    }
    matrix_fill_sequential(&mat);

    // Variables for timing
    timespec_t start;
    timespec_t end;
    double cpu_time;

    // ROW SUMS
    vector_t rsums;
    vector_init(&rsums, mat.rows);
    vector_fill_sequential(&rsums);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    // get current time
    ret = row_sums(&mat, &rsums);                       // run row_sums
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);      // get current time
    cpu_time = get_elapsed_time_sec(&start, &end);
    printf("%14s CPU usage: %.4e sec\n", "row_sums", cpu_time);

    // COL SUMS
    vector_t csums;
    vector_init(&csums, mat.cols);
    vector_fill_sequential(&csums);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    // get current time
    ret = col_sums(&mat, &csums);                       // run col_sums
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    cpu_time = get_elapsed_time_sec(&start, &end);
    printf("%14s CPU usage: %.4e sec\n", "col_sums", cpu_time);

    ////////////////////////////////////////////////////////////////////////////
    // TODO UNCOMMENT BLOCK BELOW ONCE opt_col_sum() HAS BEEN WRITTEN

    // OPT_COL SUMS
    vector_t ocsums;
    vector_init(&ocsums, mat.cols);
    vector_fill_sequential(&ocsums);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    // get current time
    ret = opt_col_sums(&mat, &ocsums);                  // run opt_col_sums
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);      // get current time
    cpu_time = get_elapsed_time_sec(&start, &end);
    printf("%14s CPU usage: %.4e sec\n", "opt_col_sums", cpu_time);

    // Check answer
    for (int i = 0; i < csums.len; i++) {
        int cs = VGET(&csums, i);
        int os = VGET(&ocsums, i);
        if (cs != os) {
            printf("ERROR: opt_col_sums produced incorrect results\n");
            printf("Element %d: expect %d  actual %d\n", i, cs, os);
            break;
        }
    }
    vector_free_data(&ocsums);

    matrix_free_data(&mat);    // clean up data
    vector_free_data(&rsums);
    vector_free_data(&csums);

    return 0;
}
