#ifndef NUMERICS_H
#define NUMERICS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dyn_array.h"

#define TAB 9
#define LF 10
#define CARRIAGE 13
#define SPACE 32

#define SMALLEST_CONTAINER_CAP 8
#define SMALLER_CONTAINER_CAP 16
#define DEFAULT_CONTAINER_CAP 32
#define BIGGER_CONTAINER_CAP 64
#define LARGE_CONTAINER_CAP 128

typedef enum e_bool{ false, true } bool;

int parse_whitespace_number(DynArray *tokens, size_t *idx, ptrdiff_t *number);

void move_funge_ip(size_t ip[], size_t delta[], size_t caps[], size_t dimensions);

bool check_array_equality_size_t(size_t *left, size_t *right, size_t length);

int stack_arithmetic_top(DynArray *array, bool is_signed, unsigned char op);

int stack_duplicate_element(DynArray *stack, size_t index);

int stack_swap_top(DynArray *stack);

#endif