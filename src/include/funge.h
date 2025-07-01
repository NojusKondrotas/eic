#ifndef FUNGE_H
#define FUNGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dyn_array.h"
#include "eic.h"

#define UNEFUNGE_SPACE_LENGTH 64
#define FUNGE_SPACE_LENGTH 16

typedef struct s_funge_space{
    ptrdiff_t offset_x, offset_y;
    ptrdiff_t *section;
    struct s_funge_space *root, *up, *down, *left, *right;
}FungeSpace;

typedef struct s_funge_dimension_wrapper{
    ptrdiff_t offset;
    struct s_funge_dimension_wraper *high, *low, *dimension_higher, *dimension_lower;
    FungeSpace *section_2D_root;
}Funge_Dimension_Wrapper;

typedef struct s_funge_ip{
    FungeSpace *section;
    char *position;
    ptrdiff_t *delta;
    DynArray *stack_stack, fingerprints;
    FILE *filed_in, *filed_out;
    int modes;
}FungeIP;

void *funge_ip_init(FungeSpace *section, char *position, ptrdiff_t *delta, DynArray *stack_stack, DynArray *fingerprints, int modes);

void funge_ip_free(FungeIP *ip);

void *funge_space_init(size_t id_x, size_t id_y, FungeSpace *root, FungeSpace *up, FungeSpace *down, size_t section_size, FungeSpace *left, FungeSpace *right);

void funge_space_free(FungeSpace *space);

void free_execution_resources_funge(FungeSpace *origin_root, DynArray *stack, DynArray *IPs);

int execute_funge_file(FILE* fptr, FungeFlags exec_flags, int exec_dimensions);

int execute_funge(char *file_name, FungeFlags exec_flags, int exec_dimensions);

#endif