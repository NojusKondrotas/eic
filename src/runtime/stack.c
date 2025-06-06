#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/stack.h"

int push_unsigned(UnsignedStack *stack, size_t element){
    if(stack->capacity == stack->count){
        if(ensure_cap_unsigned(stack) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    stack->arr[stack->count++] = element;
    
    return EXIT_SUCCESS;
}

int ensure_cap_unsigned(UnsignedStack *stack){
    if(stack->capacity == stack->count){
        size_t new_capacity = stack->capacity * 2;
        size_t *tmp = realloc(stack->arr, new_capacity * sizeof(size_t));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        stack->arr = tmp;
        stack->capacity = new_capacity;
    }

    return EXIT_SUCCESS;
}

int push_signed(SignedStack *stack, ptrdiff_t element){
    if(stack->capacity == stack->count){
        if(ensure_cap_signed(stack) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    stack->arr[stack->count++] = element;
    
    return EXIT_SUCCESS;
}

int ensure_cap_signed(SignedStack *stack){
    if(stack->capacity == stack->count){
        size_t new_capacity = stack->capacity * 2;
        ptrdiff_t *tmp = realloc(stack->arr, new_capacity * sizeof(ptrdiff_t));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        stack->arr = tmp;
        stack->capacity = new_capacity;
    }

    return EXIT_SUCCESS;
}