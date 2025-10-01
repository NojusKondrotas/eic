#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/dyn_array.h"
#include "../include/numerics.h"
#include "../include/whitespace_lexer.h"
#include "../include/whitespace.h"

int parse_whitespace_number(DynArray *tokens, size_t *idx, ptrdiff_t *number){
    size_t c;
    int sign = 0;
    *number = 0;

    if(dyn_array_get(tokens, (*idx)++, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;
    while(c != WS_LF_RAW && !sign){
        switch(c){
            case WS_SPACE_RAW:
                sign = 1;
                break;
            case WS_TAB_RAW:
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
    while(c != WS_LF_RAW){
        switch(c){
            case WS_SPACE_RAW:
                *number <<= 1;
                break;
            case WS_TAB_RAW:
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

void move_funge_ip(size_t ip[], size_t delta[], size_t caps[], size_t dimensions){
    for(size_t i = 0; i < dimensions; ++i){
        ip[i] = (ip[i] + delta[i] + caps[i]) % caps[i];
    }
}

int stack_arithmetic_top(DynArray *array, bool is_signed, unsigned char op){
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

int stack_duplicate_element(DynArray *stack, size_t index){
    void *tmp = NULL;
    if(stack->size == 0){
        fprintf(stderr, "Stack size cannot be 0 when duplicating its %zunth item\n", index);
        return EXIT_FAILURE;
    }

    if(dyn_array_get(stack, index, &tmp) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    return dyn_array_push_back(stack, &tmp);
}

int stack_swap_top(DynArray *stack){
    void *first_pop = NULL, *second_pop = NULL;
    if(stack->size < 2){
        fprintf(stderr, "Stack cannot have less than two items when performing a swap of its top items\n");
        return EXIT_FAILURE;
    }

    if(dyn_array_pop_back(stack, &first_pop) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(dyn_array_pop_back(stack, &second_pop) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(dyn_array_push_back(stack, &first_pop) == EXIT_FAILURE)
        return EXIT_FAILURE;
    return dyn_array_push_back(stack, &second_pop);
}

int stack_invert_top(DynArray *stack, bool is_signed){
    if(stack->size == 0){
        fprintf(stderr, "Stack size cannot be 0 when inverting its top item\n");
        return EXIT_FAILURE;
    }
    ptrdiff_t tmp_signed;
    size_t tmp_unsigned;

    if(is_signed){
        if(dyn_array_pop_back(stack, &tmp_signed) == EXIT_FAILURE)
            return EXIT_FAILURE;

        if(!tmp_signed) tmp_signed = 1;
        else tmp_signed = 0;

        return dyn_array_push_back(stack, &tmp_signed);
    }
    else{
        if(dyn_array_pop_back(stack, &tmp_unsigned) == EXIT_FAILURE)
            return EXIT_FAILURE;

        if(!tmp_unsigned) tmp_unsigned = 1;
        else tmp_unsigned = 0;

        return dyn_array_push_back(stack, &tmp_unsigned);
    }
}

int stack_compare(DynArray *stack, bool is_signed){
    if(stack->size < 2){
        fprintf(stderr, "Stack size cannot be less than 2 when comparing its two top items\n");
        return EXIT_FAILURE;
    }
    ptrdiff_t signed_left, signed_right;
    size_t unsigned_left, unsigned_right;

    if(is_signed){
        if(dyn_array_pop_back(stack, &signed_right) == EXIT_FAILURE)
            return EXIT_FAILURE;
        if(dyn_array_pop_back(stack, &signed_left) == EXIT_FAILURE)
            return EXIT_FAILURE;

        if(signed_left > signed_right) signed_left = 1;
        else signed_left = 0;

        return dyn_array_push_back(stack, &signed_left);
    }
    else{
        if(dyn_array_pop_back(stack, &unsigned_right) == EXIT_FAILURE)
            return EXIT_FAILURE;
        if(dyn_array_pop_back(stack, &unsigned_left) == EXIT_FAILURE)
            return EXIT_FAILURE;

        if(unsigned_left > unsigned_right) unsigned_left = 1;
        else unsigned_left = 0;

        return dyn_array_push_back(stack, &unsigned_left);
    }
}