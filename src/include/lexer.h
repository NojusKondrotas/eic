#ifndef BEFUNGE93_LEXER_H
#define BEFUNGE93_LEXER_H

#include <stdio.h>

#define LF 10
#define SPACE 32

typedef enum{
    Add = '+',
    Substract = '-',
    Multiply = '*',
    Int_Divide = '/',
    Modulo = '%',
    Logical_Not = '!',
    Greater_Than = '`',
    Move_Left = '<',
    Move_Right = '>',
    Move_Up = '^',
    Move_Down = 'v',
    Move_Random = '?',
    Pop_Move_Horiz = '_',
    Pop_Move_Vert = '|',
    Bridge = '#',
    String_Endpoint = '\"',
    Duplicate = ':',
    Swap = '\\',
    Pop_Discard = '$',
    In_Int = '&',
    In_ASCII = '~',
    Out_Int = '.',
    Out_ASCII = ',',
    Put = 'p',
    Get = 'g',
    End = '@'
}Op;

int tokenize_befunge93(FILE *fptr, unsigned char *grid);

#endif