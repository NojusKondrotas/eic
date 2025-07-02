#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dyn_array.h"

DynArray *dyn_array_init(size_t cap, size_t element_size){
    if(cap <= 0 || element_size <= 0) return NULL;

    DynArray *array = malloc(cap * element_size);
    
    if(!array) return NULL;

    array->data = calloc(cap, element_size);
    if(!array->data){
        dyn_array_free(array);
        return NULL;
    }

    array->element_size = element_size;
    array->capacity = cap;
    array->size = 0;
    return array;
}

void dyn_array_free(DynArray* array) {
    if(!array)
        return;

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
    ++array->size;
    return EXIT_SUCCESS;
}

int dyn_array_pop_back(DynArray *array, void *element){
    if(array->size == 0){
        fprintf(stderr, "Cannot perform a pop operation on an empty array\n");
        return EXIT_FAILURE;
    }

    memcpy(element, (char*)array->data + (array->size - 1) * array->element_size, array->element_size);
    --array->size;
    return EXIT_SUCCESS;
}

int dyn_array_get(DynArray* array, size_t index, void *element) {
    if (index >= array->capacity) {
        fprintf(stderr, "Index out of bounds\n");
        return EXIT_FAILURE;
    }
    
    memcpy(element, (char*)array->data + index * array->element_size, array->element_size);
    return EXIT_SUCCESS;
}

int dyn_array_set(DynArray* array, size_t index, const void* element) {
    if (index >= array->capacity) {
        fprintf(stderr, "Index out of bounds\n");
        return EXIT_FAILURE;
    }
    
    memcpy((char*)array->data + index * array->element_size, element, array->element_size);
    return EXIT_SUCCESS;
}