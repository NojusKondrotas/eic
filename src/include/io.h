#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

int read_in_bf(size_t *);

int read_ws_command_char(FILE *, size_t *);

int tokenize_ws_raw(FILE *, size_t **, size_t *, size_t *);

#endif