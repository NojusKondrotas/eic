#include <stdio.h>
#include <stdlib.h>
#include "../../include/dyn_array.h"
#include "../../include/funge.h"
#include "../../include/funge_lexer.h"

void *funge_space_init(size_t section_size){
    FungeSpace *space;
    space = (FungeSpace *)calloc(1, sizeof(FungeSpace));
    if(!space) return space;

    space->section = (ptrdiff_t *)calloc(section_size, sizeof(ptrdiff_t));
    if(!space->section) return NULL;
    space->right = NULL;
    space->left = NULL;
    space->up = NULL;
    space->down = NULL;
    
    return space;
}

//TO-DO: IMPLEMENT
void funge_space_connect(FungeSpace *root, FungeSpace *other, ptrdiff_t offset_x, ptrdiff_t offset_y){
    while(offset_x > 0){
        root = root->right;
        --offset_x;
    }
    while(offset_x < 0){
        root = root->left;
        ++offset_x;
    }
    while(offset_y > 0){
        root = root->down;
        --offset_y;
    }
    while(offset_y < 0){
        root = root->up;
        ++offset_y;
    }

    return root;
}

void funge_space_free(FungeSpace *space){
    if(!space) return;

    free(space->section);
    space->section = NULL;
    free(space);
    space = NULL;
}

void root_funge_space_free(FungeSpace *root){
    if(!root) return;

    FungeSpace *right = root->right, *left = root->left, *up = root->up , *down = root->down;
    funge_space_free(root);
    root_funge_space_free(right);
    root_funge_space_free(left);
    root_funge_space_free(up);
    root_funge_space_free(down);
}

void free_execution_resources_funge(FungeSpace *root, DynArray *stack){
    dyn_array_free(stack);
    root_funge_space_free(root);
}

int execute_funge_file(FILE* fptr, size_t exec_flags, size_t exec_dimensions){
    FungeSpace *root = NULL;
    DynArray stack;

    if(tokenize_funge(fptr, root, exec_flags, exec_dimensions) == EXIT_FAILURE){
        free_execution_resources_funge(root, &stack);
        return EXIT_FAILURE;
    }
}

int execute_funge(char *file_name, size_t exec_flags, size_t exec_dimensions){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_funge_file(fptr, exec_flags, exec_dimensions);

    fclose(fptr);

    return status;
}