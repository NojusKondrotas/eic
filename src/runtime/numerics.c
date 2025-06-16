#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/numerics.h"
#include "../include/whitespace_lexer.h"
#include "../include/dyn_array.h"
#include "../include/whitespace.h"

int parse_whitespace_number(DynArray *tokens, size_t *idx, ptrdiff_t *number){
    size_t c = *(size_t *)dyn_array_get(tokens, (*idx)++);
    int sign;
    *number = 0;

    if(c != LF_RAW){
        switch(c){
            case SPACE_RAW:
                sign = 1;
                break;
            case TAB_RAW:
                sign = -1;
                break;
            default:
                fprintf(stderr, "Unrecognised stack manipulation command\n");
                return EXIT_FAILURE;
        }
    }

    c = *(size_t *)dyn_array_get(tokens, (*idx)++);
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

        c = *(size_t *)dyn_array_get(tokens, (*idx)++);
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
    ptrdiff_t num_signed;
    size_t num_unsigned;
    void *ptr1, *ptr2;
    
    if(array->size < 2){
        fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
        return EXIT_FAILURE;
    }

    ptr1 = dyn_array_get(array, array->size - 2);
    if(check_if_null_ptr(ptr1)){
        return EXIT_FAILURE;
    }
    ptr2 = dyn_array_get(array, array->size - 1);
    if(check_if_null_ptr(ptr2)){
        return EXIT_FAILURE;
    }

    if(is_signed){
        switch(op){
            case '+': num_signed = (*(ptrdiff_t *)ptr1) + (*(ptrdiff_t *)ptr2); break;
            case '-': num_signed = (*(ptrdiff_t *)ptr1) - (*(ptrdiff_t *)ptr2); break;
            case '*': num_signed = (*(ptrdiff_t *)ptr1) * (*(ptrdiff_t *)ptr2); break;
            case '/':
                if((*(ptrdiff_t *)ptr2) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing an integer division operation\n");
                    return EXIT_FAILURE;
                }
                num_signed = (*(ptrdiff_t *)ptr1) / (*(ptrdiff_t *)ptr2);
                break;
            case '%':
                if((*(ptrdiff_t *)ptr2) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing a modulo operation\n");
                    return EXIT_FAILURE;
                }
                num_signed = (*(ptrdiff_t *)ptr1) % (*(ptrdiff_t *)ptr2);
                break;
            default:
                fprintf(stderr, "Unrecognised arithmetic operation (ASCII: %d)\n", op);
                return EXIT_FAILURE;
        }

        if(dyn_array_set(array, array->size - 1, &num_signed) == EXIT_FAILURE){
            return EXIT_FAILURE;
        }
        --array->size;
    }
    else{
        switch(op){
            case '+': num_unsigned = (*(size_t *)ptr1) + (*(size_t *)ptr2); break;
            case '-': num_unsigned = (*(size_t *)ptr1) - (*(size_t *)ptr2); break;
            case '*': num_unsigned = (*(size_t *)ptr1) * (*(size_t *)ptr2); break;
            case '/':
                if((*(size_t *)ptr2) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing an integer division operation\n");
                    return EXIT_FAILURE;
                }
                num_unsigned = (*(size_t *)ptr1) / (*(size_t *)ptr2);
                break;
            case '%':
                if((*(size_t *)ptr2) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing a modulo operation\n");
                    return EXIT_FAILURE;
                }
                num_unsigned = (*(size_t *)ptr1) % (*(size_t *)ptr2);
                break;
            default:
                fprintf(stderr, "Unrecognised arithmetic operation (ASCII: %d)\n", op);
                return EXIT_FAILURE;
        }

        if(dyn_array_set(array, array->size - 1, &num_unsigned) == EXIT_FAILURE){
            return EXIT_FAILURE;
        }
        --array->size;
    }

    return EXIT_SUCCESS;
}