#ifndef WHITESPACE_H
#define WHITESPACE_H

#include <stdio.h>
#include <stdlib.h>
#include "dyn_array.h"
#include "numerics.h"

#define STACK_CAP 16
#define HEAP_CAP 16
#define LABELS_CAP 8

typedef struct{
    DynArray id;
    size_t instruction_index;
}Label;

void free_execution_resources_whitespace(DynArray *tokens, DynArray *stack, DynArray *heap, DynArray *labels, DynArray *call_stack, DynArray *tmp_label_array);

bool match_whitespace_label(Label *label, DynArray *against_labels, size_t *matching_label_idx);

int jump_to_whitespace_label(DynArray *tokens, DynArray *labels, size_t *tokens_idx, size_t *idx, Label *tmp_label);

int execute_whitespace_file(FILE* fptr);

int execute_whitespace(char *file_name);

#endif