#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/dyn_array.h"
#include "../../include/befunge93.h"
#include "../../include/lexer.h"
#include "../../include/numerics.h"

int tokenize_befunge93(FILE *fptr, unsigned char *grid){
    size_t horizontal_index = 0, vertical_index = 0;

    int c;

    while((c = fgetc(fptr)) != EOF && vertical_index < GRID_VERTICAL_CAP){
        horizontal_index = 0;
        while(c != LF && horizontal_index < GRID_HORIZONTAL_CAP){
            grid[vertical_index * GRID_HORIZONTAL_CAP + horizontal_index] = c;

            c = fgetc(fptr);
            ++horizontal_index;
        }

        ++vertical_index;
    }

    return EXIT_SUCCESS;
}