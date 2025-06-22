#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/befunge93.h"
#include "../../include/lexer.h"
#include "../../include/dyn_array.h"
#include "../../include/numerics.h"

void free_execution_resources_befunge93(unsigned char *grid, DynArray *stack){
    free(grid);
    dyn_array_free(stack);
}

int execute_befunge93_file(FILE* fptr){
    unsigned char *grid;
    if(tokenize_befunge93(fptr, grid) == EXIT_FAILURE)
        return EXIT_FAILURE;

    DynArray stack;
    if(dyn_array_init(&stack, STACK_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
        free_execution_resources_befunge93(grid, &stack);
        return EXIT_FAILURE;
    }

    size_t ip[2] = {0, 0};
    size_t delta[2];
    size_t caps[2] = {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP};
    unsigned char c;
    // Utility variables
    int tmp;
    ptrdiff_t num_one, num_two;

    while(1){
        c = grid[ip[1] * caps[0] + ip[0]];

        switch(c){
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                if(dyn_array_push_back(&stack, &c) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
    
                break;
            case Add:
            case Substract:
            case Multiply:
            case Int_Divide:
            case Modulo:
                if(stack_arithmetic_top(&stack, true, c) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
    
                break;
            case Logical_Not:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a logical not operation\n");
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(&stack, &num_one) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                
                if(num_one == 0)
                    num_one = 1;
                else num_one = 0;

                if(dyn_array_push_back(&stack, &num_one) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
    
                break;
            case Greater_Than:
                if(stack.size < 2){
                    fprintf(stderr, "Stack size cannot be less than 2 when performing a greater than comparison\n");
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(dyn_array_pop_back(&stack, &num_one) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(&stack, &num_two) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(num_two > num_one)
                    num_one = 1;
                else num_one = 0;

                if(dyn_array_push_back(&stack, &num_one) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                break;
            case Move_Random:
                tmp = rand() % 4;
                switch(tmp){
                    case 0:
                        c = Move_Right;
                        break;
                    case 1:
                        c = Move_Left;
                        break;
                    case 2:
                        c = Move_Up;
                        break;
                    case 3:
                        c = Move_Down;
                        break;
                }
                break;
            case Pop_Move_Horiz:
                if(stack.size == 0){
                    num_one = 0;
                }
                else if(dyn_array_pop_back(&stack, &num_one) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                c = num_one == 0 ? Move_Right : Move_Left;
                break;
            case Pop_Move_Vert:
                if(stack.size == 0){
                    num_one = 0;
                }
                else if(dyn_array_pop_back(&stack, &num_one) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                c = num_one == 0 ? Move_Down : Move_Up;
                break;
            case Bridge:
                move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);
                break;
            case String_Endpoint:
                move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);

                c = grid[ip[1] * caps[0] + ip[0]];

                while(c != '\"'){
                    if(dyn_array_push_back(&stack, &c) != EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                    
                    move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);

                    c = grid[ip[1] * caps[0] + ip[0]];
                }
    
                break;
            case Duplicate:
    
                break;
            case Swap:

                break;
            case Pop_Discard:
                if(stack.size == 0){
                    fprintf(stderr, "Stack size cannot be 0 when performing a pop operation\n");
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                --stack.size;

                break;
            case In_Int:
    
                break;
            case In_ASCII:
    
                break;
            case Out_Int:
    
                break;
            case Out_ASCII:
    
                break;
            case Put:
    
                break;
            case Get:
    
                break;
            case End:
    
                break;
            default:
                break;
        }

        switch(c){
            case Move_Right:
                delta[0] = 1;
                delta[1] = 0;
                break;
            case Move_Left:
                delta[0] = -1;
                delta[1] = 0;
                break;
            case Move_Up:
                delta[0] = 0;
                delta[1] = -1;
                break;
            case Move_Down:
                delta[0] = 0;
                delta[1] = 1;
                break;
        }
        
        move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);
    }
    
    return EXIT_SUCCESS;
}

int execute_befunge93(char *file_name){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_befunge93_file(fptr);

    fclose(fptr);

    return status;
}