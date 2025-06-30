#include <stdio.h>
#include <stdlib.h>
#include "../../include/dyn_array.h"
#include "../../include/funge.h"
#include "../../include/funge_lexer.h"

void *funge_space_init(size_t section_size){
    FungeSpace *space;
    space = (FungeSpace *)calloc(1, sizeof(FungeSpace));
    if(!space) return space;

    space->section = (ptrdiff_t *)calloc(section_size, sizeof(ptrdiff_t));
    if(!space->section) return NULL;
    space->right = NULL;
    space->left = NULL;
    space->up = NULL;
    space->down = NULL;
    
    return space;
}

//TO-DO: IMPLEMENT
void funge_space_connect(FungeSpace *root, FungeSpace *other, ptrdiff_t offset_x, ptrdiff_t offset_y){
    while(offset_x > 0){
        root = root->right;
        --offset_x;
    }
    while(offset_x < 0){
        root = root->left;
        ++offset_x;
    }
    while(offset_y > 0){
        root = root->down;
        --offset_y;
    }
    while(offset_y < 0){
        root = root->up;
        ++offset_y;
    }

    return root;
}

void funge_space_free(FungeSpace *space){
    if(!space) return;

    free(space->section);
    space->section = NULL;
    free(space);
    space = NULL;
}

void root_funge_space_free(FungeSpace *root){
    if(!root) return;

    FungeSpace *right = root->right, *left = root->left, *up = root->up , *down = root->down;
    funge_space_free(root);
    root_funge_space_free(right);
    root_funge_space_free(left);
    root_funge_space_free(up);
    root_funge_space_free(down);
}

void free_execution_resources_funge(FungeSpace *root, DynArray *stack){
    dyn_array_free(stack);
    root_funge_space_free(root);
}

// int execute_befunge93_file(FILE* fptr){
//     size_t grids_size[4][2] = {
//         {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP},
//         {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP},
//         {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP},
//         {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP}
//     };
//     unsigned char *grids[4];
//     for(int i = 0; i < 4; ++i){
//         grids[i] = (unsigned char *)malloc(grids_size[3][0] * grids_size[3][1]);
//         if(!grids[i]){
//             fprintf(stderr, "Failure allcoating memory\n");
//             return EXIT_FAILURE;
//         }
//         memset(grids[i], SPACE, grids_size[i][0] * grids_size[i][1]);
//     }

//     DynArray ips;
//     if(dyn_array_init(&ips, IPS_CAP, sizeof(FungeIp)) == EXIT_FAILURE){
//         free_execution_resources_befunge93(grids, &ips, NULL);
//         return EXIT_FAILURE;
//     }

//     if(tokenize_befunge93(fptr, grids) == EXIT_FAILURE)
//         return EXIT_FAILURE;
//     DynArray stack;
//     if(dyn_array_init(&stack, STACK_CAP, sizeof(ptrdiff_t)) == EXIT_FAILURE){
//         free_execution_resources_befunge93(grids, &ips, &stack);
//         return EXIT_FAILURE;
//     }

//     size_t ip[2] = {0, 0};
//     size_t delta[2] = {1, 0};
//     size_t caps[2] = {GRID_HORIZONTAL_CAP, GRID_VERTICAL_CAP};
//     unsigned char c;
//     // Utility variables
//     unsigned char io_character;
//     int rand_res;
//     ptrdiff_t x, y, val;

//     while(1){
//         c = grids[0][ip[1] * caps[0] + ip[0]];

//         switch(c){
//             case '0':
//             case '1':
//             case '2':
//             case '3':
//             case '4':
//             case '5':
//             case '6':
//             case '7':
//             case '8':
//             case '9':
//                 x = c - '0';
//                 if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
    
//                 break;
//             case Add:
//             case Substract:
//             case Multiply:
//             case Int_Divide:
//             case Modulo:
//                 while(stack.size < 2){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }
//                 if(stack_arithmetic_top(&stack, true, c) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
    
//                 break;
//             case Logical_Not:
//                 if(stack.size == 0){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }
//                 if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
                
//                 if(x == 0)
//                     x = 1;
//                 else x = 0;

//                 if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
    
//                 break;
//             case Greater_Than:
//                 while(stack.size < 2){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }

//                 if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 if(dyn_array_pop_back(&stack, &y) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 if(y > x)
//                     x = 1;
//                 else x = 0;

//                 if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 break;
//             case Move_Random:
//                 rand_res = rand() % 4;
//                 switch(rand_res){
//                     case 0:
//                         c = Move_Right;
//                         break;
//                     case 1:
//                         c = Move_Left;
//                         break;
//                     case 2:
//                         c = Move_Up;
//                         break;
//                     case 3:
//                         c = Move_Down;
//                         break;
//                 }
//                 break;
//             case Pop_Move_Horiz:
//                 if(stack.size == 0){
//                     val = 0;
//                 }
//                 else if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 c = val == 0 ? Move_Right : Move_Left;
//                 break;
//             case Pop_Move_Vert:
//                 if(stack.size == 0){
//                     val = 0;
//                 }
//                 else if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 c = val == 0 ? Move_Down : Move_Up;
//                 break;
//             case Bridge:
//                 move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);
//                 break;
//             case String_Endpoint:
//                 move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);

//                 c = grid[ip[1] * caps[0] + ip[0]];

//                 while(c != '\"'){
//                     if(dyn_array_push_back(&stack, &c) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
                    
//                     move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);

//                     c = grid[ip[1] * caps[0] + ip[0]];
//                 }
    
//                 break;
//             case Duplicate:
//                 if(stack.size == 0){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }

//                 if(stack_duplicate_element(&stack, stack.size - 1) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case Swap:
//                 while(stack.size < 2){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }

//                 if(stack_swap_top(&stack) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case Pop_Discard:
//                 if(stack.size > 0){
//                     --stack.size;
//                 }

//                 break;
//             case In_Int:
//                 if(in_number(&x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 if(dyn_array_push_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case In_ASCII:
//                 if(in_char(&io_character) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 if(dyn_array_push_back(&stack, &io_character) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case Out_Int:
//                 if(stack.size == 0){
//                     val = 0;
//                 }
//                 else if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 if(out_number(val) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case Out_ASCII:
//                 if(stack.size == 0){
//                     io_character = 0;
//                 }
//                 else if(dyn_array_pop_back(&stack, &io_character) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 if(out_char(io_character) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case Put:
//                 while(stack.size < 3){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }

//                 if(dyn_array_pop_back(&stack, &y) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 if(dyn_array_pop_back(&stack, &val) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 x %= caps[0];
//                 y %= caps[1];
//                 grid[y * caps[0] + x] = val;
//                 break;
//             case Get:
//                 while(stack.size < 3){
//                     val = 0;
//                     if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                         free_execution_resources_befunge93(grids, &ips, &stack);
//                         return EXIT_FAILURE;
//                     }
//                 }

//                 if(dyn_array_pop_back(&stack, &y) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 if(dyn_array_pop_back(&stack, &x) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }

//                 x = (x % caps[0] + caps[0]) % caps[0];
//                 y = (y % caps[1] + caps[1]) % caps[1];
//                 val = grid[y * caps[0] + x];
//                 if(dyn_array_push_back(&stack, &val) == EXIT_FAILURE){
//                     free_execution_resources_befunge93(grids, &ips, &stack);
//                     return EXIT_FAILURE;
//                 }
//                 break;
//             case End:
//                 free_execution_resources_befunge93(grids, &ips, &stack);
//                 return EXIT_SUCCESS;
//             default:
//                 break;
//         }

//         switch(c){
//             case Move_Right:
//                 delta[0] = 1;
//                 delta[1] = 0;
//                 break;
//             case Move_Left:
//                 delta[0] = -1;
//                 delta[1] = 0;
//                 break;
//             case Move_Up:
//                 delta[0] = 0;
//                 delta[1] = -1;
//                 break;
//             case Move_Down:
//                 delta[0] = 0;
//                 delta[1] = 1;
//                 break;
//         }
        
//         move_funge_ip(ip, delta, caps, BEFUNGE93_DIMENSIONS);
//     }
    
//     return EXIT_SUCCESS;
// }

int execute_funge_file(FILE* fptr, size_t exec_flags, size_t exec_dimensions){
    FungeSpace *root = NULL;
    DynArray stack;

    if(tokenize_funge(fptr, root, exec_flags, exec_dimensions) == EXIT_FAILURE){
        free_execution_resources_funge(root, &stack);
        return EXIT_FAILURE;
    }
}

int execute_funge(char *file_name, size_t exec_flags, size_t exec_dimensions){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_funge_file(fptr, exec_flags, exec_dimensions);

    fclose(fptr);

    return status;
}