#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "stack.h"

int size_t_push_token(size_t **tokens, size_t *tokens_cap, size_t *tokens_count, size_t token){
    if(*tokens_cap == *tokens_count){
        if(size_t_ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    (*tokens)[(*tokens_count)++] = token;
    
    return EXIT_SUCCESS;
}

int size_t_ensure_cap(size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    if(*tokens_cap == *tokens_count){
        (*tokens_cap) *= 2;
        size_t *tmp = realloc(*tokens, *tokens_cap * sizeof(size_t));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        *tokens = tmp;
    }

    return EXIT_SUCCESS;
}

int ptrdiff_t_push_token(ptrdiff_t **tokens, size_t *tokens_cap, size_t *tokens_count, size_t token){
    if(*tokens_cap == *tokens_count){
        if(ssize_t_ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    (*tokens)[(*tokens_count)++] = token;
    
    return EXIT_SUCCESS;
}

int ptrdiff_t_ensure_cap(ptrdiff_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    if(*tokens_cap == *tokens_count){
        (*tokens_cap) *= 2;
        ptrdiff_t *tmp = realloc(*tokens, *tokens_cap * sizeof(ptrdiff_t));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        *tokens = tmp;
    }

    return EXIT_SUCCESS;
}