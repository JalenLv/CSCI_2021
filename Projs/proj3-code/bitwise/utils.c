#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "dl_protocol.h"
#include "utils.h"

// Features code from Randy Bryant and Dave O'Halloran's original "Data Lab"

int get_num_val(char *sval, unsigned *valp) {
    char *endp;

    // See if it's an integer or floating point
    int ishex = 0;
    int isfloat = 0;
    for (int i = 0; sval[i] != '\0'; i++) {
        switch (sval[i]) {
            case 'x':
            case 'X':
                ishex = 1;
                break;
            case 'e':
            case 'E':
                if (!ishex) {
                    isfloat = 1;
                }
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
        long long int llval = strtoll(sval, &endp, 0);
        long long int upperbits = llval >> 31;
        /* will give -1 for negative, 0 or 1 for positive */
        if (!*valp && (upperbits == 0 || upperbits == -1 || upperbits == 1)) {
            *valp = (unsigned) llval;
            return 1;
        }
        return 0;
    }
}

// Get the number of arguments associated with each function
unsigned getNumArgs(enum function_id id) {
    switch (id) {
        case IS_ZERO:
        case DISTINCT_NEGATION:
        case IS_POSITIVE:
        case FLOAT_NEGATE: {
            return 1;
        }

        case BIT_NOR:
        case DIVIDE_POWER_2:
        case GET_BYTE:
        case IS_LESS_OR_EQUAL:
        case BIT_MASK:
        case ADD_OK: {
            return 2;
        }

        // Should never happen
        default: {
            return -1;
        }
    }
}

// Get name of a function from its ID value
const char *getFuncName(enum function_id id) {
    switch (id) {
        case IS_ZERO:
            return "isZero";
        case DISTINCT_NEGATION:
            return "distinctNegation";
        case IS_POSITIVE:
            return "isPositive";
        case FLOAT_NEGATE:
            return "floatNegate";
        case BIT_NOR:
            return "bitNor";
        case DIVIDE_POWER_2:
            return "dividePower2";
        case GET_BYTE:
            return "getByte";
        case IS_LESS_OR_EQUAL:
            return "isLessOrEqual";
        case BIT_MASK:
            return "bitMask";
        case ADD_OK:
            return "addOK";
        // Should never happen
        default:
            return NULL;
    }
}

enum function_id getFuncId(const char *name) {
    if (strcmp(name, "isZero") == 0) {
        return IS_ZERO;
    } else if (strcmp(name, "distinctNegation") == 0) {
        return  DISTINCT_NEGATION;
    } else if (strcmp(name, "isPositive") == 0) {
        return IS_POSITIVE;
    } else if (strcmp(name, "floatNegate") == 0) {
        return FLOAT_NEGATE;
    } else if (strcmp(name, "bitNor") == 0) {
        return BIT_NOR;
    } else if (strcmp(name, "dividePower2") == 0) {
        return DIVIDE_POWER_2;
    } else if (strcmp(name, "getByte") == 0) {
        return GET_BYTE;
    } else if (strcmp(name, "isLessOrEqual") == 0) {
        return IS_LESS_OR_EQUAL;
    } else if (strcmp(name, "bitMask") == 0) {
        return BIT_MASK;
    } else if (strcmp(name, "addOK") == 0) {
        return ADD_OK;
    }
    return 0;
}

// Get function's difficulty rating
unsigned getFuncRating(enum function_id id) {
    switch (id) {
        case IS_ZERO:
            return 1;
        case DISTINCT_NEGATION:
            return 2;
        case IS_POSITIVE:
            return 2;
        case FLOAT_NEGATE:
            return 2;
        case BIT_NOR:
            return 1;
        case DIVIDE_POWER_2:
            return 2;
        case GET_BYTE:
            return 2;
        case IS_LESS_OR_EQUAL:
            return 3;
        case BIT_MASK:
            return 3;
        case ADD_OK:
            return 3;
        // Should never happen
        default:
            return 0;
    }
}

int getFuncMinArg(enum function_id id, int arg_pos) {
    switch (arg_pos) {
        case 1: {
            switch (id) {
                case IS_ZERO:
                case BIT_NOR:
                case DISTINCT_NEGATION:
                case DIVIDE_POWER_2:
                case GET_BYTE:
                case IS_POSITIVE:
                case IS_LESS_OR_EQUAL:
                case ADD_OK:
                    return INT_MIN;

                case BIT_MASK:
                    return 0;

                default:
                    // Should never happen. All other puzzles are float-based
                    return -1;
            }
        }

        case 2:
            switch (id) {
                case BIT_NOR:
                case IS_LESS_OR_EQUAL:
                case ADD_OK:
                    return INT_MIN;

                case DIVIDE_POWER_2:
                case GET_BYTE:
                case BIT_MASK:
                    return 0;

                default:
                    // Should never happen. All other puzzles are float-based or require only 1 arg
                    return -1;
            }

        default:
            // Should never happen, all puzzles use 1 or 2 arguments
            return -1;
    }
}

int getFuncMaxArg(enum function_id id, int arg_pos) {
    switch (arg_pos) {
        case 1: {
            switch (id) {
                case IS_ZERO:
                case BIT_NOR:
                case DISTINCT_NEGATION:
                case DIVIDE_POWER_2:
                case GET_BYTE:
                case IS_POSITIVE:
                case IS_LESS_OR_EQUAL:
                case ADD_OK:
                    return INT_MAX;

                case BIT_MASK:
                    return 31;

                default:
                    // Should never happen. All other puzzles are float-based
                    return -1;
            }
        }

        case 2:
            switch (id) {
                case BIT_NOR:
                case IS_LESS_OR_EQUAL:
                case ADD_OK:
                    return INT_MAX;

                case DIVIDE_POWER_2:
                    return 30;

                case GET_BYTE:
                    return 3;

                case BIT_MASK:
                    return 31;

                default:
                    // Should never happen. All other puzzles are float-based or require only 1 arg
                    return -1;
            }

        default:
            // Should never happen, all puzzles use 1 or 2 arguments
            return -1;
    }
}
