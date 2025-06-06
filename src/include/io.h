#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

int read_in_bf(unsigned char *);

int read_ws_command_char(FILE *, unsigned int *);

int tokenize_ws_raw(FILE *, unsigned int **, size_t *, size_t *);

#endif