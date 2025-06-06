#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stddef.h>

int size_t_push_token(unsigned int **, size_t *, size_t *, unsigned int token);

int size_t_ensure_cap(unsigned int **, size_t *, size_t *);

#endif