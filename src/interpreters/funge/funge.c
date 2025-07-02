#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/dyn_array.h"
#include "../../include/funge.h"
#include "../../include/funge_lexer.h"
#include "../../include/numerics.h"

void *funge_ip_init(FungeSpace *section, char *position, ptrdiff_t *delta, ptrdiff_t *storage_offset, DynArray *stack_stack_f98, DynArray *fingerprints, int modes){
    FungeIP *ip = (FungeIP *)malloc(sizeof(FungeIP));
    if(!ip) return NULL;

    ip->section = section;
    ip->position = position;
    ip->delta = delta;
    ip->storage_offset = storage_offset;
    ip->stack_stack_f98 = stack_stack_f98;
    ip->fingerprints = fingerprints;
    ip->modes = modes;

    return ip;
}

void funge_ip_free(FungeIP *ip){
    ip->modes = 0;
    free(ip->position);
    free(ip->delta);
    free(ip->storage_offset);
    dyn_array_free(ip->stack_stack_f98);
    dyn_array_free(ip->fingerprints);
    free(ip);
}

void *funge_space_init(size_t offset_x, size_t offset_y, FungeSpace *root, FungeSpace *up, FungeSpace *down, size_t section_size, FungeSpace *left, FungeSpace *right){
    FungeSpace *space = (FungeSpace *)malloc(sizeof(FungeSpace));
    if(!space) return NULL;

    space->section = (ptrdiff_t *)calloc(section_size, sizeof(ptrdiff_t));
    if(!space->section){
        funge_space_free(space);
        return NULL;
    }

    space->offset_x = offset_x;
    space->offset_y = offset_y;
    space->root = root;
    space->up = up;
    space->down = down;
    space->left = left;
    space->right = right;
    
    return space;
}

void funge_space_free(FungeSpace *space){
    if(!space) return;
    FungeSpace *right = space->right, *left = space->left, *up = space->up, *down = space->down;
    
    space->offset_x = 0;
    space->offset_y = 0;
    free(space->section);
    space->section = NULL;
    free(space);
    space = NULL;

    if(right) right->left = NULL;
    if(left) left->right = NULL;
    if(up) up->down = NULL;
    if(down) down->up = NULL;

    funge_space_free(right);
    funge_space_free(left);
    funge_space_free(up);
    funge_space_free(down);
}

void free_execution_resources_funge(FungeSpace *origin_root, DynArray *stack, DynArray *IPs){
    funge_space_free(origin_root);
    dyn_array_free(stack);
    dyn_array_free(IPs);
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

int execute_funge_file(FILE* fptr, FungeFlags exec_flags, int exec_dimensions){
    FungeSpace *origin_root = NULL;
    DynArray *stack_b93_96_97 = NULL;
    DynArray *IPs = NULL;

    if(tokenize_funge(fptr, origin_root, exec_flags) == EXIT_FAILURE){
        free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
        return EXIT_FAILURE;
    }

    if(!(exec_flags & Funge98_Flag))
    {
        stack_b93_96_97 = (DynArray *)dyn_array_init(SMALLEST_CONTAINER_CAP, sizeof(DynArray));
        DynArray *tmp = dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(ptrdiff_t));
        if(!stack_b93_96_97 || !tmp){
            fprintf(stderr, "Failure allocating memory\n");
            free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
            return EXIT_FAILURE;
        }
        
        if(dyn_array_push_back(stack_b93_96_97, tmp) == EXIT_FAILURE){
            dyn_array_free(tmp);
            free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
            return EXIT_FAILURE;
        }
    }

    IPs = (DynArray *)dyn_array_init(SMALLEST_CONTAINER_CAP, sizeof(FungeIP));
    if(!IPs){
        fprintf(stderr, "Failure allocating memory\n");
        free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
        return EXIT_FAILURE;
    }
    {
        FungeIP *tmp = NULL;
        char *tmp_pos_origin = (char *)calloc(exec_dimensions, sizeof(char));
        ptrdiff_t *tmp_delta_origin = (ptrdiff_t *)calloc(exec_dimensions, sizeof(ptrdiff_t));
        ptrdiff_t *tmp_storage_offset_origin = (ptrdiff_t *)calloc(exec_dimensions, sizeof(ptrdiff_t));
        if(!tmp_pos_origin || !tmp_delta_origin || !tmp_storage_offset_origin){
            fprintf(stderr, "Failure allocating memory\n");
            free(tmp_pos_origin);
            free(tmp_delta_origin);
            free(tmp_storage_offset_origin);
            free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
            return EXIT_FAILURE;
        }

        if(exec_flags & Funge98_Flag){
            DynArray *tmp_stack = dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(ptrdiff_t));
            DynArray *tmp_stack_stack = dyn_array_init(SMALLEST_CONTAINER_CAP, sizeof(DynArray));
            if(!tmp_stack || !tmp_stack_stack){
                fprintf(stderr, "Failure allocating memory\n");
                free(tmp_pos_origin);
                free(tmp_delta_origin);
                free(tmp_storage_offset_origin);
                dyn_array_free(tmp_stack);
                dyn_array_free(tmp_stack_stack);
                free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
                return EXIT_FAILURE;
            }

            if(dyn_array_push_back(tmp_stack_stack, tmp_stack) == EXIT_FAILURE){
                free(tmp_pos_origin);
                free(tmp_delta_origin);
                free(tmp_storage_offset_origin);
                dyn_array_free(tmp_stack);
                dyn_array_free(tmp_stack_stack);
                free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
                return EXIT_FAILURE;
            }

            tmp = funge_ip_init(origin_root, tmp_pos_origin, tmp_delta_origin, tmp_storage_offset_origin, tmp_stack_stack, NULL, 0);
            if(!tmp){
                free(tmp_pos_origin);
                free(tmp_delta_origin);
                free(tmp_storage_offset_origin);
                dyn_array_free(tmp_stack);
                dyn_array_free(tmp_stack_stack);
                free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
                return EXIT_FAILURE;
            }
        }
        else{
            tmp = funge_ip_init(origin_root, tmp_pos_origin, tmp_delta_origin, tmp_storage_offset_origin, NULL, NULL, 0);
            if(!tmp){
                free(tmp_pos_origin);
                free(tmp_delta_origin);
                free(tmp_storage_offset_origin);
                free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
                return EXIT_FAILURE;
            }
        }
    }

    ptrdiff_t c;
    FungeIP curr_ip;
    // Utility
    size_t cell_coordinate;

    while(true){
        for(size_t i = 0; i < IPs->size; ++i){
            if(dyn_array_get(IPs, i, &curr_ip) == EXIT_FAILURE){
                free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
                return EXIT_FAILURE;
            }
            if(exec_dimensions == 1)
                cell_coordinate = curr_ip.position[0];
            else cell_coordinate = curr_ip.position[1] * FUNGE_SPACE_LENGTH + curr_ip.position[0];

            c = curr_ip.section->section[cell_coordinate];

            // switch
        }
    }

    free_execution_resources_funge(origin_root, stack_b93_96_97, IPs);
    return EXIT_SUCCESS;
}

int execute_funge(char *file_name, FungeFlags exec_flags, int exec_dimensions){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_funge_file(fptr, exec_flags, exec_dimensions);

    fclose(fptr);

    return status;
}