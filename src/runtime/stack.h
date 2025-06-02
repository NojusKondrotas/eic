#ifndef STACK_H
#define STACK_H

#include <stdio.h>

typedef long ssize_t;

int int_push_token(int **, size_t *, size_t *, int token);

int int_ensure_cap(int **, size_t *, size_t *);

#endif