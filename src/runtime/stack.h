#ifndef STACK_H
#define STACK_H

#include <stdio.h>

typedef long ssize_t;

int size_t_push_token(size_t **, size_t *, size_t *, size_t token);

int size_t_ensure_cap(size_t **, size_t *, size_t *);

int ssize_t_push_token(ssize_t **, size_t *, size_t *, size_t token);

int ssize_t_ensure_cap(ssize_t **, size_t *, size_t *);

#endif