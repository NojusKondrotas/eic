#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/eic.h"
#include "../../include/funge.h"
#include "../../include/funge_lexer.h"
#include "../../include/numerics.h"

int tokenize_funge(FILE *fptr, FungeSpace *root, size_t exec_flags){
    size_t max_length, max_size;
    if(exec_flags & Unefunge_Flag){
        max_length = UNEFUNGE_SPACE_LENGTH;
        max_size = UNEFUNGE_SPACE_LENGTH;
    }
    else{
        max_length = FUNGE_SPACE_LENGTH;
        max_size = FUNGE_SPACE_LENGTH * FUNGE_SPACE_LENGTH;
    }

    root = (FungeSpace *)funge_space_init(0, 0, NULL, NULL, NULL, max_size, NULL, NULL);
    if(!root){
        fprintf(stderr, "Failure allocating memory\n");
        funge_space_free(root);
        return EXIT_FAILURE;
    }

    FungeSpace *current_space = root;

    char horizontal_index = 0, vertical_index = 0;

    int c;

    while((c = fgetc(fptr)) != EOF){
        horizontal_index = 0;
        if(vertical_index >= max_length){
            if(!root->down){
                root->down = (FungeSpace *)funge_space_init(0, root->offset_y + 1, NULL, root, NULL, max_size, NULL, NULL);
                if(!root->down){
                    fprintf(stderr, "Failure allocating memory\n");
                    funge_space_free(root);
                    return EXIT_FAILURE;
                }
                root = root->down;
            }
            size_t idx = current_space->offset_x/max_length;
            current_space = root;
            while(idx-- > 0){
                if(!current_space->right){
                    current_space->right = (FungeSpace *)funge_space_init(current_space->offset_x + 1, current_space->offset_y, root, NULL, NULL, max_size, current_space, NULL);
                    if(!current_space->right){
                        fprintf(stderr, "Failure allocating memory\n");
                        funge_space_free(root);
                        return EXIT_FAILURE;
                    }
                    current_space = current_space->right;
                }
            }
        }
        
        while(c != LF){
            if(horizontal_index >= max_length){
                if(exec_flags & Unefunge_Flag)
                    current_space->right = (FungeSpace *)funge_space_init(current_space->offset_x + 1, current_space->offset_y, root, NULL, NULL, UNEFUNGE_SPACE_LENGTH, current_space, NULL);
                else
                    current_space->right = (FungeSpace *)funge_space_init(current_space->offset_x + 1, current_space->offset_y, root, NULL, NULL, FUNGE_SPACE_LENGTH * FUNGE_SPACE_LENGTH, current_space, NULL);

                if(!current_space->right){
                    fprintf(stderr, "Failure allocating memory\n");
                    funge_space_free(root);
                    return EXIT_FAILURE;
                }
                current_space = current_space->right;
                horizontal_index = 0;
            }

            current_space->section[vertical_index * max_length + horizontal_index++] = c;

            c = fgetc(fptr);
        }
        
        if(exec_flags & Unefunge_Flag) return EXIT_SUCCESS;

        ++vertical_index;
    }

    return EXIT_SUCCESS;
}