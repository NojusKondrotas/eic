#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stddef.h>

int size_t_push_token(size_t **, size_t *, size_t *, size_t token);

int size_t_ensure_cap(size_t **, size_t *, size_t *);

int ptrdiff_t_push_token(ptrdiff_t **, size_t *, size_t *, size_t token);

int ptrdiff_t_ensure_cap(ptrdiff_t **, size_t *, size_t *);

#endif