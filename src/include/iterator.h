#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdlib.h>

typedef struct{
    size_t index, count, capacity;
    size_t *elements;
}Iterator;

int next(Iterator *);

int iter_ctor_add(Iterator *, size_t);

int iter_ctor_ensure_cap(Iterator *);

#endif