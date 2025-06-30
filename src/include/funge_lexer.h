#ifndef FUNGE_LEXER_H
#define FUNGE_LEXER_H

#include <stdio.h>
#include <stddef.h>
#include "funge.h"

typedef enum e_bf93_op{
    BF3678_Decimal_Zero = '0',
    BF3678_Decimal_One = '1',
    BF3678_Decimal_Two = '2',
    BF3678_Decimal_Three = '3',
    BF3678_Decimal_Four = '4',
    BF3678_Decimal_Five = '5',
    BF3678_Decimal_Six = '6',
    BF3678_Decimal_Seven = '7',
    BF3678_Decimal_Eight = '8',
    BF3678_Decimal_Nine = '9',
    BF3678_Stack_Add = '+',
    BF3678_Stack_Subtract = '-',
    BF3678_Stack_Multiply = '*',
    BF3678_Stack_Divide = '/',
    BF3678_Stack_Modulo = '%',
    BF3678_Stack_Not = '!',
    BF3678_Stack_Greater = '`',
    BF3678_IP_Move_Right = '>',
    BF3678_IP_Move_Left = '<',
    BF3678_IP_Move_Up = '^',
    BF3678_IP_Move_Down = 'v',
    BF3678_IP_Move_Rand = '?',
    BF3678_IP_Move_Horiz = '_',
    BF3678_IP_Move_Vert = '|',
    BF3678_IP_Bridge = '#',
    BF3678_String_Endpoint = '\"',
    BF3678_Stack_Dup = ':',
    BF3678_Stack_Swap = '\\',
    BF3678_Stack_Pop = '$',
    BF3678_In_Int = '&',
    BF3678_In_ASCII = '~',
    BF3678_Out_Int = '.',
    BF3678_Out_ASCII = ',',
    BF3678_Reflect_Absolute_Get = 'g',
    BF3678_Reflect_Absolute_Put = 'p',
    BF3_End = '@'
}Befunge93_Op;

typedef enum e_bf96_op_sup_93{
    BF6_Directive = ';',
    BF678_Direct_Insert = 'I',
    BF678_Direct_Remap = 'R',
    BF678_Direct_Constant = 'C',
    BF678_Direct_Action = 'A',
    BF67_Holistic_Delta = 'h',
    BF678_Stack_Invert = 'i',
    BF678_OneShot_String = '\'',
    BF678_IP_Jump_Forward = 'j',
    BF678_IP_Rotate_Left = '[',
    BF678_IP_Rotate_Right = ']',
    BF67_IP_Split = 'T',
    BF678_IP_Terminate = '@',
    BF67_Reflect_Relative_Get = 'G',
    BF67_Reflect_Relative_Put = 'P',
    BF67_Reflect_IP_Left_Get = '(',
    BF67_Reflect_IP_Right_Get = ')',
    BF67_Reflect_IP_Left_Put = '{',
    BF67_Reflect_IP_Right_Put = '}'
}Befunge96_Op_Sup_93;

typedef enum e_funge97_op_sup_96{
    BF78_Hexadecimal_Ten = 'a',
    BF78_Hexadecimal_Eleven = 'b',
    BF78_Hexadecimal_Twelve = 'c',
    BF78_Hexadecimal_Thirteen = 'd',
    BF78_Hexadecimal_Fourteen = 'e',
    BF78_Hexadecimal_Fifteen = 'f',
    BF7_Script_Directive = '#',
    BF7_Interpreter_Directive = '=',
    BF7_Direct_Disable_Cmd = 'd',
    BF7_Direct_Require_Cmd = 'r',
    BF7_Direct_Language = 'l',
    BF7_Direct_Change_direct_Char = '=',
    BF7_Direct_Forward_Compatibility = '*',
    BF7_Direct_Orient = 'o',
    BF7_Direct_Include = 'i',
    BF7_IP_Move_3D_Up = 'U',
    BF7_IP_Move_3D_Down = 'D',
    BF7_IP_Move_3D_Conditional = 'H',
    BF78_IP_Jump_Over = ';',
    BF7_IP_Set_Ahead = 'J',
    BF7_IP_Set = 'Z',
    BF7_IP_Reverse = 'A',
    BF7_IP_Delta_Vector_Set = 'X',
    BF7_Push_IP_Delta = 'Y',
    BF78_IP_Rotate_Conditional = 'w',
    BF7_IP_Offset_Location_Reposition = 'R',
    BF78_Stack_Clear = 'n',
    BF7_Stack_Roll = 'r',
    BF7_Stack_Queuemode = 'q',
    BF78_Stack_Invertmode = 'i',
    BF7_Reflect_Instruction_Valid = 'V',
    BF7_Reflect_Internal_Flags = 'Q',
    BF7_Switchmode = 's',
    BF7_Syscall = 'y',
    BF7_File_Open = 'F',
    BF7_End = 'E'
}Funge97_Op_Sup_96;

typedef enum e_funge98_op_sup_97{
    BF8_NoOp = 'z',
    BF8_Stack_Transfer_SOSS_To_TOSS = 'u',
    BF8_Begin_Block = '{',
    BF8_End_Block = '}',
    BF8_IP_Move_3D_Up = 'h',
    BF8_IP_Move_3D_Down = 'l',
    BF8_IP_Move_3D_Conditional = 'm',
    BF8_IP_Reverse = 'r',
    BF8_IP_Delta_Vector_Set = 'x',
    BF8_Instruction_Iterate = 'k',
    BF8_Reflect_Store_Character = 's',
    BF8_Reflect_Execute_String = '=',
    BF8_Reflect_Semantics_Load = '(',
    BF8_Reflect_Semantics_Unload = ')',
    BF8_SysInfo = 'y',
    BF8_File_Input_Insert = 'i',
    BF8_File_Output_Write = 'o',
    BF8_End = 'q'
}Funge98_Op_Sup_97;

int tokenize_funge(FILE *fptr, FungeSpace *root, size_t exec_flags, size_t exec_dimensions);

#endif