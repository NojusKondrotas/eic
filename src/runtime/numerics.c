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