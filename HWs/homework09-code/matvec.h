#ifndef MATVEC_H
#define MATVEC_H

#include <stdio.h>

// data type for a matrix (2D array), tracks number of rows and
// columns. Element (i,j) is stored in a linear array at index
// mat.data[(i*mat.cols) + j];
typedef struct {
    long rows;    // number of rows
    long cols;    // number of cols
    int *data;    // actual data
} matrix_t;

// data type for a vector (1D array) which tracks its length and data.
typedef struct {
    long len;     // length of vector
    int *data;    // actual data
} vector_t;

// Macros to get an element from matrix or vector pointers. Substitutes
// in wherever MGET() or VGET() are present for added efficiency.
#define MGET(mat, i, j) ((mat)->data[((i) * ((mat)->cols)) + (j)])
#define VGET(vec, i) ((vec)->data[(i)])

// Macros to set an element in a matrix or vector struct
#define MSET(mat, i, j, x) ((mat)->data[((i) * ((mat)->cols)) + (j)] = (x))
#define VSET(vec, i, x) ((vec)->data[(i)] = (x))

// matvec_util.c
int vector_init(vector_t *vec, long len);
int matrix_init(matrix_t *mat, long rows, long cols);
void vector_free_data(vector_t *vec);
void matrix_free_data(matrix_t *mat);
int vector_read_from_file(char *fname, vector_t *vec_ref);
int matrix_read_from_file(char *fname, matrix_t *mat_ref);
void vector_write(FILE *file, vector_t *vec);
void matrix_write(FILE *file, matrix_t *mat);
void vector_fill_sequential(vector_t *vec);
void matrix_fill_sequential(matrix_t *mat);

// matsums_funcs.c
int row_sums(matrix_t *mat, vector_t *rsums);
int col_sums(matrix_t *mat, vector_t *csums);
int opt_col_sums(matrix_t *mat, vector_t *csums);

#endif    // MATVEC_H
