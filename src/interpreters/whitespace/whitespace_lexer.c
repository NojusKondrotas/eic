#include <stdio.h>
#include <stdlib.h>
#include "../../include/dyn_array.h"
#include "../../include/io.h"
#include "../../include/numerics.h"
#include "../../include/whitespace.h"
#include "../../include/whitespace_lexer.h"

int create_whitespace_label(FILE *fptr, size_t instr_idx, DynArray *labels){
    Whitespace_Label label;
    label.instruction_index = instr_idx - 1;
    label.id = (DynArray *)dyn_array_init(SMALLEST_CONTAINER_CAP, sizeof(Whitespace_Op));
    if(!label.id){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    if (tokenize_whitespace_raw(fptr, label.id) == EXIT_FAILURE) {
        dyn_array_free(label.id);
        return EXIT_FAILURE;
    }
    
    if(dyn_array_push_back(labels, &label) == EXIT_FAILURE){
        dyn_array_free(label.id);
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
            val = WS_IO_TT;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY( SPACE, SPACE):
            val = WS_IO_SS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(TAB, SPACE):
            val = WS_IO_TS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(SPACE, TAB):
            val = WS_IO_ST;
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
            val = WS_AR_SS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(SPACE, LF):
            val = WS_AR_SL;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(TAB, TAB):
            val = WS_AR_TT;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(TAB, SPACE):
            val = WS_AR_TS;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(SPACE, TAB):
            val = WS_AR_ST;
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
            val = WS_HP_S;
            return dyn_array_push_back(tokens, &val);

        case TAB:
            val = WS_HP_T;
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
            val = WS_SM_S_n;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            return tokenize_whitespace_raw(fptr, tokens);

        case LF:
            if(read_whitespace_command_char(fptr, &c2) == EXIT_FAILURE)
                return EXIT_FAILURE;

            key = WS_KEY(c1, c2);
            switch(key){
                case WS_KEY(LF, SPACE):
                    val = WS_SM_LS;
                    return dyn_array_push_back(tokens, &val);

                case WS_KEY(LF, TAB):
                    val = WS_SM_LT;
                    return dyn_array_push_back(tokens, &val);

                case WS_KEY(LF, LF):
                    val = WS_SM_LL;
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
                    val = WS_SM_TS_n;
                    if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                        return EXIT_FAILURE;

                    return tokenize_whitespace_raw(fptr, tokens);

                case WS_KEY(TAB, LF):
                    val = WS_SM_TL_n;
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
            val = WS_FC_SS_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return create_whitespace_label(fptr, tokens->size, labels);
        
        case WS_KEY(SPACE, TAB):
            val = WS_FC_ST_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(SPACE, LF):
            val = WS_FC_SL_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(TAB, SPACE):
            val = WS_FC_TS_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(TAB, TAB):
            val = WS_FC_TT_l;
            if(dyn_array_push_back(tokens, &val) == EXIT_FAILURE)
                return EXIT_FAILURE;

            return tokenize_whitespace_raw(fptr, tokens);

        case WS_KEY(TAB, LF):
            val = WS_FC_TL;
            return dyn_array_push_back(tokens, &val);

        case WS_KEY(LF, LF):
            val = WS_FC_LL;
            return dyn_array_push_back(tokens, &val);
        
        default:
            fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace Flow Control command: (ASCII: %u %u)\n", (unsigned char)c1, (unsigned char)c2);
            return EXIT_FAILURE;
    }
}

int tokenize_whitespace(FILE *fptr, DynArray **tokens, DynArray **labels){
    *tokens = (DynArray *)dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(Whitespace_Op));
    *labels = (DynArray *)dyn_array_init(DEFAULT_CONTAINER_CAP, sizeof(Whitespace_Label));
    if(!*tokens || !*labels){
        dyn_array_free(*tokens);
        dyn_array_free(*labels);
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    
    int imp_c1, imp_c2;

    while((imp_c1 = fgetc(fptr)) != EOF){
        switch(imp_c1){
            case TAB:
                if(read_whitespace_command_char(fptr, &imp_c2) == EXIT_FAILURE){
                    dyn_array_free(*tokens);
                    dyn_array_free(*labels);
                    return EXIT_FAILURE;
                }

                unsigned int key = WS_KEY(imp_c1, imp_c2);
                switch(key){
                    case WS_KEY(TAB, LF):
                        if(tokenize_io(fptr, *tokens) == EXIT_FAILURE){
                            dyn_array_free(*tokens);
                            dyn_array_free(*labels);
                            return EXIT_FAILURE;
                        }

                        break;

                    case WS_KEY(TAB, SPACE):
                        if(tokenize_arithmetic(fptr, *tokens) == EXIT_FAILURE){
                            dyn_array_free(*tokens);
                            dyn_array_free(*labels);
                            return EXIT_FAILURE;
                        }

                        break;

                    case WS_KEY(TAB, TAB):
                        if(tokenize_heap(fptr, *tokens) == EXIT_FAILURE){
                            dyn_array_free(*tokens);
                            dyn_array_free(*labels);
                            return EXIT_FAILURE;
                        }

                        break;

                    default:
                        fprintf(stderr, "Unrecognised character sequence while tokenizing whitespace IMP: (ASCII: %u %u)\n", (unsigned char)imp_c1, (unsigned char)imp_c2);
                        dyn_array_free(*tokens);
                        dyn_array_free(*labels);
                        return EXIT_FAILURE;
                }
                break;

            case SPACE:
                if(tokenize_stack_manip(fptr, *tokens) == EXIT_FAILURE){
                    dyn_array_free(*tokens);
                    dyn_array_free(*labels);
                    return EXIT_FAILURE;
                }
                break;

            case LF:
                if(tokenize_flow_control(fptr, *tokens, *labels) == EXIT_FAILURE){
                    dyn_array_free(*tokens);
                    dyn_array_free(*labels);
                    return EXIT_FAILURE;
                }
                break;
        }
    }

    return EXIT_SUCCESS;
}