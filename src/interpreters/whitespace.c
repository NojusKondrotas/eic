#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "whitespace.h"
#include "../runtime/numerics.h"

typedef struct{
    size_t Spaces, Tabs, instruction_index;
}Label;

void handle_flow_control(FILE *fptr, ssize_t *stack, Label *labels){

}

int ensure_stack_cap(ssize_t **stack, size_t *stack_cap, size_t *stack_top){
    if(*stack_cap == *stack_top){
        *stack_cap *= 2;
        *stack = realloc(*stack, *stack_cap * sizeof(ssize_t));
        if(!(*stack)){
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}

int push_to_stack(ssize_t **stack, ssize_t number, size_t *stack_cap, size_t *stack_top){
    if(ensure_stack_cap(stack, stack_cap, stack_top) == EXIT_FAILURE){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    
    (*stack)[*stack_top] = number;
    ++(*stack_top);

    return EXIT_SUCCESS;
}

int dup_stack(ssize_t **stack, size_t *stack_cap, size_t *stack_top){
    if(*stack_top == 0){
        fprintf(stderr, "Failure duplicating stack, no elements contained\n");
        return EXIT_FAILURE;
    }
    if(ensure_stack_cap(stack, stack_cap, stack_top) == EXIT_FAILURE){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    (*stack)[*stack_top] = (*stack)[*stack_top - 1];
    ++(*stack_top);

    return EXIT_SUCCESS;
}

int swap_stack(ssize_t **stack, size_t *stack_cap, size_t *stack_top){
    if(*stack_top < 2){
        fprintf(stderr, "Failure conducting stack swap: stack contains too few elements (%zu)\n", *stack_top);
        return EXIT_FAILURE;
    }

    --(*stack_top);
    ssize_t tmp = (*stack)[*stack_top - 1];
    (*stack)[*stack_top - 1] = (*stack)[*stack_top];
    (*stack)[*stack_top] = tmp;
    ++(*stack_top);

    return EXIT_SUCCESS;
}

int slide_stack(ssize_t **stack, size_t n, size_t *stack_cap, size_t *stack_top){
    if(n >= *stack_top){
        fprintf(stderr, "Failure sliding stack, too many elements to remove (%zu)", n);
        return EXIT_FAILURE;
    }

    size_t op_bottom = *stack_top - n - 1;
    (*stack)[op_bottom] = (*stack)[*stack_top - 1];

    (*stack_top) -= n;

    return EXIT_SUCCESS;
}

int handle_stack_manip(FILE *fptr, ssize_t **stack, size_t *stack_cap, size_t *stack_top){
    int c = fgetc(fptr);

    switch(c){
        case SPACE:
            ssize_t number = parse_whitespace_number(fptr);
            
            if(push_to_stack(stack, number, stack_cap, stack_top) == EXIT_FAILURE)
                return EXIT_FAILURE;

            break;
        case LF:
            c = fgetc(fptr);
            
            switch (c)
            {
            case SPACE:
                if(dup_stack(stack, stack_cap, stack_top) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                    
                break;
            case TAB:
                if(swap_stack(stack, stack_cap, stack_top) == EXIT_FAILURE)
                    return EXIT_FAILURE;

                break;
            case LF:
                if(stack_top == 0){
                    fprintf(stderr, "Failure conducting stack discard: stack contains zero elements\n");
                    return EXIT_FAILURE;
                }

                --(*stack_top);
                break;
            default:
                fprintf(stderr, "Unrecognised stack manipulation command\n");
                return EXIT_FAILURE;
            }

            break;
        case TAB:
            c = fgetc(fptr);

            switch(c){
                case SPACE:
                    ssize_t i = parse_whitespace_number(fptr);
                    if(i < 0 || i >= *stack_top){
                        fprintf(stderr, "Failure copying %zdnth element\n", i);
                        return EXIT_FAILURE;
                    }

                    if(ensure_stack_cap(stack, stack_cap, stack_top) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    (*stack)[(*stack_top)++] = (*stack)[i];
                    break;
                case LF:
                    ssize_t n = parse_whitespace_number(fptr);
                    if(n < 0){
                        fprintf(stderr, "stack sliding amount can't be less than zero (%zd)\n", n);
                        return EXIT_FAILURE;
                    }

                    if(slide_stack(stack, n, stack_cap, stack_top) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                    
                    break;
                default:
                    fprintf(stderr, "Unrecognised stack manipulation command\n");
                    return EXIT_FAILURE;
            }

            break;
        default:
            fprintf(stderr, "Unrecognised stack manipulation command\n");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void handle_io(ssize_t *stack, ssize_t *heap){

}

void handle_arithmetic(ssize_t *stack){

}

int execute_whitespace_file(FILE* fptr){
    int c1, c2;

    size_t stack_cap = STACK_CAP, stack_top = 0, heap_cap = HEAP_CAP, labels_cap = LABELS_CAP;
    ssize_t *stack = calloc(stack_cap, sizeof(ssize_t));
    if(!stack){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    ssize_t *heap = calloc(heap_cap, sizeof(ssize_t));
    if(!heap){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    Label *labels = calloc(labels_cap, sizeof(Label));
    if(!labels){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    while((c1 = fgetc(fptr)) != EOF){
        switch(c1){
            case LF:
                //handle_flow_control(fptr, stack, labels);
                break;
            case SPACE:
                if(handle_stack_manip(fptr, &stack, &stack_cap, &stack_top) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
                break;
            case TAB:
                c2 = fgetc(fptr);
                switch (c2){
                    case LF:
                        //handle_io(stack, heap);
                        break;
                    case SPACE:
                        //handle_arithmetic(stack);
                        break;
                    case TAB:
                        //logic
                        break;
                    
                    default:
                        return EXIT_FAILURE;
                }
        }
    }

    free(stack);
    free(heap);
    free(labels);

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