#include <stdio.h>
#include <stdlib.h>
#include "../include/io.h"
#include "../include/dyn_array.h"
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

int tokenize_ws_raw(FILE *fptr, DynArray *array){
    size_t c = fgetc(fptr);
    size_t val;
    while(c != EOF){
        switch (c) {
            case SPACE:
                val = SPACE_RAW;
                if(dyn_array_push_back(array, &val) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;

            case TAB:
                val = TAB_RAW;
                if (dyn_array_push_back(array, &val) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                break;
            case LF:
                val = LF_RAW;
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

int read_in_char_ws(unsigned char *ch){
    int tmp = fgetc(stdin);
    
    if(tmp == EOF){
        fprintf(stderr, "Encountered unexpected end of stream while reading Whitespace console input\n");
        return EXIT_FAILURE;
    }

    while(fgetc(stdin) != LF);

    *ch = tmp;

    return EXIT_SUCCESS;
}

int read_in_number_ws(ptrdiff_t *num){
    int sign, tmp = fgetc(stdin);
    *num = 0;
    if(tmp == '-'){
        sign = -1;
        tmp = fgetc(stdin);
    }
    else if(tmp == '+'){
        sign = 1;
        tmp = fgetc(stdin);
    }
    else if(tmp > 47 && tmp < 58){
        sign = 1;
    }
    else{
        fprintf(stderr, "Unexpected character encountered while reading Whitespace input (ASCII: %d)\n", tmp);
        return EXIT_FAILURE;
    }
    
    while(tmp > 47 && tmp < 58){
        *num *= 10;
        *num += tmp - '0';

        tmp = fgetc(stdin);
        if(tmp == EOF){
            fprintf(stderr, "Encountered unexpected end of stream while reading Whitespace console input\n");
            return EXIT_FAILURE;
        }
    }

    while(fgetc(stdin) != LF);

    *num *= sign;
    return EXIT_SUCCESS;
}

//TODO: add error handling
int out_char_ws(unsigned char ch){
    fprintf(stdout, "%c", ch);

    return EXIT_SUCCESS;
}

//TODO: add error handling
int out_number_ws(ptrdiff_t number){
    fprintf(stdout, "%td", number);

    return EXIT_SUCCESS;
}