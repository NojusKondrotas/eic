#ifndef WHITESPACE_H
#define WHITESPACE_H

#include <stdlib.h>
#include "dyn_array.h"

#define STACK_CAP 16
#define HEAP_CAP 16
#define LABELS_CAP 8

typedef struct{
    DynArray id;
    size_t instruction_index;
}Label;

int execute_whitespace(char *);

#endif