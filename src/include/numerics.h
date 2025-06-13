#ifndef NUMERICS_H
#define NUMERICS_H

#include <stdio.h>
#include <stddef.h>

int parse_whitespace_number(DynArray *, size_t *, ptrdiff_t *);

int check_array_equality_size_t(size_t *, size_t *, size_t);

#endif