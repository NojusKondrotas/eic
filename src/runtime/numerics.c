#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../interpreters/whitespace/whitespace.h"

ptrdiff_t parse_whitespace_number(FILE *fptr){
    int sign;
    ptrdiff_t number = 0;

    size_t c = fgetc(fptr);

    switch(c){
        case SPACE:
            sign = 1;
            break;
        case TAB:
            sign = -1;
            break;
        default:
            fprintf(stderr, "Unrecognised stack manipulation command\n");
            exit(EXIT_FAILURE);
    }

    while((c = fgetc(fptr)) != EOF){
        switch(c){
            case SPACE:
                number <<= 1;
                break;
            case TAB:
                number <<= 1;
                ++number;
                break;
            case LF:
                return number * sign;
            default:
                exit(EXIT_FAILURE);
        }
    }
    
    fprintf(stderr, "EOF encountered while parsing a number\n");
    exit(EXIT_FAILURE);
}