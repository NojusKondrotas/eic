#include <stdio.h>
#include <stdlib.h>
#include "../include/io.h"
#include "../include/stack.h"
#include "../include/whitespace.h"
#include "../include/lexer.h"

int read_in_bf(unsigned char *val){
    char in_data[4] = {0};
    *val = 0;
    if(fscanf(stdin, "%3s", in_data) != 1){
        fprintf(stderr, "Reading from stdin was unsuccessful\n");
        return EXIT_FAILURE;
    }

    if(in_data[0] < 48 || in_data[0] > 57){
        *val = (unsigned char)in_data[0];
        return EXIT_SUCCESS;
    }
    
    size_t i = 0;
    while(in_data[i] > 47 && in_data[i] < 58){
        *val *= 10;
        *val += in_data[i++] - 48;
    }

    return EXIT_SUCCESS;
}

int read_ws_command_char(FILE *fptr, unsigned int *out_char){
    do {
        *out_char = fgetc(fptr);
        if (*out_char == EOF) {
            fprintf(stderr, "Encountered unexpected EOF\n");
            return EXIT_FAILURE;
        }
    } while(*out_char != SPACE && *out_char != TAB && *out_char != LF);

    return EXIT_SUCCESS;
}

int tokenize_ws_raw(FILE *fptr, UnsignedStack *tokens){
    size_t c = fgetc(fptr);
    while(c != LF && c != EOF){
        switch (c) {
            case SPACE:
                if(push_unsigned(tokens, SPACE_RAW) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;

            case TAB:
                if (push_unsigned(tokens, TAB_RAW) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;
        }

        c = fgetc(fptr);
    }

    if(c == EOF){
        fprintf(stderr, "Encountered unexpected EOF\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}