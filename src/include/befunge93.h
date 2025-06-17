#ifndef BEFUNGE93_H
#define BEFUNGE93_H

#include <stdio.h>
#include <stdlib.h>
#include "dyn_array.h"

#define STACK_CAP 16

#define GRID_HORIZONTAL_CAP 80
#define GRID_VERTICAL_CAP 25

void free_execution_resources_befunge93(unsigned char *grid, DynArray *stack);

int move_instruction_pointer(size_t *horizontal_index, size_t *vertical_index, unsigned char c);

int execute_befunge93_file(FILE* fptr);

int execute_befunge93(char *file_name);

#endif