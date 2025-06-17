#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/numerics.h"
#include "../include/whitespace_lexer.h"
#include "../include/dyn_array.h"
#include "../include/whitespace.h"

int parse_whitespace_number(DynArray *tokens, size_t *idx, ptrdiff_t *number){
    size_t c;
    int sign = 0;
    *number = 0;

    if(dyn_array_get(tokens, (*idx)++, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;
    while(c != LF_RAW && !sign){
        switch(c){
            case SPACE_RAW:
                sign = 1;
                break;
            case TAB_RAW:
                sign = -1;
                break;
            default:
                if(dyn_array_get(tokens, (*idx)++, &c) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;
        }
    }

    if(!sign){
        fprintf(stderr, "Unexpected LF encountered while parsing Whitespace number's sign\n");
        return EXIT_FAILURE;
    }

    if(dyn_array_get(tokens, (*idx)++, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;
    while(c != LF_RAW){
        switch(c){
            case SPACE_RAW:
                *number <<= 1;
                break;
            case TAB_RAW:
                *number <<= 1;
                ++*number;
                break;
        }

        if(dyn_array_get(tokens, (*idx)++, &c) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    *number *= sign;
    return EXIT_SUCCESS;
}

bool check_array_equality_size_t(size_t *left, size_t *right, size_t length){
    for(size_t i = 0; i < length; ++i){
        // printf("first: %zX, second: %zX\n", left[i], right[i]);
        if (left[i] != right[i])
            return false;
    }

    return true;
}

bool check_if_null_ptr(const void *ptr){
    if(!ptr){
        fprintf(stderr, "A null pointer encountered, cannot continue\n");
        return true;
    }

    return false;
}

int perform_arithmetic_on_top(DynArray *array, bool is_signed, unsigned char op){
    ptrdiff_t operand_left_signed, operand_right_signed;
    size_t operand_left_unsigned, operand_right_unsigned;
    
    if(array->size < 2){
        fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
        return EXIT_FAILURE;
    }

    if(is_signed){
        if(dyn_array_pop_back(array, &operand_right_signed) == EXIT_FAILURE)
            return EXIT_FAILURE;
        if(dyn_array_pop_back(array, &operand_left_signed) == EXIT_FAILURE)
            return EXIT_FAILURE;

        switch(op){
            case '+': operand_left_signed = operand_left_signed + operand_right_signed; break;
            case '-': operand_left_signed = operand_left_signed - operand_right_signed; break;
            case '*': operand_left_signed = operand_left_signed * operand_right_signed; break;
            case '/':
                if(operand_right_signed == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing an integer division operation\n");
                    return EXIT_FAILURE;
                }
                operand_left_signed = operand_left_signed / operand_right_signed;
                break;
            case '%':
                if(operand_right_signed == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing a modulo operation\n");
                    return EXIT_FAILURE;
                }
                operand_left_signed = operand_left_signed % operand_right_signed;
                break;
            default:
                fprintf(stderr, "Unrecognised arithmetic operation (ASCII: %d)\n", op);
                return EXIT_FAILURE;
        }

        if(dyn_array_push_back(array, &operand_left_signed) == EXIT_FAILURE){
            return EXIT_FAILURE;
        }
    }
    else{
        if(dyn_array_pop_back(array, &operand_right_unsigned) == EXIT_FAILURE)
            return EXIT_FAILURE;
        if(dyn_array_pop_back(array, &operand_left_unsigned) == EXIT_FAILURE)
            return EXIT_FAILURE;

        switch(op){
            case '+': operand_left_unsigned = operand_left_unsigned + operand_right_unsigned; break;
            case '-': operand_left_unsigned = operand_left_unsigned - operand_right_unsigned; break;
            case '*': operand_left_unsigned = operand_left_unsigned * operand_right_unsigned; break;
            case '/':
                if(operand_right_unsigned == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing an integer division operation\n");
                    return EXIT_FAILURE;
                }
                operand_left_unsigned = operand_left_unsigned / operand_right_unsigned;
                break;
            case '%':
                if(operand_right_unsigned == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing a modulo operation\n");
                    return EXIT_FAILURE;
                }
                operand_left_unsigned = operand_left_unsigned % operand_right_unsigned;
                break;
            default:
                fprintf(stderr, "Unrecognised arithmetic operation (ASCII: %d)\n", op);
                return EXIT_FAILURE;
        }

        if(dyn_array_push_back(array, &operand_left_unsigned) == EXIT_FAILURE){
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}