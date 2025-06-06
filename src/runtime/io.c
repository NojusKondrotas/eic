#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "stack.h"
#include "../interpreters/whitespace/whitespace.h"
#include "../interpreters/whitespace/lexer.h"

unsigned int read_in_bf(){
    char in_data[3] = {0};
    unsigned int num_val = 0;
    fscanf(stdin, "%3s", in_data);

    if(in_data[0] < 48 || in_data[0] > 57)
        return in_data[0];
    
    int i = 0;
    while(in_data[i] > 47 && in_data[i] < 58){
        num_val *= 10;
        num_val += in_data[i++] - 48;
    }

    return num_val;
}

int read_ws_command_char(FILE *fptr, size_t *out_char){
    do {
        *out_char = fgetc(fptr);
        if (*out_char == EOF) {
            fprintf(stderr, "Encountered unexpected EOF\n");
            return EXIT_FAILURE;
        }
    } while(*out_char != SPACE && *out_char != TAB && *out_char != LF);

    return EXIT_SUCCESS;
}

int tokenize_ws_raw(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c = fgetc(fptr);
    while(c != LF && c != EOF){
        switch (c) {
            case SPACE:
                if(size_t_push_token(tokens, tokens_cap, tokens_count, SPACE_RAW) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;

            case TAB:
                if (size_t_push_token(tokens, tokens_cap, tokens_count, TAB_RAW) == EXIT_FAILURE)
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