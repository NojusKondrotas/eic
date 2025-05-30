#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brainfuck.h"
#include "io.h"


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

int execute_bf_file(FILE *fptr){
    unsigned char *memory_tape = (unsigned char *)calloc(TAPE_LENGTH, sizeof(unsigned char));
    if(!memory_tape){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    size_t pc = 0;
    int memc = 0;
    int c;

    while((c = fgetc(fptr)) != EOF){
        switch(c){
            case OP_INC_PC:
                memc = (memc + 1) % TAPE_LENGTH;
                break;
            case OP_DEC_PC:
                memc = memc / UTIL_MAX_TAPE_WRAPPING * (TAPE_LENGTH - 1) + (1 - memc / UTIL_MAX_TAPE_WRAPPING) * memc;
                break;
            case OP_INC_VAL:
                --memory_tape[memc];
                break;
            case OP_DEC_VAL:
                --memory_tape[memc];
                break;
            case OP_IN:
                memory_tape[memc] = read_in_bf();
                break;
            case OP_OUT:
                putchar(memory_tape[memc]);
                break;
            case OP_JMP_FWD:
                //implement
                break;
            case OP_JMP_BCK:
                //implement
                break;
        }

        ++pc;
    }

    free(memory_tape);

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