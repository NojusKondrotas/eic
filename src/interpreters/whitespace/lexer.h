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

#define AR_SS 0x20
#define AR_ST 0x21
#define AR_SL 0x22
#define AR_TS 0x23
#define AR_TT 0x24

#define FC_SS_l 0x30
#define FC_ST_l 0x31
#define FC_Sl_l 0x32
#define FC_TS_l 0x33
#define FC_TT_l 0x34
#define FC_TL 0x35
#define FC_LL 0x36

#define HP_S 0x40
#define HP_T 0x41


#define WS_KEY(a, b) (((unsigned char)(a) << 8) | (unsigned char)(b))

size_t *tokenize_whitespace(FILE *, size_t *);

#endif