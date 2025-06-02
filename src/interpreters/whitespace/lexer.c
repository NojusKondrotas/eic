#include <stdio.h>
#include <stdlib.h>
#include "whitespace.h"
#include "lexer.h"
#include "../../runtime/io.h"

int ensure_cap(int **tokens, size_t *tokens_cap, size_t *tokens_count){
    if(*tokens_cap == *tokens_count){
        (*tokens_cap) *= 2;
        int *tmp = realloc(*tokens, *tokens_cap * sizeof(int));
        if(!tmp){
            fprintf(stderr, "Failure allocating memory\n");
            return EXIT_FAILURE;
        }

        *tokens = tmp;
    }

    return EXIT_SUCCESS;
}

int tokenize_io(FILE *fptr, int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    int key = ((unsigned char)c1 << 8) | (unsigned char)c2;

    if(*tokens_cap == *tokens_count){
        if(ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    switch(key){
        case (TAB << 8) | TAB:
            (*tokens)[(*tokens_count)++] = IO_TT;
            return EXIT_SUCCESS;

        case( SPACE << 8) | SPACE:
            (*tokens)[(*tokens_count)++] = IO_SS;
            return EXIT_SUCCESS;

        case (TAB << 8) | SPACE:
            (*tokens)[(*tokens_count)++] = IO_TS;
            return EXIT_SUCCESS;

        case (SPACE << 8) | TAB:
            (*tokens)[(*tokens_count)++] = IO_ST;
            return EXIT_SUCCESS;

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IO command: %c %c (ASCII: %d %d)\n", c1, c2, c1, c2);
            return EXIT_FAILURE;
    }
}

int tokenize_arithmetic(FILE *fptr, int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;

    int key = ((unsigned char)c1 << 8) | (unsigned char)c2;

    if(*tokens_cap == (*tokens_count)){
        if(ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    switch(key){
        case (SPACE << 8) | SPACE:
            (*tokens)[(*tokens_count)++] = AR_SS;
            return EXIT_SUCCESS;

        case (SPACE << 8) | LF:
            (*tokens)[(*tokens_count)++] = AR_SL;
            return EXIT_SUCCESS;

        case (TAB << 8) | TAB:
            (*tokens)[(*tokens_count)++] = AR_TT;
            return EXIT_SUCCESS;

        case (TAB<< 8) | SPACE:
            (*tokens)[(*tokens_count)++] = AR_TS;
            return EXIT_SUCCESS;

        case (SPACE << 8) | TAB:
            (*tokens)[(*tokens_count)++] = AR_ST;
            return EXIT_SUCCESS;

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: %c %c (ASCII: %d %d)\n", c1, c2, c1, c2);
            return EXIT_FAILURE;
    }
}

int tokenize_heap(FILE *fptr, int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c;
    if(read_ws_command_char(fptr, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(*tokens_cap == (*tokens_count)){
        if(ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    switch((unsigned char)c){
        case SPACE:
            (*tokens)[(*tokens_count)++] = HP_S;
            return EXIT_SUCCESS;

        case TAB:
            (*tokens)[(*tokens_count)++] = HP_T;
            return EXIT_SUCCESS;

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Heap Access command: %c (ASCII: %d)\n", c, c);
            return EXIT_FAILURE;
    }
}

int tokenize_stack_manip(FILE *fptr, int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(*tokens_cap == (*tokens_count)){
        if(ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    switch((unsigned char)c1){
        case SPACE:
            (*tokens)[(*tokens_count)++] = SM_S_n;
            return EXIT_SUCCESS;

        case LF:
            int c2_lf;
            if(read_ws_command_char(fptr, &c2_lf) == EXIT_FAILURE)
                return EXIT_FAILURE;

            int key_lf = ((unsigned char)c1 << 8) | (unsigned char)c2_lf;
            switch(key_lf){
                case (LF << 8) | SPACE:
                    (*tokens)[(*tokens_count)++] = SM_LS;
                    return EXIT_SUCCESS;

                case (LF << 8) | TAB:
                    (*tokens)[(*tokens_count)++] = SM_LT;
                    return EXIT_SUCCESS;

                case (LF << 8) | LF:
                    (*tokens)[(*tokens_count)++] = SM_LL;
                    return EXIT_SUCCESS;

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: %c %c (ASCII: %d %d)\n", c1, c2_lf, c1, c2_lf);
                    return EXIT_FAILURE;
            }

        case TAB:
            int c2_tab;
            if(read_ws_command_char(fptr, &c2_tab) == EXIT_FAILURE)
                return EXIT_FAILURE;

            int key_tab = ((unsigned char)c1 << 8) | (unsigned char)c2_tab;
            switch(key_tab){
                case (TAB << 8) | SPACE:
                    (*tokens)[(*tokens_count)++] = SM_TS_n;
                    return EXIT_SUCCESS;

                case (TAB << 8) | LF:
                    (*tokens)[(*tokens_count)++] = SM_TL_n;
                    return EXIT_SUCCESS;

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: %c %c (ASCII: %d %d)\n", c1, c2_tab, c1, c2_tab);
                    return EXIT_FAILURE;
            }

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Stack Manipulation command: %c (ASCII: %d)\n", c1, c1);
            return EXIT_FAILURE;
    }
}

int tokenize_flow_control(FILE *fptr, int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(*tokens_cap == (*tokens_count)){
        if(ensure_cap(tokens, tokens_cap, tokens_count) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    
    int key = ((unsigned char)c1 << 8) | (unsigned char)c2;
    switch(key){
        case (SPACE << 8) | SPACE:
            (*tokens)[(*tokens_count)++] = FC_SS_l;
            return EXIT_SUCCESS;
        
        case (SPACE << 8) | TAB:
            (*tokens)[(*tokens_count)++] = FC_ST_l;
            return EXIT_SUCCESS;

        case (SPACE << 8) | LF:
            (*tokens)[(*tokens_count)++] = FC_Sl_l;
            return EXIT_SUCCESS;

        case (TAB << 8) | SPACE:
            (*tokens)[(*tokens_count)++] = FC_TS_l;
            return EXIT_SUCCESS;

        case (TAB << 8) | TAB:
            (*tokens)[(*tokens_count)++] = FC_TT_l;
            return EXIT_SUCCESS;

        case (TAB << 8) | LF:
            (*tokens)[(*tokens_count)++] = FC_TL;
            return EXIT_SUCCESS;

        case (LF << 8) | LF:
            (*tokens)[(*tokens_count)++] = FC_LL;
            return EXIT_SUCCESS;
        
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Flow Control command: %c %c (ASCII: %d %d)\n", c1, c2, c1, c2);
            return EXIT_FAILURE;
    }
}

int *tokenize_whitespace(FILE *fptr, size_t *out_tokens_count){
    size_t tokens_cap = TOKENS_CAP, tokens_count = 0;
    int *tokens = calloc(tokens_cap, sizeof(int));
    
    int imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch((unsigned char)imp_c1){
            case TAB:
                if(read_ws_command_char(fptr, &imp_c2) == EXIT_FAILURE){
                    free(tokens);
                    return NULL;
                }

                int key = ((unsigned char)imp_c1 << 8) | (unsigned char)imp_c2;
                switch(key){
                    case (TAB << 8) | LF:
                        if(tokenize_io(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                            free(tokens);
                            return NULL;
                        }

                        break;

                    case (TAB << 8) | SPACE:
                        if(tokenize_arithmetic(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                            free(tokens);
                            return NULL;
                        }

                        break;

                    case (TAB << 8) | TAB:
                        if(tokenize_heap(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                            free(tokens);
                            return NULL;
                        }

                        break;

                    default:
                        fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IMP: %c %c (ASCII: %d %d)\n", imp_c1, imp_c2, imp_c1, imp_c2);
                        free(tokens);
                        return NULL;
                }
                break;

            case SPACE:
                if(tokenize_stack_manip(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                    free(tokens);
                    return NULL;
                }
                break;

            case LF:
                if(tokenize_flow_control(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                    free(tokens);
                    return NULL;
                }
                break;
        }
    }

    *out_tokens_count = tokens_count;
    return tokens;
}