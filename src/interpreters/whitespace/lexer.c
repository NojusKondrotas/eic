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
            if(size_t_push_token(tokens, tokens_cap, tokens_count, IO_TT) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case( SPACE << 8) | SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, IO_SS) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (TAB << 8) | SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, IO_TS) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (SPACE << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, IO_ST) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

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
            if(size_t_push_token(tokens, tokens_cap, tokens_count, AR_SS) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (SPACE << 8) | LF:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, AR_SL) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (TAB << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, AR_TT) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (TAB<< 8) | SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, AR_TS) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (SPACE << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, AR_ST) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: (ASCII: %zu %zu)\n", c1, c2);
            return EXIT_FAILURE;
    }
}

int tokenize_heap(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c;
    if(read_ws_command_char(fptr, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch((unsigned char)c){
        case SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, HP_S) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, HP_T) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Heap Access command: (ASCII: %zu)\n", c);
            return EXIT_FAILURE;
    }
}

int tokenize_stack_manip(FILE *fptr, size_t **tokens, size_t *tokens_cap, size_t *tokens_count){
    size_t c1;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch((unsigned char)c1){
        case SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_S_n) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case LF:
            size_t c2_lf;
            if(read_ws_command_char(fptr, &c2_lf) == EXIT_FAILURE)
                return EXIT_FAILURE;

            size_t key_lf = WS_KEY(c1, c2_lf);
            switch(key_lf){
                case (LF << 8) | SPACE:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_LS) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return EXIT_SUCCESS;

                case (LF << 8) | TAB:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_LT) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return EXIT_SUCCESS;

                case (LF << 8) | LF:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_LL) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return EXIT_SUCCESS;

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %zu %zu)\n", c1, c2_lf);
                    return EXIT_FAILURE;
            }

        case TAB:
        size_t c2_tab;
            if(read_ws_command_char(fptr, &c2_tab) == EXIT_FAILURE)
                return EXIT_FAILURE;

            size_t key_tab = WS_KEY(c1, c2_tab);
            switch(key_tab){
                case (TAB << 8) | SPACE:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_TS_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return EXIT_SUCCESS;

                case (TAB << 8) | LF:
                    if(size_t_push_token(tokens, tokens_cap, tokens_count, SM_TL_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return EXIT_SUCCESS;

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %zu %zu)\n", c1, c2_tab);
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

            return EXIT_SUCCESS;
        
        case (SPACE << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_ST_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (SPACE << 8) | LF:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_Sl_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (TAB << 8) | SPACE:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (TAB << 8) | TAB:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TT_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (TAB << 8) | LF:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_TL) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;

        case (LF << 8) | LF:
            if(size_t_push_token(tokens, tokens_cap, tokens_count, FC_LL) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return EXIT_SUCCESS;
        
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