#include <stdio.h>
#include <stdlib.h>
#include "whitespace.h"
#include "lexer.h"
#include "../../runtime/io.h"
#include "../../runtime/stack.h"

int tokenize_io(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    size_t key = WS_KEY(c1, c2);

    switch(key){
        case (TAB << 8) | TAB:
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_TT);

        case( SPACE << 8) | SPACE:
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_SS);

        case (TAB << 8) | SPACE:
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_TS);

        case (SPACE << 8) | TAB:
            return size_t_push_token(tokens, tokens_cap, tokens_count, IO_ST);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IO command: (ASCII: %zu %zu)\n", c1, c2);
            return EXIT_FAILURE;
    }
}

int tokenize_arithmetic(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;

    size_t key = WS_KEY(c1, c2);

    switch(key){
        case (SPACE << 8) | SPACE:
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_SS);

        case (SPACE << 8) | LF:
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_SL);

        case (TAB << 8) | TAB:
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_TT);

        case (TAB<< 8) | SPACE:
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_TS);

        case (SPACE << 8) | TAB:
            return size_t_push_token(tokens, tokens_cap, tokens_count, AR_ST);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: (ASCII: %zu %zu)\n", c1, c2);
            return EXIT_FAILURE;
    }
}

int tokenize_heap(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c;
    if(read_ws_command_char(fptr, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c){
        case SPACE:
            return size_t_push_token(tokens, tokens_cap, tokens_count, HP_S);

        case TAB:
            return size_t_push_token(tokens, tokens_cap, tokens_count, HP_T);

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Heap Access command: (ASCII: %zu)\n", c);
            return EXIT_FAILURE;
    }
}

int tokenize_stack_manip(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c1, c2, key;
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
                case (LF << 8) | SPACE:
                    return size_t_push_token(tokens, tokens_cap, tokens_count, SM_LS);

                case (LF << 8) | TAB:
                    return size_t_push_token(tokens, tokens_cap, tokens_count, SM_LT);

                case (LF << 8) | LF:
                    return size_t_push_token(tokens, tokens_cap, tokens_count, SM_LL);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %zu %zu)\n", c1, c2);
                    return EXIT_FAILURE;
            }

        case TAB:
            if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case (TAB << 8) | SPACE:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_TS_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

                case (TAB << 8) | LF:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_TL_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %zu %zu)\n", c1, c2);
                    return EXIT_FAILURE;
            }

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Stack Manipulation command: (ASCII: %zu)\n", c1);
            return EXIT_FAILURE;
    }
}

int tokenize_flow_control(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    size_t key = WS_KEY(c1, c2);
    switch(key){
        case (SPACE << 8) | SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_SS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

            return EXIT_SUCCESS;
        
        case (SPACE << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_ST_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case (SPACE << 8) | LF:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_Sl_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

            return EXIT_SUCCESS;

        case (TAB << 8) | SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case (TAB << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TT_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens, tokens_cap, tokens_count);

        case (TAB << 8) | LF:
            return size_t_push_token(tokens, tokens_cap, tokens_count, FC_TL);

        case (LF << 8) | LF:
            return size_t_push_token(tokens, tokens_cap, tokens_count, FC_LL);
        
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Flow Control command: (ASCII: %zu %zu)\n", c1, c2);
            return EXIT_FAILURE;
    }
}

size_t *tokenize_whitespace(FILE *fptr, size_t *out_tokens_count){
    size_t tokens_cap = TOKENS_CAP, tokens_count = 0;
    size_t *tokens = calloc(tokens_cap, sizeof(size_t));
    if(!tokens){
        fprintf(stderr, "Failure allocating memory\n");
        return NULL;
    }
    
    size_t imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch(imp_c1){
            case TAB:
                if(read_ws_command_char(fptr, &imp_c2) == EXIT_FAILURE){
                    free(tokens);
                    return NULL;
                }

                size_t key = WS_KEY(imp_c1, imp_c2);
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
                        fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IMP: (ASCII: %zu %zu)\n", imp_c1, imp_c2);
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