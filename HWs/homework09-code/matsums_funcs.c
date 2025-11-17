#include "matvec.h"

// Calculates sum of each matrix row, stores in provided
// vector. Returns 0 on success or 1 on a size mismatch.
int row_sums(matrix_t *mat, vector_t *rsums) {
    if (mat->rows != rsums->len) {
        printf("size mismatch: mat->rows %ld != rsums->len %ld\n", mat->rows, rsums->len);
        return 1;
    }

    for (int i = 0; i < mat->rows; i++) {
        int sum = 0;
        for (int j = 0; j < mat->cols; j++) {
            sum += MGET(mat, i, j);
        }
        VSET(rsums, i, sum);
    }
    return 0;
}

// Calculates sum of each matrix column, stores in provided
// vector. Returns 0 on success or 1 on a size mismatch.
int col_sums(matrix_t *mat, vector_t *csums) {
    if (mat->cols != csums->len) {
        printf("size mismatch: mat->cols %ld != rsums->len %ld\n", mat->cols, csums->len);
        return 1;
    }

    for (int j = 0; j < mat->cols; j++) {
        int sum = 0;
        for (int i = 0; i < mat->rows; i++) {
            sum += MGET(mat, i, j);
        }
        VSET(csums, j, sum);
    }
    return 0;
}

// Optimized version of column summing.  Fill in code to provide better
// performance than the provided col_sums by iterating through memory in a
// similar fashion to the row-based solution. Should be much better than
// col_sums() and get close to row_sums() for performance but not likely to
// match it.
int opt_col_sums(matrix_t *mat, vector_t *csums) {
    if (mat->cols != csums->len) {
        printf("size mismatch: mat->cols %ld != rsums->len %ld\n", mat->cols, csums->len);
        return 1;
    }

    int csums_local[csums->len];
    for (int i = 0; i < sizeof(csums_local) / sizeof(csums_local[0]); i++) {
        csums_local[i] = 0;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            csums_local[j] += MGET(mat, i, j);
        }
    }
    for (int i = 0; i < csums->len; i++) {
        VSET(csums, i, csums_local[i]);
    }

    return 0;
}
