#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#define TAPE_LENGTH 30000

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

int execute_bf(char *);

#endif