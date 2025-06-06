#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stddef.h>

typedef struct{
    size_t capacity, count;
    size_t *arr;
}UnsignedStack;

typedef struct{
    size_t capacity, count;
    ptrdiff_t *arr;
}SignedStack;

int push_unsigned(UnsignedStack *, size_t);

int ensure_cap_unsigned(UnsignedStack *);

int push_signed(SignedStack *, ptrdiff_t);

int ensure_cap_signed(SignedStack *);

#endif