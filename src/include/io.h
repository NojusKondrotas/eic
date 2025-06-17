#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dyn_array.h"

int in_brainfuck(unsigned char *val);

int read_whitespace_command_char(FILE *fptr, int *out_char);

int tokenize_whitespace_raw(FILE *fptr, DynArray *array);

int get_whitespace_label(DynArray *tokens, size_t *idx, DynArray *label);

int in_char_whitespace(unsigned char *ch);

int in_number_whitespace(ptrdiff_t *num);

int out_char_whitespace(unsigned char ch);

int out_number_whitespace(ptrdiff_t number);

#endif