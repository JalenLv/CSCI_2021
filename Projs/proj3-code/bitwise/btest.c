/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * btest.c - A test harness that checks bitwise puzzle solutions for correctness
 * This is a modified version of 'btest.c' from David O'Halloran and Randy Bryant's
 * original "datalab" assignment
 *
 * Improvements by John Kolb <jhkolb@umn.edu>
 */
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "puzzle_spec.h"

/* For functions with a single argument, generate TEST_RANGE values
   above and below the min and max test values, and above and below
   zero. Functions with two or three args will use square and cube
   roots of this value, respectively, to avoid combinatorial
   explosion */
#define TEST_RANGE 500000

/* This defines the maximum size of any test value array. The
   gen_vals() routine creates k test values for each value of
   TEST_RANGE, thus MAX_TEST_VALS must be at least k*TEST_RANGE */
#define MAX_TEST_VALS 13 * TEST_RANGE

extern puzzle_spec_t puzzle_specs[];

/*
 * random_val - Return random integer value between min and max
 */
static int random_val(int min, int max) {
    double weight = rand() / (double) RAND_MAX;
    int result = min * (1 - weight) + max * weight;
    return result;
}

/*
 * gen_vals - Generate the integer values we'll use to test a function
 */
static int gen_vals(int test_vals[], int min, int max, bool is_float_input, int test_range) {
    int test_count = 0;

    /*
     * Special case: Generate test vals for floating point functions
     * where the input argument is an unsigned bit-level
     * representation of a float. For this case we want to test the
     * regions around zero, the smallest normalized and largest
     * denormalized numbers, one, and the largest normalized number,
     * as well as inf and nan.
     */
    if (is_float_input) {
        unsigned smallest_norm = 0x00800000;
        unsigned one = 0x3f800000;
        unsigned largest_norm = 0x7f000000;

        unsigned inf = 0x7f800000;
        unsigned nan = 0x7fc00000;
        unsigned sign = 0x80000000;

        /* Test range should be at most 1/2 the range of one exponent
           value */
        if (test_range > (1 << 23)) {
            test_range = 1 << 23;
        }

        /* Functions where the input argument is an unsigned bit-level
           representation of a float. The number of tests generated
           inside this loop body is the value k referenced in the
           comment for the global variable MAX_TEST_VALS. */

        for (int i = 0; i < test_range; i++) {
            /* Denorms around zero */
            test_vals[test_count++] = i;
            test_vals[test_count++] = sign | i;

            /* Region around norm to denorm transition */
            test_vals[test_count++] = smallest_norm + i;
            test_vals[test_count++] = smallest_norm - i;
            test_vals[test_count++] = sign | (smallest_norm + i);
            test_vals[test_count++] = sign | (smallest_norm - i);

            /* Region around one */
            test_vals[test_count++] = one + i;
            test_vals[test_count++] = one - i;
            test_vals[test_count++] = sign | (one + i);
            test_vals[test_count++] = sign | (one - i);

            /* Region below largest norm */
            test_vals[test_count++] = largest_norm - i;
            test_vals[test_count++] = sign | (largest_norm - i);
        }

        /* special vals */
        test_vals[test_count++] = inf;        /* inf */
        test_vals[test_count++] = sign | inf; /* -inf */
        test_vals[test_count++] = nan;        /* nan */
        test_vals[test_count++] = sign | nan; /* -nan */

        return test_count;
    }

    /*
     * Normal case: Generate test vals for integer functions
     */

    /* If the range is small enough, then do exhaustively */
    if (max - MAX_TEST_VALS <= min) {
        for (int i = min; i <= max; i++)
            test_vals[test_count++] = i;
        return test_count;
    }

    /* Otherwise, need to sample.  Do so near the boundaries, around
       zero, and for some random cases. */
    for (int i = 0; i < test_range; i++) {
        /* Test around the boundaries */
        test_vals[test_count++] = min + i;
        test_vals[test_count++] = max - i;

        /* If zero falls between min and max, then also test around zero */
        if (i >= min && i <= max) {
            test_vals[test_count++] = i;
        }
        if (-i >= min && -i <= max) {
            test_vals[test_count++] = -i;
        }

        /* Random case between min and max */
        test_vals[test_count++] = random_val(min, max);
    }
    return test_count;
}

/*
 * Test a specific function.
 * Returns 0 on success and -1 on failure
 */
static int test_function(puzzle_spec_t *spec, unsigned *input_args[3]) {
    /* These are the test values for each arg. Declared with the
       static attribute so that the array will be allocated in bss
       rather than the stack */
    static int arg_test_vals[3][MAX_TEST_VALS];
    unsigned num_args[3];

    unsigned test_range;
    /* Assign range of argument test vals so as to conserve the total
       number of tests, independent of number of arguments */
    switch (spec->num_args) {
        case 0:
        case 1:
            test_range = TEST_RANGE;
            break;
        case 2:
            test_range = pow((double) TEST_RANGE, 0.5); /* sqrt */
            break;
        case 3:
            test_range = pow((double) TEST_RANGE, 0.333); /* cbrt */
            break;
        default:
            printf("Error: Invalid number of arguments for test case '%s'\n", spec->name);
            exit(1);
    }

    for (int i = 0; i < spec->num_args; i++) {
        bool is_float_input;
        switch (spec->arg_types[i]) {
            case INT_ARG:
            case UNSIGNED_ARG:
                is_float_input = false;
                break;
            case FLOAT_AS_UNSIGNED_ARG:
                is_float_input = true;
                break;
            default:
                printf("Error: Unknown type for argument %d of test case '%s'\n", i + 1,
                       spec->name);
                exit(1);
        }
        if (input_args[i] != NULL) {
            num_args[i] = 1;
            unsigned *arg_ptr = input_args[i];
            arg_test_vals[i][0] = *arg_ptr;
        } else {
            num_args[i] = gen_vals(arg_test_vals[i], spec->arg_min[i], spec->arg_max[i],
                                   is_float_input, test_range);
        }
    }

    switch (spec->num_args) {
        case 0:
            switch (spec->return_type) {
                case INT_RET: {
                    int (*impl)(void) = spec->impl_func;
                    int (*test)(void) = spec->test_func;
                    int actual = impl();
                    int expected = test();
                    if (actual != expected) {
                        printf(
                            "ERROR: Test %s() failed...\n...Gives %d[0x%x]. Should be %d[0x%x]\n",
                            spec->name, actual, actual, expected, expected);
                        return -1;
                    }
                    return 0;
                }

                case UNSIGNED_RET: {
                    unsigned (*impl)(void) = (unsigned (*)(void)) spec->impl_func;
                    unsigned (*test)(void) = (unsigned (*)(void)) spec->test_func;
                    unsigned actual = impl();
                    unsigned expected = test();
                    if (actual != expected) {
                        printf(
                            "ERROR: Test %s() failed...\n...Gives %d[0x%x]. Should be %d[0x%x]\n",
                            spec->name, actual, actual, expected, expected);
                        return -1;
                    }
                    return 0;
                }

                default:
                    printf("Unkonwn return type for test '%s'\n", spec->name);
                    exit(1);
            }

        case 1:
            switch (spec->return_type) {
                case INT_RET:
                    switch (spec->arg_types[0]) {
                        case INT_ARG: {
                            int (*impl)(int) = (int (*)(int)) spec->impl_func;
                            int (*test)(int) = (int (*)(int)) spec->test_func;
                            for (int i = 0; i < num_args[0]; i++) {
                                int arg1 = arg_test_vals[0][i];
                                int actual = impl(arg1);
                                int expected = test(arg1);
                                if (actual != expected) {
                                    printf(
                                        "ERROR: Test %s(%d[0x%x]) failed...\n...Gives %d[0x%x]. "
                                        "Should be %d[0x%x]\n",
                                        spec->name, arg1, arg1, actual, actual, expected, expected);
                                    return -1;
                                }
                            }
                            return 0;
                        }

                        case FLOAT_AS_UNSIGNED_ARG:
                        case UNSIGNED_ARG: {
                            int (*impl)(unsigned) = (int (*)(unsigned)) spec->impl_func;
                            int (*test)(unsigned) = (int (*)(unsigned)) spec->test_func;
                            for (int i = 0; i < num_args[0]; i++) {
                                unsigned arg1 = arg_test_vals[0][i];
                                int actual = impl(arg1);
                                int expected = test(arg1);
                                if (actual != expected) {
                                    printf(
                                        "ERROR: Test %s(%d[0x%x]) failed...\n...Gives %d[0x%x]. "
                                        "Should be %d[0x%x]\n",
                                        spec->name, arg1, arg1, actual, actual, expected, expected);
                                    return -1;
                                }
                            }
                            return 0;
                        }

                        default:
                            printf("Unknown type for argument 1 of test '%s'\n", spec->name);
                            exit(1);
                    }

                case UNSIGNED_RET:
                    switch (spec->arg_types[0]) {
                        case INT_ARG: {
                            unsigned (*impl)(int) = (unsigned (*)(int)) spec->impl_func;
                            unsigned (*test)(int) = (unsigned (*)(int)) spec->test_func;
                            for (int i = 0; i < num_args[0]; i++) {
                                int arg1 = arg_test_vals[0][i];
                                unsigned actual = impl(arg1);
                                unsigned expected = test(arg1);
                                if (actual != expected) {
                                    printf(
                                        "ERROR: Test %s(%u[0x%x]) failed...\n...Gives %u[0x%x]. "
                                        "Should be %u[0x%x]\n",
                                        spec->name, arg1, arg1, actual, actual, expected, expected);
                                    return -1;
                                }
                            }
                            return 0;
                        }

                        case FLOAT_AS_UNSIGNED_ARG:
                        case UNSIGNED_ARG: {
                            unsigned (*impl)(unsigned) = (unsigned (*)(unsigned)) spec->impl_func;
                            unsigned (*test)(unsigned) = (unsigned (*)(unsigned)) spec->test_func;
                            for (int i = 0; i < num_args[0]; i++) {
                                unsigned arg1 = arg_test_vals[0][i];
                                unsigned actual = impl(arg1);
                                unsigned expected = test(arg1);
                                if (actual != expected) {
                                    printf(
                                        "ERROR: Test %s(%u[0x%x]) failed...\n...Gives %u[0x%x]. "
                                        "Should be %u[0x%x]\n",
                                        spec->name, arg1, arg1, actual, actual, expected, expected);
                                    return -1;
                                }
                            }
                            return 0;
                        }

                        default:
                            printf("Unknown type for argument 1 of test '%s'\n", spec->name);
                            exit(1);
                    }

                default:
                    printf("Unknown return type for test '%s'\n", spec->name);
                    exit(1);
            }

        case 2:
            switch (spec->return_type) {
                case INT_RET:
                    switch (spec->arg_types[0]) {
                        case INT_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG: {
                                    int (*impl)(int, int) = (int (*)(int, int)) spec->impl_func;
                                    int (*test)(int, int) = (int (*)(int, int)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            int arg1 = arg_test_vals[0][i];
                                            int arg2 = arg_test_vals[1][j];
                                            int actual = impl(arg1, arg2);
                                            int expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%d[0x%x],%d[0x%x]) "
                                                    "failed...\n...Gives %d[0x%x]. Should be "
                                                    "%d[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    int (*impl)(int, unsigned) =
                                        (int (*)(int, unsigned)) spec->impl_func;
                                    int (*test)(int, unsigned) =
                                        (int (*)(int, unsigned)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            int arg1 = arg_test_vals[0][i];
                                            unsigned arg2 = arg_test_vals[1][j];
                                            int actual = impl(arg1, arg2);
                                            int expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%d[0x%x],%u[0x%x]) "
                                                    "failed...\n...Gives %d[0x%x]. Should be "
                                                    "%d[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                default:
                                    printf("Unkonwn type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        case FLOAT_AS_UNSIGNED_ARG:
                        case UNSIGNED_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG: {
                                    int (*impl)(unsigned, int) =
                                        (int (*)(unsigned, int)) spec->impl_func;
                                    int (*test)(unsigned, int) =
                                        (int (*)(unsigned, int)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            unsigned arg1 = arg_test_vals[0][i];
                                            int arg2 = arg_test_vals[1][j];
                                            int actual = impl(arg1, arg2);
                                            int expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%u[0x%x],%d[0x%x]) "
                                                    "failed...\n...Gives %d[0x%x]. Should be "
                                                    "%d[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    int (*impl)(unsigned, unsigned) =
                                        (int (*)(unsigned, unsigned)) spec->impl_func;
                                    int (*test)(unsigned, unsigned) =
                                        (int (*)(unsigned, unsigned)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            unsigned arg1 = arg_test_vals[0][i];
                                            unsigned arg2 = arg_test_vals[1][j];
                                            int actual = impl(arg1, arg2);
                                            int expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%u[0x%x],%u[0x%x]) "
                                                    "failed...\n...Gives %d[0x%x]. Should be "
                                                    "%d[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                default:
                                    printf("Unknown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        default:
                            printf("Unknown type for argument 1 of test '%s'\n", spec->name);
                            exit(1);
                    }
                case UNSIGNED_RET:
                    switch (spec->arg_types[0]) {
                        case INT_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG: {
                                    unsigned (*impl)(int, int) =
                                        (unsigned (*)(int, int)) spec->impl_func;
                                    unsigned (*test)(int, int) =
                                        (unsigned (*)(int, int)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            int arg1 = arg_test_vals[0][i];
                                            int arg2 = arg_test_vals[1][j];
                                            unsigned actual = impl(arg1, arg2);
                                            unsigned expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%d[0x%x],%d[0x%x]) "
                                                    "failed...\n...Gives %u[0x%x]. Should be "
                                                    "%u[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    unsigned (*impl)(int, unsigned) =
                                        (unsigned (*)(int, unsigned)) spec->impl_func;
                                    unsigned (*test)(int, unsigned) =
                                        (unsigned (*)(int, unsigned)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            int arg1 = arg_test_vals[0][i];
                                            unsigned arg2 = arg_test_vals[1][j];
                                            unsigned actual = impl(arg1, arg2);
                                            unsigned expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%d[0x%x],%u[0x%x]) "
                                                    "failed...\n...Gives %u[0x%x]. Should be "
                                                    "%u[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                default:
                                    printf("Unknown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        case FLOAT_AS_UNSIGNED_ARG:
                        case UNSIGNED_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG: {
                                    unsigned (*impl)(unsigned, int) =
                                        (unsigned (*)(unsigned, int)) spec->impl_func;
                                    unsigned (*test)(unsigned, int) =
                                        (unsigned (*)(unsigned, int)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            unsigned arg1 = arg_test_vals[0][i];
                                            int arg2 = arg_test_vals[1][j];
                                            unsigned actual = impl(arg1, arg2);
                                            unsigned expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%u[0x%x],%d[0x%x]) "
                                                    "failed...\n...Gives %u[0x%x]. Should be "
                                                    "%u[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    unsigned (*impl)(unsigned, unsigned) =
                                        (unsigned (*)(unsigned, unsigned)) spec->impl_func;
                                    unsigned (*test)(unsigned, unsigned) =
                                        (unsigned (*)(unsigned, unsigned)) spec->test_func;
                                    for (int i = 0; i < num_args[0]; i++) {
                                        for (int j = 0; j < num_args[1]; j++) {
                                            unsigned arg1 = arg_test_vals[0][i];
                                            unsigned arg2 = arg_test_vals[1][j];
                                            unsigned actual = impl(arg1, arg2);
                                            unsigned expected = test(arg1, arg2);
                                            if (actual != expected) {
                                                printf(
                                                    "ERROR: Test %s(%u[0x%x],%u[0x%x]) "
                                                    "failed...\n...Gives %u[0x%x]. Should be "
                                                    "%u[0x%x]\n",
                                                    spec->name, arg1, arg1, arg2, arg2, actual,
                                                    actual, expected, expected);
                                                return -1;
                                            }
                                        }
                                    }
                                    return 0;
                                }

                                default:
                                    printf("Unknown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        default:
                            printf("Unknown type for argument 1 of test '%s'\n", spec->name);
                            exit(1);
                    }

                default:
                    printf("Unknown return type for test '%s'\n", spec->name);
                    exit(1);
            }

        case 3:
            switch (spec->return_type) {
                case INT_RET:
                    switch (spec->arg_types[0]) {
                        case INT_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG:
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            int (*impl)(int, int, int) =
                                                (int (*)(int, int, int)) spec->impl_func;
                                            int (*test)(int, int, int) =
                                                (int (*)(int, int, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %d[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            int (*impl)(int, int, unsigned) =
                                                (int (*)(int, int, unsigned)) spec->impl_func;
                                            int (*test)(int, int, unsigned) =
                                                (int (*)(int, int, unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %d[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            int (*impl)(int, unsigned, int) =
                                                (int (*)(int, unsigned, int)) spec->impl_func;
                                            int (*test)(int, unsigned, int) =
                                                (int (*)(int, unsigned, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %u[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            int (*impl)(int, unsigned, unsigned) =
                                                (int (*)(int, unsigned, unsigned)) spec->impl_func;
                                            int (*test)(int, unsigned, unsigned) =
                                                (int (*)(int, unsigned, unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %u[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }
                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }
                                }
                                default:
                                    printf("Error: Unkown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        case FLOAT_AS_UNSIGNED_ARG:
                        case UNSIGNED_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG:
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            int (*impl)(unsigned, int, int) =
                                                (int (*)(unsigned, int, int)) spec->impl_func;
                                            int (*test)(unsigned, int, int) =
                                                (int (*)(unsigned, int, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %d[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            int (*impl)(unsigned, int, unsigned) =
                                                (int (*)(unsigned, int, unsigned)) spec->impl_func;
                                            int (*test)(unsigned, int, unsigned) =
                                                (int (*)(unsigned, int, unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %d[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        default:
                                            printf("Unkown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                    }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            int (*impl)(unsigned, unsigned, int) =
                                                (int (*)(unsigned, unsigned, int)) spec->impl_func;
                                            int (*test)(unsigned, unsigned, int) =
                                                (int (*)(unsigned, unsigned, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %u[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            int (*impl)(unsigned, unsigned, unsigned) = (int (*)(
                                                unsigned, unsigned, unsigned)) spec->impl_func;
                                            int (*test)(unsigned, unsigned, unsigned) = (int (*)(
                                                unsigned, unsigned, unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        int actual = impl(arg1, arg2, arg3);
                                                        int expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %u[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %d[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }
                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }
                                }
                                default:
                                    printf("Error: Unkown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        default:
                            printf("Error: Unknown type for argument 1 of test '%s'\n", spec->name);
                            exit(1);
                    }

                case UNSIGNED_RET:
                    switch (spec->arg_types[0]) {
                        case INT_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG:
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            unsigned (*impl)(int, int, int) =
                                                (unsigned (*)(int, int, int)) spec->impl_func;
                                            unsigned (*test)(int, int, int) =
                                                (unsigned (*)(int, int, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %d[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            unsigned (*impl)(int, int, unsigned) =
                                                (unsigned (*)(int, int, unsigned)) spec->impl_func;
                                            unsigned (*test)(int, int, unsigned) =
                                                (unsigned (*)(int, int, unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %d[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%d[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            unsigned (*impl)(int, unsigned, int) =
                                                (unsigned (*)(int, unsigned, int)) spec->impl_func;
                                            unsigned (*test)(int, unsigned, int) =
                                                (unsigned (*)(int, unsigned, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %u[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            unsigned (*impl)(int, unsigned, unsigned) =
                                                (unsigned (*)(int, unsigned,
                                                              unsigned)) spec->impl_func;
                                            unsigned (*test)(int, unsigned, unsigned) =
                                                (unsigned (*)(int, unsigned,
                                                              unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        int arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%d[0x%x], %u[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }
                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }
                                }
                                default:
                                    printf("Error: Unkown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        case FLOAT_AS_UNSIGNED_ARG:
                        case UNSIGNED_ARG:
                            switch (spec->arg_types[1]) {
                                case INT_ARG:
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            unsigned (*impl)(unsigned, int, int) =
                                                (unsigned (*)(unsigned, int, int)) spec->impl_func;
                                            unsigned (*test)(unsigned, int, int) =
                                                (unsigned (*)(unsigned, int, int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %d[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            unsigned (*impl)(unsigned, int, unsigned) =
                                                (unsigned (*)(unsigned, int,
                                                              unsigned)) spec->impl_func;
                                            unsigned (*test)(unsigned, int, unsigned) =
                                                (unsigned (*)(unsigned, int,
                                                              unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        int arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %d[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }

                                case FLOAT_AS_UNSIGNED_ARG:
                                case UNSIGNED_ARG: {
                                    switch (spec->arg_types[2]) {
                                        case INT_ARG: {
                                            unsigned (*impl)(unsigned, unsigned, int) =
                                                (unsigned (*)(unsigned, unsigned,
                                                              int)) spec->impl_func;
                                            unsigned (*test)(unsigned, unsigned, int) =
                                                (unsigned (*)(unsigned, unsigned,
                                                              int)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        int arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %u[0x%x], "
                                                                "%d[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }

                                        case FLOAT_AS_UNSIGNED_ARG:
                                        case UNSIGNED_ARG: {
                                            unsigned (*impl)(unsigned, unsigned, unsigned) =
                                                (unsigned (*)(unsigned, unsigned,
                                                              unsigned)) spec->impl_func;
                                            unsigned (*test)(unsigned, unsigned, unsigned) =
                                                (unsigned (*)(unsigned, unsigned,
                                                              unsigned)) spec->test_func;
                                            for (int i = 0; i < num_args[0]; i++) {
                                                for (int j = 0; j < num_args[1]; j++) {
                                                    for (int k = 0; k < num_args[2]; k++) {
                                                        unsigned arg1 = arg_test_vals[0][i];
                                                        unsigned arg2 = arg_test_vals[1][j];
                                                        unsigned arg3 = arg_test_vals[2][k];
                                                        unsigned actual = impl(arg1, arg2, arg3);
                                                        unsigned expected = test(arg1, arg2, arg3);
                                                        if (actual != expected) {
                                                            printf(
                                                                "ERROR Test %s(%u[0x%x], %u[0x%x], "
                                                                "%u[0x%x]) failed...\n...Gives "
                                                                "%u[0x%x]. Should be %u[0x%x]\n",
                                                                spec->name, arg1, arg1, arg2, arg2,
                                                                arg3, arg3, actual, actual,
                                                                expected, expected);
                                                            return -1;
                                                        }
                                                    }
                                                }
                                            }
                                            return 0;
                                        }
                                        default:
                                            printf("Unknown type for argument 3 of test '%s'\n",
                                                   spec->name);
                                            exit(1);
                                    }
                                }
                                default:
                                    printf("Error: Unkown type for argument 2 of test '%s'\n",
                                           spec->name);
                                    exit(1);
                            }

                        default:
                            printf("Error: Unknown type for argument 1 of test '%s'\n", spec->name);
                            exit(1);
                    }

                default:
                    printf("Unknown return type for test '%s'\n", spec->name);
                    exit(1);
            }

        default:
            printf(
                "Error: Test cases with more than 3 arguments are not "
                "supported\n");
            exit(1);
    }
}

/*
 * get_num_val - Extract hex/decimal/or float value from string
 * *valp must be initialized to 0
 */
static int get_num_val(char *sval, unsigned *valp) {
    char *endp;

    /* See if it's an integer or floating point */
    int ishex = 0;
    int isfloat = 0;
    for (int i = 0; sval[i]; i++) {
        switch (sval[i]) {
            case 'x':
            case 'X':
                ishex = 1;
                break;
            case 'e':
            case 'E':
                if (!ishex)
                    isfloat = 1;
                break;
            case '.':
                isfloat = 1;
                break;
            default:
                break;
        }
    }
    if (isfloat) {
        float fval = strtof(sval, &endp);
        if (!*endp) {
            *valp = *(unsigned *) &fval;
            return 1;
        }
        return 0;
    } else {
        long llval = strtol(sval, &endp, 0);
        long upperbits = llval >> 31;
        /* will give -1 for negative, 0 or 1 for positive */
        if (!*valp && (upperbits == 0 || upperbits == -1 || upperbits == 1)) {
            *valp = (unsigned) llval;
            return 1;
        }
        return 0;
    }
}

int main(int argc, char *argv[]) {
    char *puzzle_name = NULL;
    unsigned arg1 = 0;
    unsigned arg2 = 0;
    unsigned arg3 = 0;
    unsigned *args[] = {NULL, NULL, NULL};

    switch (argc) {
        case 5:
            if (get_num_val(argv[4], &arg3) != 0) {
                args[2] = &arg3;
            } else {
                printf("Invalid input for function argument 3: '%s'\n", argv[4]);
                exit(1);
            }
            // Fall through

        case 4:
            if (get_num_val(argv[3], &arg2) != 0) {
                args[1] = &arg2;
            } else {
                printf("Invalid input for function argument 2: '%s'\n", argv[3]);
                exit(1);
            }
            // Fall through

        case 3:
            if (get_num_val(argv[2], &arg1) != 0) {
                args[0] = &arg1;
            } else {
                printf("Invalid input for function argument 1: '%s'\n", argv[2]);
                exit(1);
            }
            // Fall through

        case 2:
            puzzle_name = argv[1];
            break;

        case 1:
            // Do Nothing
            break;

        default:
            printf("Usage: %s <func_name> [arg1] [arg2] [arg3]\n", argv[0]);
            exit(1);
    }

    if (puzzle_name != NULL) {
        // User has specified one puzzle to test
        puzzle_spec_t *current = puzzle_specs;
        while (current->name != NULL) {
            if (strcmp(current->name, puzzle_name) == 0) {
                test_function(current, args);
                return 0;
            }
            current++;
        }

        printf("Error: No puzzle with name '%s' found\n", puzzle_name);
    } else {
        puzzle_spec_t *current = puzzle_specs;
        while (current->name != NULL) {
            test_function(current, args);
            current++;
        }
    }
    return 0;
}
