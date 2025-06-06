#include <stdio.h>
#include <stdlib.h>
#include "../../include/whitespace.h"
#include "../../include/lexer.h"
#include "../../include/io.h"
#include "../../include/stack.h"

int tokenize_io(FILE *fptr, unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    unsigned int key = WS_KEY(c1, c2);

    switch(key){
        case WS_KEY(TAB, TAB):
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_TT);

        case WS_KEY( SPACE, SPACE):
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_SS);

        case WS_KEY(TAB, SPACE):
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_TS);

        case WS_KEY(SPACE, TAB):
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_ST);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IO command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_arithmetic(FILE *fptr, unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;

    unsigned int key = WS_KEY(c1, c2);

    switch(key){
        case WS_KEY(SPACE, SPACE):
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_SS);

        case WS_KEY(SPACE, LF):
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_SL);

        case WS_KEY(TAB, TAB):
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_TT);

        case WS_KEY(TAB, SPACE):
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_TS);

        case WS_KEY(SPACE, TAB):
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_ST);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_heap(FILE *fptr, unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c;
    if(read_ws_command_char(fptr, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c){
        case SPACE:
            return size_t_push_token(tokens, tokens_cap, tokens_count, HP_S);

        case TAB:
            return size_t_push_token(tokens, tokens_cap, tokens_count, HP_T);

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Heap Access command: (ASCII: %u)\n", (unsigned char)c);
            return EXIT_FAILURE;
    }
}

int tokenize_stack_manip(FILE *fptr, unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    unsigned int key;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c1){
        case SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_S_n) == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case LF:
            if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case WS_KEY(LF, SPACE):
                    return size_t_push_token(tokens, tokens_cap, tokens_count, SM_LS);

                case WS_KEY(LF, TAB):
                    return size_t_push_token(tokens, tokens_cap, tokens_count, SM_LT);

                case WS_KEY(LF, LF):
                    return size_t_push_token(tokens, tokens_cap, tokens_count, SM_LL);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
                    return EXIT_FAILURE;
            }

        case TAB:
            if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case WS_KEY(TAB, SPACE):
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_TS_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

                case WS_KEY(TAB, LF):
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_TL_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
                    return EXIT_FAILURE;
            }

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Stack Manipulation command: (ASCII: %u)\n", (unsigned char)c1);
            return EXIT_FAILURE;
    }
}

int tokenize_flow_control(FILE *fptr, unsigned int **tokens, size_t *tokens_cap, size_t *tokens_count){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    unsigned int key = WS_KEY(c1, c2);
    switch(key){
        case WS_KEY(SPACE, SPACE):
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_SS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);
        
        case WS_KEY(SPACE, TAB):
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_ST_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case WS_KEY(SPACE, LF):
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_SL_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case WS_KEY(TAB, SPACE):
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case WS_KEY(TAB, TAB):
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TT_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case WS_KEY(TAB, LF):
            return size_t_push_token(tokens, tokens_cap, tokens_count, FC_TL);

        case WS_KEY(LF, LF):
            return size_t_push_token(tokens, tokens_cap, tokens_count, FC_LL);
        
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Flow Control command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

unsigned int *tokenize_whitespace(FILE *fptr, size_t *out_tokens_count){
    size_t tokens_cap = TOKENS_CAP, tokens_count = 0;
    unsigned int *tokens = calloc(tokens_cap, sizeof(unsigned int));
    if(!tokens){
        fprintf(stderr, "Failure allocating memory\n");
        return NULL;
    }
    
    int imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch(imp_c1){
            case TAB:
                if(read_ws_command_char(fptr, &imp_c2) == EXIT_FAILURE){
                    free(tokens);
                    return NULL;
                }

                unsigned int key = WS_KEY(imp_c1, imp_c2);
                switch(key){
                    case WS_KEY(TAB, LF):
                        if(tokenize_io(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                            free(tokens);
                            return NULL;
                        }

                        break;

                    case WS_KEY(TAB, SPACE):
                        if(tokenize_arithmetic(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                            free(tokens);
                            return NULL;
                        }

                        break;

                    case WS_KEY(TAB, TAB):
                        if(tokenize_heap(fptr, &tokens, &tokens_cap, &tokens_count) == EXIT_FAILURE){
                            free(tokens);
                            return NULL;
                        }

                        break;

                    default:
                        fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IMP: (ASCII: %u %u)\n", (unsigned char)imp_c1, (unsigned char)imp_c2);
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