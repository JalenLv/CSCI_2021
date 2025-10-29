// SPDX-License-Identifier: GPL-3.0-or-later
// Based on Bryant and O'Halloran's original "datalab" assignment
#ifndef PUZZLE_SPEC_H
#define PUZZLE_SPEC_H

enum returnType {
    INT_RET,
    UNSIGNED_RET,
};

enum argType {
    INT_ARG,
    UNSIGNED_ARG,
    FLOAT_AS_UNSIGNED_ARG,
    UNUSED_ARG,
};

typedef struct {
    char *name;
    enum returnType return_type;
    unsigned num_args;
    enum argType arg_types[3];
    int arg_min[3];            // Bounds are inclusive
    int arg_max[3];            // Bounds are inclusive
    int (*test_func)(void);    // Function pointer that will be cast as needed
    int (*impl_func)(void);    // Function pointer that will be cast as needed
} puzzle_spec_t;

extern puzzle_spec_t puzzle_specs[];

#endif    // PUZZLE_SPEC_H
