#ifndef WHITESPACE_LEXER_H
#define WHITESPACE_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include "dyn_array.h"
#include "numerics.h"

typedef enum e_whitespace_op{
    WS_IO_TS = 0x00, //Pop a heap address from the stack, read a character as ASCII, and store that character to that heap address
    WS_IO_TT = 0x01, //Pop a heap address from the stack, read a number, and store that number to that heap address
    WS_IO_SS = 0x02, //Pop a number from the stack and output it as an ASCII character
    WS_IO_ST = 0x03, //Pop a number from the stack and output it

    WS_SM_S_n = 0x10, //Push a number to the stack
    WS_SM_LS = 0x11, //Duplicate the top item on the stack
    WS_SM_LT = 0x12, //Swap the top two items on the stack
    WS_SM_LL = 0x13, //Discard the top item on the stack
    WS_SM_TS_n = 0x14, //Copy the nth item on the stack (given by the argument) onto the top of the stack (v0.3)
    WS_SM_TL_n = 0x15, //Slide n items off the stack, keeping the top item (v0.3)

    WS_AR_SS = 0x20, //Addition
    WS_AR_ST = 0x21, //Subtraction
    WS_AR_SL = 0x22, //Multiplication
    WS_AR_TS = 0x23, //Integer Division
    WS_AR_TT = 0x24, //Modulo

    WS_FC_SS_l = 0x30, //Mark a location in the program
    WS_FC_ST_l = 0x31, //Call a subroutine
    WS_FC_SL_l = 0x32, //Jump unconditionally to a label
    WS_FC_TS_l = 0x33, //Jump to a label if the top of the stack is zero
    WS_FC_TT_l = 0x34, //Jump to a label if the top of the stack is negative
    WS_FC_TL = 0x35, //End a subroutine and transfer control back to the caller
    WS_FC_LL = 0x36, //Ends the program

    WS_HP_S = 0x40, //Store
    WS_HP_T = 0x41, //Retrieve

    WS_SPACE_RAW = 0x50,
    WS_TAB_RAW = 0x51,
    WS_LF_RAW = 0x52
}Whitespace_Op;


#define WS_KEY(a, b) (((unsigned char)(a) << 8) | (unsigned char)(b))

int create_whitespace_label(FILE *fptr, size_t instr_idx, DynArray *labels);

int tokenize_io(FILE *fptr, DynArray *tokens);

int tokenize_arithmetic(FILE *fptr, DynArray *tokens);

int tokenize_heap(FILE *fptr, DynArray *tokens);

int tokenize_stack_manip(FILE *fptr, DynArray *tokens);

int tokenize_flow_control(FILE *fptr, DynArray *tokens, DynArray *labels);

int tokenize_whitespace(FILE *fptr, DynArray **tokens, DynArray **labels);

#endif