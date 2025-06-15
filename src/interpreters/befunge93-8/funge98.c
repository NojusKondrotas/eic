#include <stdio.h>
#include <stdlib.h>
#include "../../include/funge98.h"

int execute_funge98_file(FILE* fptr){
    return EXIT_SUCCESS;
}

int execute_funge98(char *file_name){
    FILE *fptr;

    if((fptr = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "Failure opening input file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    int status = execute_funge98_file(fptr);

    fclose(fptr);

    return status;
}