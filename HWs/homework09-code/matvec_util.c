#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "matvec.h"

// Allocates memory for the parameter vector vec. Sets its data field
// to point at a proper amount of memory and sets the len field
// according to parameter len. Returns 0 on success and nonzero
// if len is 0 or negative.
int vector_init(vector_t *vec, long len) {
    if (len <= 0) {
        printf("Invalid length: %ld\n", len);
        return 1;
    }
    vec->data = malloc(sizeof(int) * len);
    vec->len = len;
    return 0;
}

// Allocates memory for the parameter matrix mat. Sets its data field
// to point at a proper amount of memory and sets the rows and cols fields
// according to parameters rows and cols. Returns 0 on success and nonzero
// if rows or cols are 0 or negative.
int matrix_init(matrix_t *mat, long rows, long cols) {
    if (rows <= 0 || cols <= 0) {
        printf("Invalid rows or cols: %ld %ld\n", rows, cols);
        return 1;
    }
    mat->data = malloc(sizeof(int) * rows * cols);
    mat->rows = rows;
    mat->cols = cols;
    return 0;
}

// Frees memory associated with the data field of vec.
void vector_free_data(vector_t *vec) {
    free(vec->data);
    vec->len = -1;
}

// Frees memory associated with the data field of mat.
void matrix_free_data(matrix_t *mat) {
    free(mat->data);
    mat->rows = -1;
    mat->cols = -1;
}

// Reads data from the specified file and initializes specified vector
// with this data. Allocates memory and checks for correct dimensions. The
// format of the file is space-separated numbers.
//   - first long indicates size of vector
//   - remaining ints are data in the vector
// Returns 0 on success and non-zero on error.
int vector_read_from_file(char *fname, vector_t *vec) {
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        perror("couldn't open vector file");
        return 1;
    }

    long len;
    assert(fscanf(file, "%ld", &len) == 1);
    int ret = vector_init(vec, len);
    if (ret != 0) {
        return ret;
    }

    for (int i = 0; i < len; i++) {
        int x;
        assert(fscanf(file, "%d", &x) == 1);
        VSET(vec, i, x);
    }

    fclose(file);
    return 0;
}

// Reads data from the specified file and initializes specified matrix
// with this data. Allocates memory and checks for correct dimensions. The
// format of the file is space separated numbers.
//   - first two longs indicate size of matrix
//   - remaining ints are data in the matrix
// Returns 0 on success and non-zero on error.
int matrix_read_from_file(char *fname, matrix_t *mat_ref) {
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        perror("couldn't open matrix file");
        return 1;
    }

    long rows;
    long cols;
    assert(fscanf(file, "%ld %ld", &rows, &cols) == 2);
    matrix_t mat;
    int ret = matrix_init(&mat, rows, cols);
    if (ret != 0) {
        return ret;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int x;
            assert(fscanf(file, "%d", &x) == 1);
            MSET(&mat, i, j, x);
        }
    }

    fclose(file);
    *mat_ref = mat;
    return 0;
}

// Writes a vector to an open file handle. Prints some dimension
// information followed by index and data on each line. Use with
// stdout to print to the screen.
void vector_write(FILE *file, vector_t *vec) {
    fprintf(file, "%ld x 1 vector\n", vec->len);
    for (int i = 0; i < vec->len; i++) {
        fprintf(file, "%4d: ", i);
        fprintf(file, "%4d\n", VGET(vec, i));
    }
}

// Writes a matrix to an open file handle. Prints some dimension
// information followed by index and data on each line. Use with
// stdout to print to the screen.
void matrix_write(FILE *file, matrix_t *mat) {
    fprintf(file, "%ld x %ld matrix\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        fprintf(file, "%4d: ", i);
        for (int j = 0; j < mat->cols; j++) {
            fprintf(file, "%4d ", MGET(mat, i, j));
        }
        fprintf(file, "\n");
    }
}

// Set elements of the given vector to 0,1,2,...,len
void vector_fill_sequential(vector_t *vec) {
    for (int i = 0; i < vec->len; i++) {
        VSET(vec, i, i);
    }
}

// Set elements of the given matrix to 0,1,2,...,len.
void matrix_fill_sequential(matrix_t *mat) {
    int c = 0;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            MSET(mat, i, j, c);
            c++;
        }
    }
}
