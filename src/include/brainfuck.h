#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include <stdio.h>

#define TAPE_LENGTH 30000

typedef enum e_brainfuck_op{
    BF_INC_PC = '>',
    BF_DEC_PC = '<',
    BF_INC_VAL = '+',
    BF_DEC_VAL = '-',
    BF_IN = ',',
    BF_OUT = '.',
    BF_JMP_FWD = '[',
    BF_JMP_BCK = ']'
}Brainfuck_Op;

int execute_brainfuck_file(FILE *fptr);

int execute_brainfuck(char *file_name);

#endif