#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../include/stack.h"

int size_t_push_token(unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count, unsigned int token){
    if(*tokens_cap == *tokens_count){
        if(size_t_ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    (*tokens)[(*tokens_count)++] = token;
    
    return EXIT_SUCCESS;
}

int size_t_ensure_cap(unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count){
    if(*tokens_cap == *tokens_count){
        (*tokens_cap) *= 2;
        unsigned int *tmp = realloc(*tokens, *tokens_cap * sizeof(unsigned int));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        *tokens = tmp;
    }

    return EXIT_SUCCESS;
}