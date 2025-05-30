#include <stdio.h>
#include <stdlib.h>
#include "eic.h"
#include "src/interpreters/brainfuck.h"

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "No input file\n");
        return EXIT_FAILURE;
    }

    char *extension;
    if(*(extension = get_extension(argv[1])) == '\0'){
        fprintf(stderr, "File does not contain an extension: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    unsigned long hash_result = hash_string(extension);

    switch (hash_result)
    {
    case BF:
        case B:
            break;
    default:
        fprintf(stderr, "Unsupported file extension: %s\n", extension);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}