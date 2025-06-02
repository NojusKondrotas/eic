#include <stdio.h>
#include <stdlib.h>
#include "whitespace.h"
#include "lexer.h"

int ensure_cap(char **tokens, size_t *tokens_cap, size_t *tokens_count){
    if(*tokens_cap == *tokens_count){
        (*tokens_cap) *= 2;
        *tokens = realloc(*tokens, *tokens_cap);
        if(!tokens){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int tokenize_io(FILE *fptr, char **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1 = fgetc(fptr), c2 = fgetc(fptr);

    switch(c1 & c2){
        case TAB & TAB:
            if(*tokens_cap == *tokens_count){
                if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }
            *tokens[*tokens_count++] = IO_TT;
            return EXIT_SUCCESS;
        case SPACE & SPACE:
            if(*tokens_cap == *tokens_count){
                if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }
            *tokens[*tokens_count++] = IO_SS;
            return EXIT_SUCCESS;
        case TAB & SPACE:
            if(c1 == TAB){
                if(*tokens_cap == *tokens_count){
                    if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                }
                *tokens[*tokens_count++] = IO_TS;
                return EXIT_SUCCESS;
            }
            else{
                if(*tokens_cap == *tokens_count){
                    if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                }
                *tokens[*tokens_count++] = IO_ST;
                return EXIT_SUCCESS;
            }
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IO command: %c %c (ASCII: %d %d)", c1, c2, c1, c2);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int tokenize_arithmetic(FILE *fptr, char **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1 = fgetc(fptr), c2 = fgetc(fptr);

    switch(c1 & c2){
        case SPACE & SPACE:
            if(*tokens_cap == *tokens_count){
                if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }
            *tokens[*tokens_count++] = AR_SS;
            return EXIT_SUCCESS;
        case SPACE & LF:
            if(*tokens_cap == *tokens_count){
                if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }
            *tokens[*tokens_count++] = AR_SL;
            return EXIT_SUCCESS;
        case TAB & TAB:
            if(*tokens_cap == *tokens_count){
                if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }
            *tokens[*tokens_count++] = AR_TT;
            return EXIT_SUCCESS;
        case TAB & SPACE:
            if(c1 == TAB){
                if(*tokens_cap == *tokens_count){
                    if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                }
                *tokens[*tokens_count++] = AR_TS;
                return EXIT_SUCCESS;
            }
            else{
                if(*tokens_cap == *tokens_count){
                    if(ensure_cap(&tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                }
                *tokens[*tokens_count++] = AR_ST;
                return EXIT_SUCCESS;
            }
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: %c %c (ASCII: %d %d)", c1, c2, c1, c2);
            return EXIT_FAILURE;
    }
}

char *tokenize_whitespace(FILE *fptr){
    size_t tokens_cap = TOKENS_CAP, tokens_count = 0;
    char *tokens = calloc(tokens_cap, sizeof(char *));
    
    int imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch(imp_c1){
            case TAB:
                imp_c2 = fgetc(fptr);
                switch(imp_c2){
                    case LF:
                        if(tokenize_io(fptr, tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE)
                            return NULL;

                        break;
                    case SPACE:
                        //logic
                        break;
                    case TAB:
                        //logic
                        break;
                    default:
                        fprintf(stderr, "Unrecognised character while tokenizing whitespace IMP: %c (ASCII: %d)", imp_c2, imp_c2);
                }
        }
    }

    return tokens;
}