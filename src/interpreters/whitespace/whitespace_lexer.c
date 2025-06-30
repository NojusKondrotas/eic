#include <stdio.h>
#include <stdlib.h>
#include "../../include/io.h"
#include "../../include/dyn_array.h"
#include "../../include/numerics.h"
#include "../../include/whitespace.h"
#include "../../include/whitespace_lexer.h"

int create_whitespace_label(FILE *fptr, size_t instr_idx, DynArray *labels){
    Label label;
    label.instruction_index = instr_idx - 1;
    if(dyn_array_init(&label.id, LABELS_CAP, sizeof(size_t)) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (tokenize_whitespace_raw(fptr, &label.id) == EXIT_FAILURE) {
        dyn_array_free(&label.id);
        return EXIT_FAILURE;
    }
    
    if(dyn_array_push_back(labels, &label) == EXIT_FAILURE){
        dyn_array_free(&label.id);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int tokenize_io(FILE *fptr, DynArray *tokens){
    int c1, c2;
    size_t val;
    if(read_whitespace_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_whitespace_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    unsigned int key = WS_KEY(c1, c2);

    switch(key){
        case WS_KEY(TAB, TAB):
            val = IO_TT;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY( SPACE, SPACE):
            val = IO_SS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(TAB, SPACE):
            val = IO_TS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(SPACE, TAB):
            val = IO_ST;
            return dyn_array_push_back(tokens, &val);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IO command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_arithmetic(FILE *fptr, DynArray *tokens){
    int c1, c2;
    size_t val;
    if(read_whitespace_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_whitespace_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;

    unsigned int key = WS_KEY(c1, c2);

    switch(key){
        case WS_KEY(SPACE, SPACE):
            val = AR_SS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(SPACE, LF):
            val = AR_SL;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(TAB, TAB):
            val = AR_TT;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(TAB, SPACE):
            val = AR_TS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(SPACE, TAB):
            val = AR_ST;
            return dyn_array_push_back(tokens, &val);

        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Arithmetic command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_heap(FILE *fptr, DynArray *tokens){
    int c;
    size_t val;
    if(read_whitespace_command_char(fptr, &c) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c){
        case SPACE:
            val = HP_S;
            return dyn_array_push_back(tokens, &val);

        case TAB:
            val = HP_T;
            return dyn_array_push_back(tokens, &val);

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Heap Access command: (ASCII: %u)\n", (unsigned char)c);
            return EXIT_FAILURE;
    }
}

int tokenize_stack_manip(FILE *fptr, DynArray *tokens){
    int c1, c2;
    unsigned int key;
    size_t val;
    if(read_whitespace_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch(c1){
        case SPACE:
            val = SM_S_n;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            return tokenize_whitespace_raw(fptr, tokens);

        case LF:
            if(read_whitespace_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case WS_KEY(LF, SPACE):
                    val = SM_LS;
                    return dyn_array_push_back(tokens, &val);

                case WS_KEY(LF, TAB):
                    val = SM_LT;
                    return dyn_array_push_back(tokens, &val);

                case WS_KEY(LF, LF):
                    val = SM_LL;
                    return dyn_array_push_back(tokens, &val);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
                    return EXIT_FAILURE;
            }

        case TAB:
            if(read_whitespace_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case WS_KEY(TAB, SPACE):
                    val = SM_TS_n;
                    if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_whitespace_raw(fptr, tokens);

                case WS_KEY(TAB, LF):
                    val = SM_TL_n;
                    if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_whitespace_raw(fptr, tokens);

                default:
                    fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Stack Manipulation command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
                    return EXIT_FAILURE;
            }

        default:
            fprintf(stderr, "Unrecognised character while tokenizing whitespace Stack Manipulation command: (ASCII: %u)\n", (unsigned char)c1);
            return EXIT_FAILURE;
    }
}

int tokenize_flow_control(FILE *fptr, DynArray *tokens, DynArray *labels){
    int c1, c2;
    size_t val;
    if(read_whitespace_command_char(fptr, &c1) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(read_whitespace_command_char(fptr, &c2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    unsigned int key = WS_KEY(c1, c2);
    switch(key){
        case WS_KEY(SPACE, SPACE):
            val = FC_SS_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return create_whitespace_label(fptr, tokens->size, labels);
        
        case WS_KEY(SPACE, TAB):
            val = FC_ST_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(SPACE, LF):
            val = FC_SL_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(TAB, SPACE):
            val = FC_TS_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(TAB, TAB):
            val = FC_TT_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(TAB, LF):
            val = FC_TL;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(LF, LF):
            val = FC_LL;
            return dyn_array_push_back(tokens, &val);
        
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Flow Control command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_whitespace(FILE *fptr, DynArray *tokens, DynArray *labels){
    if(dyn_array_init(tokens, TOKENS_CAP, sizeof(size_t)) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(dyn_array_init(labels, TOKENS_CAP, sizeof(Label)) == EXIT_FAILURE){
        dyn_array_free(tokens);
        return EXIT_FAILURE;
    }
    
    int imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch(imp_c1){
            case TAB:
                if(read_whitespace_command_char(fptr, &imp_c2) == EXIT_FAILURE){
                    dyn_array_free(tokens);
                    dyn_array_free(labels);
                    return EXIT_FAILURE;
                }

                unsigned int key = WS_KEY(imp_c1, imp_c2);
                switch(key){
                    case WS_KEY(TAB, LF):
                        if(tokenize_io(fptr, tokens) == EXIT_FAILURE){
                            dyn_array_free(tokens);
                            dyn_array_free(labels);
                            return EXIT_FAILURE;
                        }

                        break;

                    case WS_KEY(TAB, SPACE):
                        if(tokenize_arithmetic(fptr, tokens) == EXIT_FAILURE){
                            dyn_array_free(tokens);
                            dyn_array_free(labels);
                            return EXIT_FAILURE;
                        }

                        break;

                    case WS_KEY(TAB, TAB):
                        if(tokenize_heap(fptr, tokens) == EXIT_FAILURE){
                            dyn_array_free(tokens);
                            dyn_array_free(labels);
                            return EXIT_FAILURE;
                        }

                        break;

                    default:
                        fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IMP: (ASCII: %u %u)\n", (unsigned char)imp_c1, (unsigned char)imp_c2);
                        dyn_array_free(tokens);
                        dyn_array_free(labels);
                        return EXIT_FAILURE;
                }
                break;

            case SPACE:
                if(tokenize_stack_manip(fptr, tokens) == EXIT_FAILURE){
                    dyn_array_free(tokens);
                    dyn_array_free(labels);
                    return EXIT_FAILURE;
                }
                break;

            case LF:
                if(tokenize_flow_control(fptr, tokens, labels) == EXIT_FAILURE){
                    dyn_array_free(tokens);
                    dyn_array_free(labels);
                    return EXIT_FAILURE;
                }
                break;
        }
    }

    return EXIT_SUCCESS;
}