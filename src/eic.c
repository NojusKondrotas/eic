#include <stdio.h>
#include <stdlib.h>
#include "include/eic.h"
#include "include/befunge93.h"
#include "include/brainfuck.h"
#include "include/funge98.h"
#include "include/whitespace.h"

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

    size_t hash_result = hash_string(extension);
    
    switch (hash_result)
    {
    case BF:
        //TO-DO: request additional confirmation concerning confusion between the execution of Brainfuck or Befunge-93
        break;
    case B:
        return execute_brainfuck(argv[1]);
    case WS:
        return execute_whitespace(argv[1]);
    case BE:
    case B93:
    case BEFUNGE:
        return execute_befunge93(argv[1]);
    case B98:
    case TF:
    case UF:
        return execute_funge98(argv[1]);
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

size_t hash_string(char *str){
    size_t hash = 5381;
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c;
    
    return hash;
}