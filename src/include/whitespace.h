#ifndef WHITESPACE_H
#define WHITESPACE_H

#include <stdio.h>
#include <stdlib.h>
#include "dyn_array.h"
#include "numerics.h"
#include "whitespace_lexer.h"

typedef struct s_whitespace_label{
    DynArray *id;
    size_t instruction_index;
}Whitespace_Label;

void free_execution_resources_whitespace(DynArray *tokens, DynArray *stack, DynArray *heap, DynArray *labels, DynArray *call_stack, DynArray *tmp_label_array);

bool check_label_equality(DynArray *left_label, DynArray *right_label, size_t length);

bool match_whitespace_label(Whitespace_Label *label, DynArray *against_labels, size_t *matching_label_idx);

int jump_to_whitespace_label(DynArray *tokens, DynArray *labels, size_t *tokens_idx, size_t *idx, Whitespace_Label *tmp_label);

int execute_whitespace_file(FILE* fptr);

int execute_whitespace(char *file_name);

#endif