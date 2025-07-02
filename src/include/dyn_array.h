#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stdlib.h>

typedef struct s_dyn_array{
    void* data;
    size_t element_size;
    size_t capacity;
    size_t size;
}DynArray;

DynArray *dyn_array_init(size_t cap, size_t element_size);

void dyn_array_free(DynArray* array);

int dyn_array_resize(DynArray* array, size_t new_capacity);

int dyn_array_push_back(DynArray* array, const void* element);

int dyn_array_pop_back(DynArray *array, void *element);

int dyn_array_get(DynArray* array, size_t index, void *element);

int dyn_array_set(DynArray* array, size_t index, const void* element);

#endif