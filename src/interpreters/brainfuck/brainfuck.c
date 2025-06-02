#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brainfuck.h"
#include "../../runtime/io.h"


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

    size_t loop_depth = 0, loop_stack_cap = 10;
    int invalid_loop = 0, invalid_loop_start_depth = 0;;
    size_t *loop_stack = (size_t *)calloc(loop_stack_cap, sizeof(size_t));
    if(!loop_stack){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    size_t pc = 0;
    unsigned int memc = 0;
    int c;

    while((c = fgetc(fptr)) != EOF){
        if(c != OP_JMP_FWD && c != OP_JMP_BCK && invalid_loop){
            ++pc;
            continue;
        }

        if(loop_depth == loop_stack_cap){
            loop_stack_cap *= 2;
            size_t *new_data = (size_t *)realloc(loop_stack, loop_stack_cap * sizeof(size_t));
            loop_stack = new_data;
        }

        switch(c){
            case OP_INC_PC:
                memc = (memc + 1) % TAPE_LENGTH;
                break;
            case OP_DEC_PC:
                --memc;
                //handle wrapping back to (TAPE_LENGTH - 1) if 0xFFFFFFFF is reached
                memc = memc / UTIL_MAX_TAPE_WRAPPING * (TAPE_LENGTH - 1) + (1 - memc / UTIL_MAX_TAPE_WRAPPING) * memc;
                break;
            case OP_INC_VAL:
                ++memory_tape[memc];
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
                loop_stack[loop_depth++] = pc;

                if(memory_tape[memc] == 0){
                    invalid_loop = 1;
                    invalid_loop_start_depth = loop_depth;
                }

                break;
            case OP_JMP_BCK:
                if(loop_depth == 0){
                    return EXIT_FAILURE;
                }

                if(loop_depth == invalid_loop_start_depth){
                    invalid_loop = invalid_loop_start_depth = 0;
                }
                
                if(memory_tape[memc] != 0){
                    pc = loop_stack[--loop_depth] - 1;
                    fseek(fptr, pc + 1, SEEK_SET);
                }

                break;
        }

        ++pc;
    }

    free(memory_tape);
    free(loop_stack);

    return EXIT_SUCCESS;
}

int execute_bf(char *file_name){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_bf_file(fptr);

    fclose(fptr);

    return status;
}