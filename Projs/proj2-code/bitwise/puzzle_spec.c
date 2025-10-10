// SPDX-License-Identifier: GPL-3.0-or-later
#include "puzzle_spec.h"

#include <limits.h>
#include <stddef.h>

#include "bits.h"
#include "oracle.h"

puzzle_spec_t puzzle_specs[] = {

    {
        .name = "bitXor",
        .return_type = INT_RET,
        .num_args = 2,
        .arg_types = {INT_ARG, INT_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, INT_MIN, 0},
        .arg_max = {INT_MAX, INT_MAX, 0},
        .test_func = (int (*)(void)) test_bitXor,
        .impl_func = (int (*)(void)) bitXor,
    },
    {
        .name = "bitAnd",
        .return_type = INT_RET,
        .num_args = 2,
        .arg_types = {INT_ARG, INT_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, INT_MIN, 0},
        .arg_max = {INT_MAX, INT_MAX, 0},
        .test_func = (int (*)(void)) test_bitAnd,
        .impl_func = (int (*)(void)) bitAnd,
    },
    {
        .name = "allOddBits",
        .return_type = INT_RET,
        .num_args = 1,
        .arg_types = {INT_ARG, UNUSED_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, 0, 0},
        .arg_max = {INT_MAX, 0, 0},
        .test_func = (int (*)(void)) test_allOddBits,
        .impl_func = (int (*)(void)) allOddBits,
    },
    {
        .name = "floatIsEqual",
        .return_type = UNSIGNED_RET,
        .num_args = 2,
        .arg_types = {FLOAT_AS_UNSIGNED_ARG, FLOAT_AS_UNSIGNED_ARG, UNUSED_ARG},
        .arg_min = {0, 0, 0},
        .arg_max = {UINT_MAX, UINT_MAX, 0},
        .test_func = (int (*)(void)) test_floatIsEqual,
        .impl_func = (int (*)(void)) floatIsEqual,
    },
    {
        .name = "anyEvenBit",
        .return_type = INT_RET,
        .num_args = 1,
        .arg_types = {INT_ARG, UNUSED_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, 0, 0},
        .arg_max = {INT_MAX, 0, 0},
        .test_func = (int (*)(void)) test_anyEvenBit,
        .impl_func = (int (*)(void)) anyEvenBit,
    },
    {
        .name = "isPositive",
        .return_type = INT_RET,
        .num_args = 1,
        .arg_types = {INT_ARG, UNUSED_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, 0, 0},
        .arg_max = {INT_MAX, 0, 0},
        .test_func = (int (*)(void)) test_isPositive,
        .impl_func = (int (*)(void)) isPositive,
    },
    {
        .name = "replaceByte",
        .return_type = INT_RET,
        .num_args = 3,
        .arg_types = {INT_ARG, INT_ARG, INT_ARG},
        .arg_min = {INT_MIN, 0, 0},
        .arg_max = {INT_MAX, 3, 255},
        .test_func = (int (*)(void)) test_replaceByte,
        .impl_func = (int (*)(void)) replaceByte,
    },
    {
        .name = "isLess",
        .return_type = INT_RET,
        .num_args = 2,
        .arg_types = {INT_ARG, INT_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, INT_MIN, 0},
        .arg_max = {INT_MAX, INT_MAX, 0},
        .test_func = (int (*)(void)) test_isLess,
        .impl_func = (int (*)(void)) isLess,
    },
    {
        .name = "rotateLeft",
        .return_type = INT_RET,
        .num_args = 2,
        .arg_types = {INT_ARG, INT_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, 0, 0},
        .arg_max = {INT_MAX, 31, 0},
        .test_func = (int (*)(void)) test_rotateLeft,
        .impl_func = (int (*)(void)) rotateLeft,
    },
    {
        .name = "bitMask",
        .return_type = INT_RET,
        .num_args = 2,
        .arg_types = {INT_ARG, INT_ARG, UNUSED_ARG},
        .arg_min = {0, 0, 0},
        .arg_max = {31, 31, 0},
        .test_func = (int (*)(void)) test_bitMask,
        .impl_func = (int (*)(void)) bitMask,
    },
    {
        .name = "floatScale2",
        .return_type = UNSIGNED_RET,
        .num_args = 1,
        .arg_types = {FLOAT_AS_UNSIGNED_ARG, UNUSED_ARG, UNUSED_ARG},
        .arg_min = {0, 0, 0},
        .arg_max = {UINT_MAX, 0, 0},
        .test_func = (int (*)(void)) test_floatScale2,
        .impl_func = (int (*)(void)) floatScale2,
    },
    {
        .name = "isPower2",
        .return_type = INT_RET,
        .num_args = 1,
        .arg_types = {INT_ARG, UNUSED_ARG, UNUSED_ARG},
        .arg_min = {INT_MIN, 0, 0},
        .arg_max = {INT_MAX, 0, 0},
        .test_func = (int (*)(void)) test_isPower2,
        .impl_func = (int (*)(void)) isPower2,
    },
    // Sentinel value at end
    {
        .name = NULL,
    }

};
