#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/io.h"
#include "../../include/whitespace.h"
#include "../../include/numerics.h"
#include "../../include/stack.h"
#include "../../include/lexer.h"

void free_resources(Iterator *tokens, SignedStack *stack, ptrdiff_t *heap, Label *labels){
    if(tokens->elements)
        free(tokens->elements);
    
    if(stack->arr)
        free(stack->arr);

    if(heap)
        free(heap);

    if(labels)
        free(labels);
}

// void print_stack(ssize_t *stack, size_t stack_top) {
//     printf("Stack [");
//     for (size_t i = 0; i < stack_top; ++i) {
//         printf("%zd", stack[i]);
//         if (i < stack_top - 1) printf(", ");
//     }
//     printf("]\n");
// }

// void handle_flow_control(FILE *fptr, ssize_t *stack, Label *labels){

// }

// int dup_stack(ssize_t **stack, size_t *stack_cap, size_t *stack_top){
//     if(*stack_top == 0){
//         fprintf(stderr, "Failure duplicating stack, no elements contained\n");
//         return EXIT_FAILURE;
//     }
//     if(ensure_stack_cap(stack, stack_cap, stack_top) == EXIT_FAILURE){
//         fprintf(stderr, "Failure allocating memory\n");
//         return EXIT_FAILURE;
//     }

//     (*stack)[*stack_top] = (*stack)[*stack_top - 1];
//     ++(*stack_top);

//     return EXIT_SUCCESS;
// }

// int swap_stack(ssize_t **stack, size_t *stack_cap, size_t *stack_top){
//     if(*stack_top < 2){
//         fprintf(stderr, "Failure conducting stack swap: stack contains too few elements (%zu)\n", *stack_top);
//         return EXIT_FAILURE;
//     }

//     --(*stack_top);
//     ssize_t tmp = (*stack)[*stack_top - 1];
//     (*stack)[*stack_top - 1] = (*stack)[*stack_top];
//     (*stack)[*stack_top] = tmp;
//     ++(*stack_top);

//     return EXIT_SUCCESS;
// }

// int slide_stack(ssize_t **stack, size_t n, size_t *stack_cap, size_t *stack_top){
//     if(n >= *stack_top){
//         fprintf(stderr, "Failure sliding stack, too many elements to remove (%zu)", n);
//         return EXIT_FAILURE;
//     }

//     size_t op_bottom = *stack_top - n - 1;
//     (*stack)[op_bottom] = (*stack)[*stack_top - 1];

//     (*stack_top) -= n;

//     return EXIT_SUCCESS;
// }

// int handle_stack_manip(FILE *fptr, ssize_t **stack, size_t *stack_cap, size_t *stack_top){
//     int c = fgetc(fptr);

//     switch(c){
//         case SPACE:
//             ssize_t number = parse_whitespace_number(fptr);
            
//             if(push_to_stack(stack, number, stack_cap, stack_top) == EXIT_FAILURE)
//                 return EXIT_FAILURE;

//             break;
//         case LF:
//             c = fgetc(fptr);
            
//             switch (c)
//             {
//             case SPACE:
//                 if(dup_stack(stack, stack_cap, stack_top) == EXIT_FAILURE)
//                     return EXIT_FAILURE;
                    
//                 break;
//             case TAB:
//                 if(swap_stack(stack, stack_cap, stack_top) == EXIT_FAILURE)
//                     return EXIT_FAILURE;

//                 break;
//             case LF:
//                 if(stack_top == 0){
//                     fprintf(stderr, "Failure conducting stack discard: stack contains zero elements\n");
//                     return EXIT_FAILURE;
//                 }

//                 --(*stack_top);
//                 break;
//             default:
//                 fprintf(stderr, "Unrecognised stack manipulation command\n");
//                 return EXIT_FAILURE;
//             }

//             break;
//         case TAB:
//             c = fgetc(fptr);

//             switch(c){
//                 case SPACE:
//                     ssize_t i = parse_whitespace_number(fptr);
//                     if(i < 0 || i >= *stack_top){
//                         fprintf(stderr, "Failure copying %zdnth element\n", i);
//                         return EXIT_FAILURE;
//                     }

//                     if(ensure_stack_cap(stack, stack_cap, stack_top) == EXIT_FAILURE)
//                         return EXIT_FAILURE;

//                     (*stack)[(*stack_top)++] = (*stack)[i];
//                     break;
//                 case LF:
//                     ssize_t n = parse_whitespace_number(fptr);
//                     if(n < 0){
//                         fprintf(stderr, "stack sliding amount can't be less than zero (%zd)\n", n);
//                         return EXIT_FAILURE;
//                     }

//                     if(slide_stack(stack, n, stack_cap, stack_top) == EXIT_FAILURE)
//                         return EXIT_FAILURE;
                    
//                     break;
//                 default:
//                     fprintf(stderr, "Unrecognised stack manipulation command\n");
//                     return EXIT_FAILURE;
//             }

//             break;
//         default:
//             fprintf(stderr, "Unrecognised stack manipulation command\n");
//             return EXIT_FAILURE;
//     }

//     return EXIT_SUCCESS;
// }

// void handle_io(ssize_t *stack, ssize_t *heap){

// }

// void handle_arithmetic(ssize_t *stack){

// }

int execute_whitespace_file(FILE* fptr){
    Iterator tokens_iter;
    SignedStack stack;
    ptrdiff_t *heap;
    Label *labels;
    if(tokenize_whitespace(fptr, &tokens_iter) == EXIT_FAILURE)
        return EXIT_FAILURE;

    stack = (SignedStack){.capacity = STACK_CAP, .count = 0, .arr = calloc(STACK_CAP, sizeof(ptrdiff_t)) };
    if(!stack.arr){
        fprintf(stderr, "Failure allocating memory\n");
        free_resources(&tokens_iter, &stack, heap, labels);
        return EXIT_FAILURE;
    }
    size_t heap_cap = LABELS_CAP;
    heap = calloc(HEAP_CAP, sizeof(ptrdiff_t));
    if(!heap){
        free_resources(&tokens_iter, &stack, heap, labels);
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    size_t labels_cap = LABELS_CAP, labels_count = 0;
    labels = calloc(LABELS_CAP, sizeof(Label));
    if(!labels){
        fprintf(stderr, "Failure allocating memory\n");
        free_resources(&tokens_iter, &stack, heap, labels);
        return EXIT_FAILURE;
    }

    unsigned int cmd;
    // Utility variables
    size_t heap_addr, new_cap;
    ptrdiff_t num, tmp;
    ptrdiff_t *tmp_alloc;
    unsigned char ch;
    while(next(&tokens_iter)){
        cmd = tokens_iter.elements[tokens_iter.index++];
        printf("%X\n", cmd);
        switch(cmd){
            // Handle IO command
            case IO_TS:
                if(stack.count == 0){
                    fprintf(stderr, "Stack count cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                heap_addr = stack.arr[--stack.count];

                if(read_in_char_ws(&ch) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap_cap){
                    new_cap = heap_cap * 2;
                    while(new_cap <= heap_addr)
                        new_cap = heap_cap * 2;
                        
                    tmp_alloc = realloc(heap, new_cap * sizeof(ptrdiff_t));
                    if(!tmp_alloc){
                        fprintf(stderr, "Failure allocating memory\n");
                        free_resources(&tokens_iter, &stack, heap, labels);
                        return EXIT_FAILURE;
                    }
                    heap = tmp_alloc;
                    heap_cap = new_cap;
                }

                heap[heap_addr] = ch;
                    
                break;
            case IO_TT:
                if(stack.count == 0){
                    fprintf(stderr, "Stack count cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                heap_addr = stack.arr[--stack.count];

                if(read_in_number_ws(&num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap_cap){
                    new_cap = heap_cap * 2;
                    while(new_cap <= heap_addr)
                        new_cap = heap_cap * 2;
                        
                    tmp_alloc = realloc(heap, new_cap * sizeof(ptrdiff_t));
                    if(!tmp_alloc){
                        fprintf(stderr, "Failure allocating memory\n");
                        free_resources(&tokens_iter, &stack, heap, labels);
                        return EXIT_FAILURE;
                    }
                    heap = tmp_alloc;
                    heap_cap = new_cap;
                }

                heap[heap_addr] = num;
                    
                break;
            case IO_SS:
                if(stack.count == 0){
                    fprintf(stderr, "Stack count cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                ch = stack.arr[--stack.count];

                if(out_char_ws(ch) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;
            case IO_ST:
                if(stack.count == 0){
                    fprintf(stderr, "Stack count cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                num = stack.arr[--stack.count];

                if(out_number_ws(num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;

            // Handle Stack Manipulation command
            case SM_S_n:
                if(parse_whitespace_number(&tokens_iter, &num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                if(push_signed(&stack, num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_LS:
                if(ensure_cap_signed(&stack) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                if(push_signed(&stack, stack.arr[stack.count - 1]) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_LT:
                if(stack.count < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing a swap of its top items\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                tmp = stack.arr[stack.count - 2];
                stack.arr[stack.count - 2] = stack.arr[stack.count - 1];
                stack.arr[stack.count - 1] = tmp;
                    
                break;
            case SM_LL:
                if(stack.count < 1){
                    fprintf(stderr, "Stack count cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                --stack.count;
                    
                break;
            case SM_TS_n:
                if(parse_whitespace_number(&tokens_iter, &num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                if(num >= stack.count){
                    fprintf(stderr, "Stack index out of bounds\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                
                if(push_signed(&stack, num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_TL_n:
                if(parse_whitespace_number(&tokens_iter, &num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                if(num >= stack.count){
                    fprintf(stderr, "Given argument cannot be more than or equal to stack's item count when performing stack sliding\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                stack.count -= num;
                    
                break;

            // Handle Arithmetic command
            case AR_SS:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case AR_ST:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case AR_SL:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case AR_TS:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case AR_TT:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;

            // Handle Flow Control command
            case FC_SS_l:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case FC_ST_l:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case FC_SL_l:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case FC_TS_l:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case FC_TT_l:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case FC_TL:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;
            case FC_LL:
                //if(handle == exit_failure)
                    //return exit_failure;
                    
                break;

            // Handle Heap command
            case HP_S:
                if(stack.count < 2){
                    fprintf(stderr, "Stack count cannot be less than two when performing a heap storing operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                heap_addr = stack.arr[stack.count - 2];
                num = stack.arr[stack.count - 1];
                if(heap_addr >= heap_cap){
                    new_cap = heap_cap * 2;
                    while(new_cap <= heap_addr)
                        new_cap = heap_cap * 2;
                        
                    tmp_alloc = realloc(heap, new_cap * sizeof(ptrdiff_t));
                    if(!tmp_alloc){
                        fprintf(stderr, "Failure allocating memory\n");
                        free_resources(&tokens_iter, &stack, heap, labels);
                        return EXIT_FAILURE;
                    }
                    heap = tmp_alloc;
                    heap_cap = new_cap;
                }

                heap[heap_addr] = num;
                stack.count -= 2;
                    
                break;
            case HP_T:
                if(stack.count < 1){
                    fprintf(stderr, "Stack count cannot be less than one when performing a heap retrieval operation\n");
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }

                heap_addr = stack.arr[--stack.count];
                if(heap_addr >= heap_cap){
                    new_cap = heap_cap * 2;
                    while(new_cap <= heap_addr)
                        new_cap = heap_cap * 2;
                        
                    tmp_alloc = realloc(heap, new_cap * sizeof(ptrdiff_t));
                    if(!tmp_alloc){
                        fprintf(stderr, "Failure allocating memory\n");
                        free_resources(&tokens_iter, &stack, heap, labels);
                        return EXIT_FAILURE;
                    }
                    heap = tmp_alloc;
                    heap_cap = new_cap;
                }

                num = heap[heap_addr];

                if(push_signed(&stack, num) == EXIT_FAILURE){
                    free_resources(&tokens_iter, &stack, heap, labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            
            default:
                //handle
                break;
        }
    }

    free_resources(&tokens_iter, &stack, heap, labels);

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