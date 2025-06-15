#include <stdio.h>
#include <stdlib.h>
#include "../../include/befunge93.h"

int execute_befunge93_file(FILE* fptr){
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