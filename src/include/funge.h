#ifndef FUNGE_H
#define FUNGE_H

#include <stdio.h>
#include "dyn_array.h"

#define UNEFUNGE_SPACE_WIDTH 64
#define FUNGE_SPACE_HEIGHT 16
#define FUNGE_SPACE_WIDTH 16

// funge-space
// ip

typedef struct s_funge_space{
    ptrdiff_t *section;
    FungeSpace *left, *right, *up, *down;
}FungeSpace;

typedef struct s_funge_ip{
    FungeSpace *section;
    char x, y;
}FungeIP;

void *funge_space_init(size_t section_size);

void funge_space_connect(FungeSpace *root, FungeSpace *other, ptrdiff_t offset_x, ptrdiff_t offset_y);

void funge_space_free(FungeSpace *space);

void root_funge_space_free(FungeSpace *root);

void free_execution_resources_funge(FungeSpace *root, DynArray *stack);

int execute_funge_file(FILE* fptr, size_t exec_flags, size_t exec_dimensions);

int execute_funge(char *file_name, size_t exec_flags, size_t exec_dimensions);

#endif