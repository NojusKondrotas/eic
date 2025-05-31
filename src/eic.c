#include <stdio.h>
#include <stdlib.h>
#include "eic.h"
#include "interpreters/brainfuck.h"
#include "interpreters/whitespace.h"

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
            return execute_bf(argv[1]);
    case WS:
        return execute_whitespace(argv[1]);
    default:
        fprintf(stderr, "Unsupported file extension: %s\n", extension);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

char *get_extension(char *str){
    char *extension = str;
    while(*str != '\0'){
        if(*str != '.' || *(str + 1) == '\\' || *(str + 1) == '.' || *(str + 1) == '/')
            extension = ++str;
        else return ++extension;
    }

    return extension;
}

unsigned long hash_string(char *str){
    unsigned long hash = 5381;
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c;
    
    return hash;
}