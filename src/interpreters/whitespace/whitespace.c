#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/dyn_array.h"
#include "../../include/io.h"
#include "../../include/numerics.h"
#include "../../include/whitespace.h"
#include "../../include/whitespace_lexer.h"

void free_execution_resources_whitespace(DynArray *tokens, DynArray *stack, DynArray *heap, DynArray *labels, DynArray *call_stack, DynArray *tmp_label_array){
    dyn_array_free(tokens);
    dyn_array_free(stack);
    dyn_array_free(heap);
    dyn_array_free(labels);
    dyn_array_free(call_stack);
    dyn_array_free(tmp_label_array);
}

bool check_label_equality(DynArray *left_label, DynArray *right_label, size_t length){
    Whitespace_Op left_op, right_op;
    for(size_t i = 0; i < length; ++i){
        if(dyn_array_get(left_label, i, &left_op) == EXIT_FAILURE
        || dyn_array_get(right_label, i, &right_op) == EXIT_FAILURE){
            return false;
        }
        //printf("first: %X, second: %X\n", left_op, right_op);
        if (left_op != right_op)
            return false;
    }

    return true;
}

bool match_whitespace_label(Whitespace_Label *label, DynArray *against_labels, size_t *matching_label_idx){
    size_t idx = 0;
    while(idx < against_labels->size){
        Whitespace_Label candidate;
        if(dyn_array_get(against_labels, idx, &candidate) == EXIT_FAILURE)
            return EXIT_FAILURE;
        // printf("label length: %zu, %zu\n", label->id.size, candidate->id.size);

        if(label->id->size != candidate.id->size){
            ++idx;
            continue;
        }
        
        if(check_label_equality(label->id, candidate.id, label->id->size)){
            *matching_label_idx = idx;
            return true;
        }

        ++idx;
    }
    
    return false;
}

int jump_to_whitespace_label(DynArray *tokens, DynArray *labels, size_t *tokens_idx, size_t *idx, Whitespace_Label *tmp_label){
    tmp_label->id = get_whitespace_label(tokens, tokens_idx);
    if(!tmp_label->id){
        return EXIT_FAILURE;
    }
    
    if(match_whitespace_label(tmp_label, labels, idx)){
        Whitespace_Label tmp;
        if(dyn_array_get(labels, *idx, &tmp) == EXIT_FAILURE)
            return EXIT_FAILURE;

        *tokens_idx = tmp.instruction_index + 1;
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "Found no matching label when attempting jump\n");
        return EXIT_FAILURE;
    }
}

int execute_whitespace_file(FILE* fptr){
    DynArray *tokens = NULL;
    DynArray *stack = NULL;
    DynArray *heap = NULL;
    DynArray *labels = NULL;
    DynArray *call_stack = NULL;

    // Execution variables
    size_t tokens_idx = 0;
    Whitespace_Op cmd;
    // Utility variables
    size_t idx, heap_addr, new_cap;
    ptrdiff_t num;
    Whitespace_Label tmp_label;
    unsigned char ch;

    if(tokenize_whitespace(fptr, &tokens, &labels) == EXIT_FAILURE)
        return EXIT_FAILURE;

    stack = (DynArray *)dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(ptrdiff_t));
    heap = (DynArray *)dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(ptrdiff_t));
    call_stack = (DynArray *)dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(size_t));
    tmp_label.id = (DynArray *)dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(Whitespace_Op));

    if(!stack || !heap || !call_stack || !tmp_label.id){
        fprintf(stderr, "Failure allocating memory\n");
        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
        return EXIT_FAILURE;
    }

    while(tokens_idx < tokens->size){
        if(dyn_array_get(tokens, tokens_idx++, &cmd) == EXIT_FAILURE){
            free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
            return EXIT_FAILURE;
        }
        //printf("tokne: %x\n", cmd);
        //printf("curr: %zu, size: %zu\n", tokens_idx, tokens.size);
        
        switch(cmd){
            // Handle IO command
            case WS_IO_TS:
                if(stack->size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_pop_back(stack, &heap_addr) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(in_char(&ch) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap->capacity){
                    new_cap = heap->capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap->capacity = new_cap;

                    if(dyn_array_resize(heap, heap->capacity) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(heap, heap_addr, &ch) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                break;
            case WS_IO_TT:
                if(stack->size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_pop_back(stack, &heap_addr) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(in_number(&num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap->capacity){
                    new_cap = heap->capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap->capacity = new_cap;

                    if(dyn_array_resize(heap, heap->capacity) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(heap, heap_addr, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_IO_SS:
                if(stack->size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                if(dyn_array_pop_back(stack, &ch) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(out_char(ch) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;
            case WS_IO_ST:
                if(stack->size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_pop_back(stack, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(out_number(num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;

            // Handle Stack Manipulation command
            case WS_SM_S_n:
                if(parse_whitespace_number(tokens, &tokens_idx, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_push_back(stack, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                break;
            case WS_SM_LS:
                if(stack_duplicate_element(stack, stack->size - 1) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                 
                break;
            case WS_SM_LT:
                if(stack_swap_top(stack) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_SM_LL:
                if(stack->size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                --stack->size;
                    
                break;
            case WS_SM_TS_n:
                if(parse_whitespace_number(tokens, &tokens_idx, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(num >= 0 && num >= stack->size){
                    fprintf(stderr, "Stack index out of bounds\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(stack_duplicate_element(stack, stack->size - 1 - num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_SM_TL_n:
                if(parse_whitespace_number(tokens, &tokens_idx, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(num >= 0 && num >= stack->size){
                    fprintf(stderr, "Given argument cannot be more than or equal to stack's item count when performing stack sliding\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                stack->size -= num;
                    
                break;

            // Handle Arithmetic command
            case WS_AR_SS:
                if(stack_arithmetic_top(stack, true, '+') == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_AR_ST:
                if(stack_arithmetic_top(stack, true, '-') == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_AR_SL:
                if(stack_arithmetic_top(stack, true, '*') == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_AR_TS:
                if(stack_arithmetic_top(stack, true, '/') == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_AR_TT:
                if(stack_arithmetic_top(stack, true, '%') == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;

            // Handle Flow Control command
            case WS_FC_SS_l:
                break;
            case WS_FC_ST_l:
                if(dyn_array_push_back(call_stack, &tokens_idx) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(jump_to_whitespace_label(tokens, labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_FC_SL_l:
                if(jump_to_whitespace_label(tokens, labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case WS_FC_TS_l:
                if(dyn_array_get(stack, stack->size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                if(num == 0){
                    if(jump_to_whitespace_label(tokens, labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                else{
                    tmp_label.id = get_whitespace_label(tokens, &tokens_idx);
                    if(!tmp_label.id){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                    
                break;
            case WS_FC_TT_l:
                if(dyn_array_get(stack, stack->size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                if(num < 0){
                    if(jump_to_whitespace_label(tokens, labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                else{
                    tmp_label.id = get_whitespace_label(tokens, &tokens_idx);
                    if(!tmp_label.id){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                    
                break;
            case WS_FC_TL:
                if(call_stack->size > 0){
                    if(dyn_array_pop_back(call_stack, &idx) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                else{
                    fprintf(stderr, "Call stack cannot be empty when ending a subroutine\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                tokens_idx = idx;

                break;
            case WS_FC_LL:
                free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);

                return EXIT_SUCCESS;

            // Handle Heap command
            case WS_HP_S:
                if(stack->size < 2){
                    fprintf(stderr, "Stack count cannot be less than two when performing a heap storing operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_pop_back(stack, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(stack, &heap_addr) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                if(heap_addr >= heap->capacity){
                    new_cap = heap->capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap->capacity = new_cap;

                    if(dyn_array_resize(heap, heap->capacity) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(heap, heap_addr, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                }
                    
                break;
            case WS_HP_T:
                if(stack->size == 0){
                    fprintf(stderr, "Stack size cannot be zero when performing a heap retrieval operation\n");
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_pop_back(stack, &heap_addr) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                if(heap_addr >= heap->capacity){
                    new_cap = heap->capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap->capacity = new_cap;

                    if(dyn_array_resize(heap, heap->capacity) == EXIT_FAILURE){
                        free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_get(heap, heap_addr, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_push_back(stack, &num) == EXIT_FAILURE){
                    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            
            default:
                fprintf(stderr, "Encountered unexpected token during execution\n");
                free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);
                return EXIT_FAILURE;
        }
    }

    free_execution_resources_whitespace(tokens, stack, heap, labels, call_stack, tmp_label.id);

    return EXIT_SUCCESS;
}

int execute_whitespace(char *file_name){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_whitespace_file(fptr);

    fclose(fptr);

    return status;
}