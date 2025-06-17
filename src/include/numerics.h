#ifndef NUMERICS_H
#define NUMERICS_H

#include <stdio.h>
#include <stddef.h>
#include "dyn_array.h"

typedef enum{ false, true } bool;

int parse_whitespace_number(DynArray *, size_t *, ptrdiff_t *);

bool check_array_equality_size_t(size_t *, size_t *, size_t);

int stack_arithmetic_top(DynArray *array, bool is_signed, unsigned char op);

int stack_duplicate_element(DynArray *stack, size_t index);

int stack_swap_top(DynArray *stack);

#endif