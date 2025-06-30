#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../../include/eic.h"
#include "../../include/funge.h"
#include "../../include/funge_lexer.h"
#include "../../include/numerics.h"

int tokenize_funge(FILE *fptr, FungeSpace *root, size_t exec_flags, int exec_dimensions){
    if(exec_flags & Unefunge_Flag)
        root = (FungeSpace *)funge_space_init(UNEFUNGE_SPACE_WIDTH);
    else
        root = (FungeSpace *)funge_space_init(FUNGE_SPACE_HEIGHT * FUNGE_SPACE_WIDTH);

    if(!root){
        fprintf(stderr, "Failure allocating memory\n");
        free(root->right);
        return EXIT_FAILURE;
    }
    char horizontal_index = 0, vertical_index = 0;

    int c;

    while((c = fgetc(fptr)) != EOF){
        horizontal_index = 0;
        while(c != LF){
            if(horizontal_index >= FUNGE_SPACE_WIDTH){
                if(exec_flags & Unefunge_Flag)
                    root->right = (FungeSpace *)funge_space_init(UNEFUNGE_SPACE_WIDTH);
                else
                    root->right = (FungeSpace *)funge_space_init(FUNGE_SPACE_HEIGHT * FUNGE_SPACE_WIDTH);

                if(!root->right){
                    fprintf(stderr, "Failure allocating memory\n");
                    funge_space_free(root->right);
                    return EXIT_FAILURE;
                }
                horizontal_index = 0;
            }

            c = fgetc(fptr);
            
            ++horizontal_index;
        }
        
        if(exec_flags & Unefunge_Flag)
            return EXIT_SUCCESS;

        ++vertical_index;
    }

    return EXIT_SUCCESS;
}