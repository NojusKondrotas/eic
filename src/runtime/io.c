#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/dyn_array.h"
#include "../include/io.h"
#include "../include/whitespace.h"
#include "../include/whitespace_lexer.h"

int in_brainfuck(unsigned char *val){
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

int read_whitespace_command_char(FILE *fptr, int *out_char){
    do {
        *out_char = fgetc(fptr);
        if (*out_char == EOF) {
            fprintf(stderr, "Encountered unexpected EOF\n");
            return EXIT_FAILURE;
        }
    } while(*out_char != SPACE && *out_char != TAB && *out_char != LF);

    return EXIT_SUCCESS;
}

int tokenize_whitespace_raw(FILE *fptr, DynArray *array){
    ptrdiff_t c = fgetc(fptr);
    size_t val;
    while(c != EOF){
        switch (c) {
            case SPACE:
                val = WS_SPACE_RAW;
                if(dyn_array_push_back(array, &val) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;

            case TAB:
                val = WS_TAB_RAW;
                if (dyn_array_push_back(array, &val) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;
            case LF:
                val = WS_LF_RAW;
                return dyn_array_push_back(array, &val);
        }

        c = fgetc(fptr);
    }

    if(c == EOF){
        fprintf(stderr, "Encountered unexpected EOF\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int get_whitespace_label(DynArray *tokens, size_t *idx, DynArray *label){
    if(dyn_array_init(label, BIGGER_CONTAINER_CAP, sizeof(size_t)) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    size_t c;

    while(*idx < tokens->size){
        if(dyn_array_get(tokens, (*idx)++, &c) == EXIT_FAILURE)
            return EXIT_FAILURE;

        switch (c) {
            case WS_SPACE_RAW:
            case WS_TAB_RAW:
                if(dyn_array_push_back(label, &c) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;
            case WS_LF_RAW:
                if(dyn_array_push_back(label, &c) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "Unexpected token encountered when reading label post-lexing\n");
                dyn_array_free(label);
                return EXIT_FAILURE;
        }
    }

    fprintf(stderr, "Unexpected end of tokens while reading label\n");
    return EXIT_FAILURE;
}

int in_char(unsigned char *ch){
    int tmp = fgetc(stdin);
    
    if(tmp == EOF){
        fprintf(stderr, "Encountered unexpected end of stream while reading Whitespace console input\n");
        return EXIT_FAILURE;
    }

    while(fgetc(stdin) != LF);

    *ch = tmp;

    return EXIT_SUCCESS;
}

int in_number(ptrdiff_t *num){
    int sign, tmp = fgetc(stdin);
    *num = 0;
    if(tmp == '-'){
        sign = -1;
    }
    else if(tmp == '+'){
        sign = 1;
    }
    else if(tmp > 47 && tmp < 58){
        *num = tmp - '0';
        sign = 1;
    }
    else{
        fprintf(stderr, "Unexpected character encountered while reading user input (ASCII: %d)\n", tmp);
        return EXIT_FAILURE;
    }
    
    tmp = fgetc(stdin);
    while(tmp > 47 && tmp < 58){
        *num *= 10;
        *num += tmp - '0';

        tmp = fgetc(stdin);
        if(tmp == EOF){
            fprintf(stderr, "Encountered unexpected end of stream while reading user input\n");
            return EXIT_FAILURE;
        }
    }

    *num *= sign;
    return EXIT_SUCCESS;
}

int out_char(unsigned char ch){
    if(fprintf(stdout, "%c", ch) < 0){
        fprintf(stderr, "Error writing to stdout\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int out_number(ptrdiff_t number){
    if(fprintf(stdout, "%td", number) < 0){
        fprintf(stderr, "Error writing to stdout\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}