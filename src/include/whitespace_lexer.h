#ifndef WHITESPACE_LEXER_H
#define WHITESPACE_LEXER_H

#include <stdio.h>
#include "dyn_array.h"

#define SPACE 32
#define TAB 9
#define LF 10

#define TOKENS_CAP 20

#define IO_TS 0x00 //Pop a heap address from the stack, read a character as ASCII, and store that character to that heap address
#define IO_TT 0x01 //Pop a heap address from the stack, read a number, and store that number to that heap address
#define IO_SS 0x02 //Pop a number from the stack and output it as an ASCII character
#define IO_ST 0x03 //Pop a number from the stack and output it

#define SM_S_n 0x10 //Push a number to the stack
#define SM_LS 0x11 //Duplicate the top item on the stack
#define SM_LT 0x12 //Swap the top two items on the stack
#define SM_LL 0x13 //Discard the top item on the stack
#define SM_TS_n 0x14 //Copy the nth item on the stack (given by the argument) onto the top of the stack (v0.3)
#define SM_TL_n 0x15 //Slide n items off the stack, keeping the top item (v0.3)

#define AR_SS 0x20 //Addition
#define AR_ST 0x21 //Subtraction
#define AR_SL 0x22 //Multiplication
#define AR_TS 0x23 //Integer Division
#define AR_TT 0x24 //Modulo

#define FC_SS_l 0x30 //Mark a location in the program
#define FC_ST_l 0x31 //Call a subroutine
#define FC_SL_l 0x32 //Jump unconditionally to a label
#define FC_TS_l 0x33 //Jump to a label if the top of the stack is zero
#define FC_TT_l 0x34 //Jump to a label if the top of the stack is negative
#define FC_TL 0x35 //End a subroutine and transfer control back to the caller
#define FC_LL 0x36 //Ends the program

#define HP_S 0x40 //Store
#define HP_T 0x41 //Retrieve

#define SPACE_RAW 0x50
#define TAB_RAW 0x51
#define LF_RAW 0x52


#define WS_KEY(a, b) (((unsigned char)(a) << 8) | (unsigned char)(b))

int tokenize_whitespace(FILE *, DynArray *, DynArray *);

#endif