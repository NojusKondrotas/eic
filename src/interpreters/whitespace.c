#include <stdio.h>
#include <stdlib.h>
#include "whitespace.h"

typedef struct{
    size_t Spaces, Tabs, instruction_index;
}Label;

int execute_whitespace_file(FILE* fptr){
    int c1, c2;

    size_t stack_cap = 20, heap_cap = 10, labels_cap = 5;
    size_t *stack = calloc(stack_cap, sizeof(size_t));
    if(!stack){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    size_t *heap = calloc(heap_cap, sizeof(size_t));
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
                //handle_stack_manip(stack);
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

void handle_flow_control(FILE *fptr, size_t *stack, Label *labels){

}

void handle_stack_manip(size_t *stack){

}

void handle_io(size_t *stack, size_t *heap){

}

void handle_arithmetic(size_t *stack){

}