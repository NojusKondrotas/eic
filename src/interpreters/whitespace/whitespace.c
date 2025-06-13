#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/io.h"
#include "../../include/whitespace.h"
#include "../../include/numerics.h"
#include "../../include/dyn_array.h"
#include "../../include/lexer.h"

void free_resources(DynArray *tokens, DynArray *stack, DynArray *heap, DynArray *labels){
    dyn_array_free(tokens);
    dyn_array_free(stack);
    dyn_array_free(heap);
    dyn_array_free(labels);
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
    DynArray tokens;
    DynArray stack;
    DynArray heap;
    DynArray labels;
    if(tokenize_whitespace(fptr, &tokens, &labels) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(dyn_array_init(&stack, STACK_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
        free_resources(&tokens, &stack, &heap, &labels);
        return EXIT_FAILURE;
    }
    
    if(dyn_array_init(&heap, HEAP_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
        free_resources(&tokens, &stack, &heap, &labels);
        return EXIT_FAILURE;
    }

    if(dyn_array_init(&labels, LABELS_CAP, sizeof(Label)) == EXIT_FAILURE){
        free_resources(&tokens, &stack, &heap, &labels);
        return EXIT_FAILURE;
    }

    size_t tokens_idx = 0;
    unsigned int cmd;
    // Utility variables
    size_t heap_addr, new_cap;
    ptrdiff_t num, tmp;
    unsigned char ch;

    while(tokens_idx < tokens.size){
        cmd = *(unsigned int *)dyn_array_get(&tokens, tokens_idx++);
        printf("%X\n", cmd);
        switch(cmd){
            // Handle IO command
            case IO_TS:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                heap_addr = *(size_t *)dyn_array_get(&stack, --stack.size);

                if(read_in_char_ws(&ch) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_resources(&tokens, &stack, &heap, &labels);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(&heap, heap_addr, &ch) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case IO_TT:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                heap_addr = *(size_t *)dyn_array_get(&stack, --stack.size);

                if(read_in_number_ws(&num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_resources(&tokens, &stack, &heap, &labels);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(&heap, heap_addr, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case IO_SS:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                
                ch = *(unsigned char *)dyn_array_get(&stack, --stack.size);

                if(out_char_ws(ch) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;
            case IO_ST:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                num = *(ptrdiff_t *)dyn_array_get(&stack, --stack.size);

                if(out_number_ws(num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;

            // Handle Stack Manipulation command
            case SM_S_n:
                if(parse_whitespace_number(&tokens, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                if(dyn_array_push_back(&stack, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_LS:
                if(dyn_array_push_back(&stack, dyn_array_get(&stack, stack.size - 1)) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_LT:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing a swap of its top items\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                tmp = *(ptrdiff_t *)dyn_array_get(&stack, stack.size - 2);
                if(dyn_array_set(&stack, stack.size - 2, dyn_array_get(&stack, stack.size - 1)) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                if(dyn_array_set(&stack, stack.size - 1, &tmp) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_LL:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                --stack.size;
                    
                break;
            case SM_TS_n:
                if(parse_whitespace_number(&tokens, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                if(num >= stack.size){
                    fprintf(stderr, "Stack index out of bounds\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                tmp = *(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1 - num);
                
                if(dyn_array_push_back(&stack, &tmp) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case SM_TL_n:
                if(parse_whitespace_number(&tokens, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                if(num >= stack.size){
                    fprintf(stderr, "Given argument cannot be more than or equal to stack's item count when performing stack sliding\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                stack.size -= num;
                    
                break;

            // Handle Arithmetic command
            case AR_SS:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                num = (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 2)) + (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1));
                --stack.size;
                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_ST:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                
                num = (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 2)) - (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1));
                --stack.size;
                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_SL:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                
                num = (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 2)) * (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1));
                --stack.size;
                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_TS:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                if((*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1)) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing an integer division operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
            
                num = (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 2)) / (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1));
                --stack.size;
                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            case AR_TT:
                if(stack.size < 2){
                    fprintf(stderr, "Stack cannot have less than two items when performing an arithmetic operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                
                if((*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1)) == 0){
                    fprintf(stderr, "Stack's top item cannot be zero when performing a modulo operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
            
                num = (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 2)) % (*(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1));
                --stack.size;
                if(dyn_array_set(&stack, stack.size - 1, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
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
                if(stack.size < 2){
                    fprintf(stderr, "Stack count cannot be less than two when performing a heap storing operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                
                heap_addr = *(size_t *)dyn_array_get(&stack, stack.size - 2);
                num = *(ptrdiff_t *)dyn_array_get(&stack, stack.size - 1);
                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_resources(&tokens, &stack, &heap, &labels);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_set(&heap, heap_addr, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                        return EXIT_FAILURE;
                }
                
                stack.size -= 2;
                    
                break;
            case HP_T:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be zero when performing a heap retrieval operation\n");
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }

                heap_addr = *(size_t *)dyn_array_get(&stack, --stack.size);
                if(heap_addr >= heap.capacity){
                    new_cap = heap.capacity * 2;
                    while(new_cap <= heap_addr)
                        new_cap *= 2;
                    
                    heap.capacity = new_cap;

                    if(dyn_array_resize(&heap, heap.capacity) == EXIT_FAILURE){
                        free_resources(&tokens, &stack, &heap, &labels);
                        return EXIT_FAILURE;
                    }
                }

                num = *(ptrdiff_t *)dyn_array_get(&heap, heap_addr);

                if(dyn_array_push_back(&stack, &num) == EXIT_FAILURE){
                    free_resources(&tokens, &stack, &heap, &labels);
                    return EXIT_FAILURE;
                }
                    
                break;
            
            default:
                //handle
                break;
        }
    }

    free_resources(&tokens, &stack, &heap, &labels);

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