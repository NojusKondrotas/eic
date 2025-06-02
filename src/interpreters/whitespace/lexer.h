#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define TOKENS_CAP 20

#define IO_TS 0x00
#define IO_TT 0x01
#define IO_SS 0x02
#define IO_ST 0x03

#define SM_S_n 0x10
#define SM_LS 0x11
#define SM_LT 0x12
#define SM_LL 0x13
#define SM_TS_n 0x14
#define SM_TL_n 0x15

#define A_SS 0x20
#define A_ST 0x21
#define A_SL 0x22
#define A_TS 0x23
#define A_TT 0x24

#define FC_SS_l 0x30
#define FC_ST_l 0x31
#define FC_Sl_l 0x32
#define FC_TS_l 0x33
#define FC_TT_l 0x34
#define FC_TL 0x35
#define FC_LL 0x36

#define HP_S 0x40
#define HP_T 0x41


char *tokenize_whitespace(FILE *);

#endif