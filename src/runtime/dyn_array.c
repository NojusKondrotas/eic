#include "../include/dyn_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dyn_array_init(DynArray* array, size_t cap, size_t element_size) {
    array->data = calloc(cap, element_size);
    if(!array->data && cap > 0){
        fprintf(stderr, "Failure allocating memory\n");
        dyn_array_free(array);
        return EXIT_FAILURE;
    }

    array->element_size = element_size;
    array->capacity = cap;
    array->size = 0;
    return EXIT_SUCCESS;
}

void dyn_array_free(DynArray* array) {
    free(array->data);
    array->data = NULL;
    array->capacity = 0;
    array->size = 0;
}

int dyn_array_resize(DynArray* array, size_t new_capacity) {
    void* new_data = realloc(array->data, new_capacity * array->element_size);
    if (!new_data) {
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    array->data = new_data;
    array->capacity = new_capacity;
    return EXIT_SUCCESS;
}

int dyn_array_push_back(DynArray* array, const void* element) {
    if (array->size == array->capacity) {
        size_t new_capacity = array->capacity > 0 ? array->capacity * 2 : 1;
        if(dyn_array_resize(array, new_capacity) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    memcpy((char*)array->data + array->size * array->element_size, element, array->element_size);
    array->size++;
    return EXIT_SUCCESS;
}

int dyn_array_pop_back(DynArray* array) {
    if (array->size == 0) {
        return EXIT_FAILURE;
    }

    array->size--;
    return EXIT_SUCCESS;
}

void* dyn_array_get(DynArray* array, size_t index) {
    if (index >= array->size) {
        return NULL;
    }
    
    return (char*)array->data + index * array->element_size;
}

int dyn_array_set(DynArray* array, size_t index, const void* element) {
    if (index >= array->size) {
        return EXIT_FAILURE;
    }
    
    memcpy((char*)array->data + index * array->element_size, element, array->element_size);
    return EXIT_SUCCESS;
}