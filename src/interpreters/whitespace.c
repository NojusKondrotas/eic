#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "whitespace.h"
#include "../runtime/numerics.h"

typedef struct{
    size_t Spaces, Tabs, instruction_index;
}Label;

int execute_whitespace_file(FILE* fptr){
    int c1, c2;

    size_t stack_cap = 20, stack_top_idx = 0, heap_cap = 10, labels_cap = 5;
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
                handle_stack_manip(fptr, &stack, &stack_cap, &stack_top_idx);
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

void handle_flow_control(FILE *fptr, ssize_t *stack, Label *labels){

}

int handle_stack_manip(FILE *fptr, ssize_t **stack, size_t *stack_cap, size_t *stack_top_idx){
    int c = fgetc(fptr);

    switch(c){
        case SPACE:
            ssize_t number = parse_whitespace_number(fptr);
            int status = push_to_stack(stack, number, stack_cap, stack_top_idx);
            if(status == EXIT_FAILURE)
                return EXIT_FAILURE;

            break;
    }
}

void handle_io(ssize_t *stack, ssize_t *heap){

}

void handle_arithmetic(ssize_t *stack){

}

int push_to_stack(ssize_t **stack, ssize_t number, size_t *stack_cap, size_t *stack_top_idx){
    if(*stack_cap == *stack_top_idx){
        *stack_cap *= 2;
        *stack = realloc(*stack, *stack_cap * sizeof(ssize_t));
        if(!(*stack)){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }
    }
    
    (*stack)[*stack_top_idx] = number;
    ++(*stack_top_idx);

    return EXIT_SUCCESS;
}