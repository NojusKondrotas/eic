#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/lexer.h"
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

int check_array_equality_size_t(size_t *left, size_t *right, size_t length){
    for(size_t i = 0; i < length; ++i){
        // printf("first: %zX, second: %zX\n", left[i], right[i]);
        if (left[i] != right[i])
            return 0;
    }

    return 1;
}