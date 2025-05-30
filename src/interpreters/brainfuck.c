#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brainfuck.h"


typedef enum{
    OP_INC_PC = '>',
    OP_DEC_PC = '<',
    OP_INC_VAL = '+',
    OP_DEC_VAL = '-',
    OP_IN = ',',
    OP_OUT = '.',
    OP_JMP_FWD = '[',
    OP_JMP_BCK = ']'
}OP;

int execute_bf_file(FILE *ptr){
    return EXIT_SUCCESS;
}

int execute_bf(char *str){
    FILE *fptr;

    if((fptr = fopen(str, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", str);
        return EXIT_FAILURE;
    }

    int status = execute_bf_file(fptr);

    fclose(fptr);

    return status;
}