#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dyn_array.h"

int read_in_bf(unsigned char *);

int read_ws_command_char(FILE *, int *);

int tokenize_ws_raw(FILE *, DynArray *);

int get_ws_label(DynArray *, size_t *, DynArray *);

int read_in_char_ws(unsigned char *);

int read_in_number_ws(ptrdiff_t *);

int out_char_ws(unsigned char);

int out_number_ws(ptrdiff_t);

#endif