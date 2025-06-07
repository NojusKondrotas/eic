#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/iterator.h"
#include "../include/lexer.h"
#include "../include/stack.h"
#include "../include/whitespace.h"

ptrdiff_t parse_whitespace_number(Iterator *tokens_iter, ptrdiff_t *number){
    unsigned int c;
    int sign;

    if(next(tokens_iter)){
        c = tokens_iter->elements[tokens_iter->index];
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

    while(next(tokens_iter) && (c = tokens_iter->elements[tokens_iter->index]) != EOF){
        switch(c){
            case SPACE:
                *number <<= 1;
                break;
            case TAB:
                *number <<= 1;
                ++number;
                break;
            case LF:
                *number *= sign;
                return EXIT_SUCCESS;
        }

        ++tokens_iter->index;
    }
    
    fprintf(stderr, "EOF encountered while parsing a number\n");
    return EXIT_FAILURE;
}