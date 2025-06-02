#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define TOKENS_CAP 20

#define IO_TS 0x0
#define IO_TT 0x1
#define IO_SS 0x2
#define IO_ST 0x3

#define SM_S_n 1x0
#define SM_LS 1x1
#define SM_LT 1x2
#define SM_LL 1x3
#define SM_TS_n 1x4
#define SM_TL_n 1x5

#define A_SS 2x0
#define A_ST 2x1
#define A_SL 2x2
#define A_TS 2x3
#define A_TT 2x4

#define FC_SS_l 3x0
#define FC_ST_l 3x1
#define FC_Sl_l 3x2
#define FC_TS_l 3x3
#define FC_TT_l 3x4
#define FC_TL 3x5
#define FC_LL 3x6

#define HP_S 4x0
#define HP_T 4x1


char *tokenize_whitespace(FILE *);

#endif