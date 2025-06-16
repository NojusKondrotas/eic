#ifndef NUMERICS_H
#define NUMERICS_H

#include <stdio.h>
#include <stddef.h>
#include "dyn_array.h"

typedef enum{ false, true } bool;

int parse_whitespace_number(DynArray *, size_t *, ptrdiff_t *);

bool check_array_equality_size_t(size_t *, size_t *, size_t);

bool check_if_null_ptr(const void *);

int perform_arithmetic_on_top(DynArray *array, bool is_signed, unsigned char op);

#endif