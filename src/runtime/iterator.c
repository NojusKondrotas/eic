#include <stdio.h>
#include <stdlib.h>
#include "../include/iterator.h"

int next(Iterator *iterator){
    if(iterator->index < iterator->count)
        return 1;
    
    return 0;
}

int iter_ctor_add(Iterator *iterator, size_t element){
    if(iterator->capacity == iterator->count){
        if(iter_ctor_ensure_cap(iterator) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    iterator->elements[iterator->count++] = element;
    
    return EXIT_SUCCESS;
}

int iter_ctor_ensure_cap(Iterator *iterator){
    if(iterator->capacity == iterator->count){
        size_t new_capacity = iterator->capacity * 2;
        size_t *tmp = realloc(iterator->elements, new_capacity * sizeof(size_t));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        iterator->elements = tmp;
        iterator->capacity = new_capacity;
    }

    return EXIT_SUCCESS;
}