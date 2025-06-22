#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../../include/befunge93.h"
#include "../../include/io.h"
#include "../../include/lexer.h"
#include "../../include/dyn_array.h"
#include "../../include/numerics.h"

void free_execution_resources_befunge93(unsigned char *grid, DynArray *stack){
    free(grid);
    dyn_array_free(stack);
}

int execute_befunge93_file(FILE* fptr){
    unsigned char *grid;
    grid = (unsigned char *)malloc(GRID_VERTICAL_CAP * GRID_HORIZONTAL_CAP);
    if(!grid){
        fprintf(stderr, "Failure allcoating memory\n");
        return EXIT_FAILURE;
    }
    memset(grid, SPACE, GRID_VERTICAL_CAP * GRID_HORIZONTAL_CAP);

    if(tokenize_befunge93(fptr, grid) == EXIT_FAILURE)
        return EXIT_FAILURE;
    DynArray stack;
    if(dyn_array_init(&stack, STACK_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
        free_execution_resources_befunge93(grid, &stack);
        return EXIT_FAILURE;
    }

    size_t ip[2] = {0, 0};
    size_t delta[2] = {1, 0};
    size_t caps[2] = {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP};
    unsigned char c;
    // Utility variables
    unsigned char io_character;
    int rand_res;
    ptrdiff_t x, y, val;

    while(1){
        c = grid[ip[1] * caps[0] + ip[0]];

        switch(c){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                x = c - '0';
                if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
    
                break;
            case Add:
            case Substract:
            case Multiply:
            case Int_Divide:
            case Modulo:
                while(stack.size < 2){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }
                if(stack_arithmetic_top(&stack, true, c) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
    
                break;
            case Logical_Not:
                if(stack.size == 0){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }
                if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                
                if(x == 0)
                    x = 1;
                else x = 0;

                if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
    
                break;
            case Greater_Than:
                while(stack.size < 2){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(&stack, &y) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(y > x)
                    x = 1;
                else x = 0;

                if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                break;
            case Move_Random:
                rand_res = rand() % 4;
                switch(rand_res){
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
                    val = 0;
                }
                else if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                c = val == 0 ? Move_Right : Move_Left;
                break;
            case Pop_Move_Vert:
                if(stack.size == 0){
                    val = 0;
                }
                else if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                c = val == 0 ? Move_Down : Move_Up;
                break;
            case Bridge:
                move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);
                break;
            case String_Endpoint:
                move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);

                c = grid[ip[1] * caps[0] + ip[0]];

                while(c != '\"'){
                    if(dyn_array_push_back(&stack, &c) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                    
                    move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);

                    c = grid[ip[1] * caps[0] + ip[0]];
                }
    
                break;
            case Duplicate:
                if(stack.size == 0){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }

                if(stack_duplicate_element(&stack, stack.size - 1) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case Swap:
                while(stack.size < 2){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }

                if(stack_swap_top(&stack) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case Pop_Discard:
                if(stack.size > 0){
                    --stack.size;
                }

                break;
            case In_Int:
                if(in_number(&x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case In_ASCII:
                if(in_char(&io_character) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(dyn_array_push_back(&stack, &io_character) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case Out_Int:
                if(stack.size == 0){
                    val = 0;
                }
                else if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(out_number(val) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case Out_ASCII:
                if(stack.size == 0){
                    io_character = 0;
                }
                else if(dyn_array_pop_back(&stack, &io_character) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                if(out_char(io_character) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case Put:
                while(stack.size < 3){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_pop_back(&stack, &y) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                x %= caps[0];
                y %= caps[1];
                grid[y * caps[0] + x] = val;
                break;
            case Get:
                while(stack.size < 3){
                    val = 0;
                    if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                        free_execution_resources_befunge93(grid, &stack);
                        return EXIT_FAILURE;
                    }
                }

                if(dyn_array_pop_back(&stack, &y) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }

                x = (x % caps[0] + caps[0]) % caps[0];
                y = (y % caps[1] + caps[1]) % caps[1];
                val = grid[y * caps[0] + x];
                if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
                    free_execution_resources_befunge93(grid, &stack);
                    return EXIT_FAILURE;
                }
                break;
            case End:
                free_execution_resources_befunge93(grid, &stack);
                return EXIT_SUCCESS;
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