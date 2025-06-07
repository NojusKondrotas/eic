#include <stdio.h>
#include <stdlib.h>
#include "../../include/iterator.h"
#include "../../include/whitespace.h"
#include "../../include/lexer.h"
#include "../../include/io.h"
#include "../../include/stack.h"

int tokenize_io(FILE *fptr, Iterator *tokens){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    unsigned int key = WS_KEY(c1, c2);

    switch(key){
        case WS_KEY(TAB, TAB):
            return iter_ctor_add(tokens, IO_TT);

        case WS_KEY( SPACE, SPACE):
            return iter_ctor_add(tokens, IO_SS);

        case WS_KEY(TAB, SPACE):
            return iter_ctor_add(tokens, IO_TS);

        case WS_KEY(SPACE, TAB):
            return iter_ctor_add(tokens, IO_ST);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IO command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_arithmetic(FILE *fptr, Iterator *tokens){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;

    unsigned int key = WS_KEY(c1, c2);

    switch(key){
        case WS_KEY(SPACE, SPACE):
            return iter_ctor_add(tokens, AR_SS);

        case WS_KEY(SPACE, LF):
            return iter_ctor_add(tokens, AR_SL);

        case WS_KEY(TAB, TAB):
            return iter_ctor_add(tokens, AR_TT);

        case WS_KEY(TAB, SPACE):
            return iter_ctor_add(tokens, AR_TS);

        case WS_KEY(SPACE, TAB):
            return iter_ctor_add(tokens, AR_ST);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_heap(FILE *fptr, Iterator *tokens){
    int c;
    if(read_ws_command_char(fptr, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c){
        case SPACE:
            return iter_ctor_add(tokens, HP_S);

        case TAB:
            return iter_ctor_add(tokens, HP_T);

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Heap Access command: (ASCII: %u)\n", (unsigned char)c);
            return EXIT_FAILURE;
    }
}

int tokenize_stack_manip(FILE *fptr, Iterator *tokens){
    int c1, c2;
    unsigned int key;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c1){
        case SPACE:
            if(iter_ctor_add(tokens, SM_S_n) == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            return tokenize_ws_raw(fptr, tokens);

        case LF:
            if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case WS_KEY(LF, SPACE):
                    return iter_ctor_add(tokens, SM_LS);

                case WS_KEY(LF, TAB):
                    return iter_ctor_add(tokens, SM_LT);

                case WS_KEY(LF, LF):
                    return iter_ctor_add(tokens, SM_LL);

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
                    if(iter_ctor_add(tokens, SM_TS_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_ws_raw(fptr, tokens);

                case WS_KEY(TAB, LF):
                    if(iter_ctor_add(tokens, SM_TL_n) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_ws_raw(fptr, tokens);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
                    return EXIT_FAILURE;
            }

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Stack Manipulation command: (ASCII: %u)\n", (unsigned char)c1);
            return EXIT_FAILURE;
    }
}

int tokenize_flow_control(FILE *fptr, Iterator *tokens){
    int c1, c2;
    if(read_ws_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_ws_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    unsigned int key = WS_KEY(c1, c2);
    switch(key){
        case WS_KEY(SPACE, SPACE):
            if(iter_ctor_add(tokens, FC_SS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens);
        
        case WS_KEY(SPACE, TAB):
            if(iter_ctor_add(tokens, FC_ST_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens);

        case WS_KEY(SPACE, LF):
            if(iter_ctor_add(tokens, FC_SL_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens);

        case WS_KEY(TAB, SPACE):
            if(iter_ctor_add(tokens, FC_TS_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens);

        case WS_KEY(TAB, TAB):
            if(iter_ctor_add(tokens, FC_TT_l) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_ws_raw(fptr, tokens);

        case WS_KEY(TAB, LF):
            return iter_ctor_add(tokens, FC_TL);

        case WS_KEY(LF, LF):
            return iter_ctor_add(tokens, FC_LL);
        
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Flow Control command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_whitespace(FILE *fptr, Iterator *tokens){
    *tokens = (Iterator){.index = 0, .count = 0, .capacity = TOKENS_CAP, .elements = calloc(TOKENS_CAP, sizeof(size_t))};
    if(!tokens->elements){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    
    int imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch(imp_c1){
            case TAB:
                if(read_ws_command_char(fptr, &imp_c2) == EXIT_FAILURE){
                    free(tokens->elements);
                    return EXIT_FAILURE;
                }

                unsigned int key = WS_KEY(imp_c1, imp_c2);
                switch(key){
                    case WS_KEY(TAB, LF):
                        if(tokenize_io(fptr, tokens) == EXIT_FAILURE){
                            free(tokens->elements);
                            return EXIT_FAILURE;
                        }

                        break;

                    case WS_KEY(TAB, SPACE):
                        if(tokenize_arithmetic(fptr, tokens) == EXIT_FAILURE){
                            free(tokens->elements);
                            return EXIT_FAILURE;
                        }

                        break;

                    case WS_KEY(TAB, TAB):
                        if(tokenize_heap(fptr, tokens) == EXIT_FAILURE){
                            free(tokens->elements);
                            return EXIT_FAILURE;
                        }

                        break;

                    default:
                        fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IMP: (ASCII: %u %u)\n", (unsigned char)imp_c1, (unsigned char)imp_c2);
                        free(tokens->elements);
                        return EXIT_FAILURE;
                }
                break;

            case SPACE:
                if(tokenize_stack_manip(fptr, tokens) == EXIT_FAILURE){
                    free(tokens->elements);
                    return EXIT_FAILURE;
                }
                break;

            case LF:
                if(tokenize_flow_control(fptr, tokens) == EXIT_FAILURE){
                    free(tokens->elements);
                    return EXIT_FAILURE;
                }
                break;
        }
    }

    return EXIT_SUCCESS;
}