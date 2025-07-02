#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dyn_array.h"

int in_brainfuck(unsigned char *val);

int read_whitespace_command_char(FILE *fptr, int *out_char);

int tokenize_whitespace_raw(FILE *fptr, DynArray *array);

DynArray *get_whitespace_label(DynArray *tokens, size_t *idx);

int in_char(unsigned char *ch);

int in_number(ptrdiff_t *num);

int out_char(unsigned char ch);

int out_number(ptrdiff_t number);

#endif