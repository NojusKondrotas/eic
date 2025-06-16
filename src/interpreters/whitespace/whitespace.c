#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/io.h"
#include "../../include/whitespace.h"
#include "../../include/numerics.h"
#include "../../include/dyn_array.h"
#include "../../include/whitespace_lexer.h"

void free_execution_resources(DynArray *tokens, DynArray *stack, DynArray *heap, DynArray *labels, DynArray *call_stack, DynArray *tmp_label_array){
    dyn_array_free(tokens);
    dyn_array_free(stack);
    dyn_array_free(heap);
    dyn_array_free(labels);
    dyn_array_free(call_stack);
    dyn_array_free(tmp_label_array);
}

bool match_ws_label(Label *label, DynArray *against_labels, size_t *matching_label_idx){
    size_t idx = 0;
    while(idx < against_labels->size){
        Label *candidate = (Label *)dyn_array_get(against_labels, idx);
        if(check_if_null_ptr(candidate))
            return false;
        // printf("label length: %zu, %zu\n", label->id.size, candidate->id.size);

        if(label->id.size != candidate->id.size){
            ++idx;
            continue;
        }
        
        if(check_array_equality_size_t((size_t *)label->id.data, (size_t *)candidate->id.data, label->id.size)){
            *matching_label_idx = idx;
            return true;
        }

        ++idx;
    }
    
    return false;
}

int jump_to_label(DynArray *tokens, DynArray *labels, size_t *tokens_idx, size_t *idx, Label *tmp_label){
    if(get_ws_label(tokens, tokens_idx, &tmp_label->id) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    
    if(match_ws_label(tmp_label, labels, idx)){
        Label *tmp = (Label *)dyn_array_get(labels, *idx);
        if(check_if_null_ptr(tmp))
            return EXIT_FAILURE;
        *tokens_idx = tmp->instruction_index + 1;
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "Found no matching label when attempting jump\n");
        return EXIT_FAILURE;
    }
}

int execute_whitespace_file(FILE* fptr){
    DynArray tokens;
    DynArray stack;
    DynArray heap;
    DynArray labels;
    DynArray call_stack;

    // Execution variables
    size_t tokens_idx = 0, cmd;
    // Utility variables
    size_t idx, heap_addr, new_cap;
    ptrdiff_t num, tmp;
    Label tmp_label;
    unsigned char ch;
    void *ptr1 = NULL, *ptr2 = NULL;

    if(tokenize_whitespace(fptr, &tokens, &labels) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(dyn_array_init(&stack, STACK_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
        return EXIT_FAILURE;
    }
    
    if(dyn_array_init(&heap, HEAP_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
        return EXIT_FAILURE;
    }

    if(dyn_array_init(&call_stack, STACK_CAP, sizeof(size_t)) == EXIT_FAILURE){
        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
        return EXIT_FAILURE;
    }

    if(dyn_array_init(&tmp_label.id, STACK_CAP, 1) == EXIT_FAILURE){
        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
        return EXIT_FAILURE;
    }

    while(tokens_idx < tokens.size){
        ptr1 = dyn_array_get(&tokens, tokens_idx++);
        if(check_if_null_ptr(ptr1)){
            free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
            return EXIT_FAILURE;
        }
            
        cmd = *(size_t *)ptr1;
        // printf("curr: %zu, size: %zu\n", tokens_idx, tokens.size);
        
        switch(cmd){
            // Handle IO command
            case IO_TS:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                heap_addr = *(size_t *)ptr1;
                --stack.size;

                if(read_in_char_ws(&ch) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(&heap, heap_addr, &ch) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                break;
            case IO_TT:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                heap_addr = *(size_t *)ptr1;
                --stack.size;

                if(read_in_number_ws(&num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(&heap, heap_addr, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case IO_SS:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ch = *(unsigned char *)ptr1;
                --stack.size;

                if(out_char_ws(ch) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;
            case IO_ST:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                num = *(ptrdiff_t *)ptr1;
                --stack.size;

                if(out_number_ws(num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;

            // Handle Stack Manipulation command
            case SM_S_n:
                if(parse_whitespace_number(&tokens, &tokens_idx, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_push_back(&stack, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                break;
            case SM_LS:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when duplicating the top item\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                if(dyn_array_push_back(&stack, ptr1) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                 
                break;
            case SM_LT:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing a swap of its top items\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                tmp = *(ptrdiff_t *)ptr1;

                if(dyn_array_set(&stack, stack.size - 2, ptr2) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(dyn_array_set(&stack, stack.size - 1, &tmp) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_LL:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                --stack.size;
                    
                break;
            case SM_TS_n:
                if(parse_whitespace_number(&tokens, &tokens_idx, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(num >= 0 && num >= stack.size){
                    fprintf(stderr, "Stack index out of bounds\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 1 - num);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                tmp = *(ptrdiff_t *)ptr1;
                
                if(dyn_array_push_back(&stack, &tmp) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_TL_n:
                if(parse_whitespace_number(&tokens, &tokens_idx, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(num >= 0 && num >= stack.size){
                    fprintf(stderr, "Given argument cannot be more than or equal to stack's item count when performing stack sliding\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                stack.size -= num;
                    
                break;

            // Handle Arithmetic command
            case AR_SS:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                num = (*(ptrdiff_t *)ptr1) + (*(ptrdiff_t *)ptr2);
                --stack.size;

                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_ST:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                num = (*(ptrdiff_t *)ptr1) - (*(ptrdiff_t *)ptr2);
                --stack.size;

                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_SL:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                num = (*(ptrdiff_t *)ptr1) * (*(ptrdiff_t *)ptr2);
                --stack.size;

                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_TS:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if((*(ptrdiff_t *)ptr2) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing an integer division operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
            
                num = (*(ptrdiff_t *)ptr1) / (*(ptrdiff_t *)ptr2);
                --stack.size;

                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_TT:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if((*(ptrdiff_t *)ptr2) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing a modulo operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
            
                num = (*(ptrdiff_t *)ptr1) % (*(ptrdiff_t *)ptr2);
                --stack.size;
                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;

            // Handle Flow Control command
            case FC_SS_l:
                break;
            case FC_ST_l:
                if(dyn_array_push_back(&call_stack, &tokens_idx) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                if(jump_to_label(&tokens, &labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case FC_SL_l:
                if(jump_to_label(&tokens, &labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            case FC_TS_l:
                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                num = *(ptrdiff_t *)ptr1;
                
                if(num == 0){
                    if(jump_to_label(&tokens, &labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                else{
                    if(get_ws_label(&tokens, &tokens_idx, &tmp_label.id) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                    
                break;
            case FC_TT_l:
                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                num = *(ptrdiff_t *)ptr1;
                    
                if(num < 0){
                    if(jump_to_label(&tokens, &labels, &tokens_idx, &idx, &tmp_label) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                else{
                    if(get_ws_label(&tokens, &tokens_idx, &tmp_label.id) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }
                    
                break;
            case FC_TL:
                if(call_stack.size > 0){
                    ptr1 = dyn_array_get(&call_stack, call_stack.size - 1);
                    if(check_if_null_ptr(ptr1)){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                    idx = *(size_t *)ptr1;
                }
                else{
                    fprintf(stderr, "Call stack cannot be empty when ending a subroutine\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                --call_stack.size;
                tokens_idx = idx;

                break;
            case FC_LL:
                free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);

                return EXIT_SUCCESS;

            // Handle Heap command
            case HP_S:
                if(stack.size < 2){
                    fprintf(stderr, "Stack count cannot be less than two when performing a heap storing operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                ptr1 = dyn_array_get(&stack, stack.size - 2);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                ptr2 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                
                heap_addr = *(size_t *)ptr1;
                num = *(ptrdiff_t *)ptr2;
                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(&heap, heap_addr, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                }
                
                stack.size -= 2;
                    
                break;
            case HP_T:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be zero when performing a heap retrieval operation\n");
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                ptr1 = dyn_array_get(&stack, stack.size - 1);
                if(check_if_null_ptr(ptr1)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                heap_addr = *(size_t *)ptr1;
                --stack.size;
                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                        return EXIT_FAILURE;
                    }
                }

                ptr2 = dyn_array_get(&heap, heap_addr);
                if(check_if_null_ptr(ptr2)){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }

                num = *(ptrdiff_t *)ptr2;

                if(dyn_array_push_back(&stack, &num) == EXIT_FAILURE){
                    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                    return EXIT_FAILURE;
                }
                    
                break;
            
            default:
                fprintf(stderr, "Encountered unexpected token during execution\n");
                free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);
                return EXIT_FAILURE;
        }
    }

    free_execution_resources(&tokens, &stack, &heap, &labels, &call_stack, &tmp_label.id);

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