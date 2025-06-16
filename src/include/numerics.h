#ifndef NUMERICS_H
#define NUMERICS_H

#include <stdio.h>
#include <stddef.h>

typedef enum{ false, true } bool;

int parse_whitespace_number(DynArray *, size_t *, ptrdiff_t *);

bool check_array_equality_size_t(size_t *, size_t *, size_t);

bool check_if_null_ptr(const void *);

#endif