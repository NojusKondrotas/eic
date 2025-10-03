#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/eic.h"
#include "include/brainfuck.h"
#include "include/funge.h"
#include "include/funge_lexer.h"
#include "include/whitespace.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "No source file\n");
        return EXIT_FAILURE;
    }

    EICFlags eic_flags = 0;

    FungeFlags funge_exec_flags = 0;
    int funge_exec_dimension = 0;
    
    int source_idx = -1;
    char *extension = NULL;
    int tmp;

    for(int i = 1; i < argc; ++i){
        if(argv[i][0] == '-'){
            if(define_flag(++argv[i], &eic_flags, &funge_exec_flags, &funge_exec_dimension) == EXIT_FAILURE)
                return EXIT_FAILURE;
        }
        else{
            if(*(extension = get_extension(argv[i])) == NULL){
                fprintf(stderr, "File does not contain an extension: %s\n", argv[i]);
                return EXIT_FAILURE;
            }
            source_idx = i;
        }
    }

    if(source_idx == -1){
        fprintf(stderr, "Source file unspecified\n");
        return EXIT_FAILURE;
    }

    size_t hash_result = hash_string(extension);

    if(check_flag_conflict(eic_flags, funge_exec_flags, funge_exec_dimension) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(eic_flags & EIC_IgnoreExtensions_Flag){
        if(eic_flags & EIC_Brainfuck_Flag) return execute_brainfuck(argv[source_idx]);
        else if(eic_flags & EIC_WhiteSpace_Flag) return execute_whitespace(argv[source_idx]);
        else if(eic_flags & EIC_Funge_Flag){
            tmp = funge_exec_flags & (Funge93_Flag | Funge96_Flag | Funge97_Flag | Funge98_Flag);
            switch(tmp){
                case Funge93_Flag: hash_result = B93; break;
                case Funge96_Flag: hash_result = B96; break;
                case Funge97_Flag: hash_result = B97; break;
                case Funge98_Flag: hash_result = B98; break;
                default:
                    fprintf(stderr, "Funge version must be specified when ignoring extension dependency and setting the --funge EIC flag\n");
                    return EXIT_FAILURE;
            }
        }
        else{
            fprintf(stderr, "An EIC language flag must be set when ignoring extension dependency\n");
            return EXIT_FAILURE;
        }
    }
    
    switch (hash_result)
    {
    case BF:
        tmp = eic_flags & (EIC_Brainfuck_Flag | EIC_Funge_Flag);
        if(tmp != EIC_Brainfuck_Flag && tmp != EIC_Funge_Flag){
            fprintf(stderr, "Ambiguity between Brainfuck and Funge execution when running a .bf file. Set --funge or --brainfuck EIC flag or any funge dialect flag to remove ambiguity\n");
            return EXIT_FAILURE;
        }
        else if(tmp == EIC_Brainfuck_Flag) return execute_brainfuck(argv[source_idx]);
        else{
            tmp = funge_exec_flags & (Funge93_Flag | Funge96_Flag | Funge97_Flag | Funge98_Flag);
            if(tmp == 0) funge_exec_flags |= Funge93_Flag;
            if(funge_exec_dimension == 0){ funge_exec_dimension = 2; funge_exec_flags |= Befunge_Flag; }
            return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
        }

        break;
    case B:
        return execute_brainfuck(argv[source_idx]);
    case WS:
        return execute_whitespace(argv[source_idx]);
    case BE:
    case BEFUNGE:
        tmp = funge_exec_flags & (Funge93_Flag | Funge96_Flag | Funge97_Flag | Funge98_Flag);
        if(!tmp) funge_exec_flags |= Funge93_Flag;
        if(!funge_exec_dimension){
            funge_exec_flags |= Befunge_Flag;
            funge_exec_dimension = 2;
        }
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    case B93:
        if((funge_exec_flags & Funge93_Flag) == 0){
            fprintf(stderr, "Funge version must be '93 when running a .b93 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            if(!funge_exec_dimension){
                funge_exec_dimension = 2;
                funge_exec_flags |= Befunge_Flag;
            }
        }
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    case B96:
        if((funge_exec_flags & Funge96_Flag) == 0){
            fprintf(stderr, "Funge version must be '96 when running a .b96 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if(!funge_exec_dimension){
            funge_exec_dimension = 2;
            funge_exec_flags |= Befunge_Flag;
        }
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    case B97:
        if((funge_exec_flags & Funge97_Flag) == 0){
            fprintf(stderr, "Funge version must be '97 when running a .b97 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if(!funge_exec_dimension){
            funge_exec_dimension = 3;
            funge_exec_flags |= Trefunge_Flag;
        }
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    case B98:
        if((funge_exec_flags & Funge98_Flag) == 0){
            fprintf(stderr, "Funge version must be '98 when running a .b98 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if(!funge_exec_dimension){
            funge_exec_dimension = 3;
            funge_exec_flags |= Trefunge_Flag;
        }
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    case UF:
        if(funge_exec_dimension > 1){
            fprintf(stderr, "Funge operational dimensions cannot be more than 1D when running a .uf file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        funge_exec_dimension = 1;
        funge_exec_flags &= Unefunge_Flag;
        if((funge_exec_flags & 15) == 0) funge_exec_flags |= Funge93_Flag;
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    case TF:
        if((funge_exec_dimension != 3 || funge_exec_dimension != 0)){
            fprintf(stderr, "Funge operational dimensions must be 3D when running a .tf file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        funge_exec_dimension = 3;
        funge_exec_flags &= Trefunge_Flag;
        if((funge_exec_flags & 15) == 0) funge_exec_flags |= Funge98_Flag;
        return execute_funge(argv[source_idx], funge_exec_flags, funge_exec_dimension);
    default:
        fprintf(stderr, "Unsupported file extension: %s\n", extension);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int define_flag(char *str, EICFlags *eic_flags, FungeFlags *funge_exec_flags, int *funge_exec_dimension){
    size_t length = strlen(str);
    if(str[0] != '-'){
        for(size_t i = 0; i < length; ++i){
            switch(str[i]){
                case '3':
                    (*funge_exec_flags) |= Funge93_Flag;
                    break;
                case '6':
                    (*funge_exec_flags) |= Funge96_Flag;
                    break;
                case '7':
                    (*funge_exec_flags) |= Funge97_Flag;
                    break;
                case '8':
                    (*funge_exec_flags) |= Funge98_Flag;
                    break;
                default:
                    fprintf(stderr, "Unknown EIC interpreter flag: -%c (ASCII: %d)\n", str[i], str[i]);
                    return EXIT_FAILURE;
            }
        }
    }
    else if(strcmp(str, "-ignoreext") == 0){
        (*eic_flags) |= EIC_IgnoreExtensions_Flag;
    }
    else if(strcmp(str, "-brainfuck") == 0){
        (*eic_flags) |= EIC_Brainfuck_Flag;
    }
    else if(strcmp(str, "-funge") == 0){
        (*eic_flags) |= EIC_Funge_Flag;
    }
    else if(strcmp(str, "-whitespace") == 0){
        (*eic_flags) |= EIC_WhiteSpace_Flag;
    }
    else if(strcmp(str, "-funge93") == 0){
        (*funge_exec_flags) |= Funge93_Flag;
    }
    else if(strcmp(str, "-funge96") == 0){
        (*funge_exec_flags) |= Funge96_Flag;
    }
    else if(strcmp(str, "-funge97") == 0){
        (*funge_exec_flags) |= Funge97_Flag;
    }
    else if(strcmp(str, "-funge98") == 0){
        (*funge_exec_flags) |= Funge98_Flag;
    }
    else if(strcmp(str, "-unefunge") == 0){
        (*funge_exec_flags) |= Unefunge_Flag;
        (*funge_exec_dimension) = 1;
    }
    else if(strcmp(str, "-befunge") == 0){
        (*funge_exec_flags) |= Befunge_Flag;
        (*funge_exec_dimension) = 2;
        
    }
    else if(strcmp(str, "-trefunge") == 0){
        (*funge_exec_flags) |= Trefunge_Flag;
        (*funge_exec_dimension) = 3;
    }
    else if(strlen(str) >= 11 && strncmp(str, "-dimension=", 11) == 0){
        if(strlen(str) < 12){
            fprintf(stderr, "--dimensions flag value must be set\n");
            return EXIT_FAILURE;
        }
        size_t idx = 11;
        if(str[idx] == '-'){
            fprintf(stderr, "--dimensions flag value must be positive\n");
            return EXIT_FAILURE;
        }
        while(str[idx] > 47 && str[idx] < 58){
            (*funge_exec_dimension) *= 10;
            (*funge_exec_dimension) += str[idx] - '0';
            ++idx;
        }
        if(*funge_exec_dimension == 0){
            fprintf(stderr, "--dimensions flag value must be positive\n");
            return EXIT_FAILURE;
        }
    }
    else{
        fprintf(stderr, "Unknown EIC interpreter flag: -%s\n", str);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int check_flag_conflict(EICFlags eic_flags, FungeFlags funge_exec_flags, int funge_exec_dimension){
    int op = eic_flags & (EIC_Brainfuck_Flag | EIC_Funge_Flag | EIC_WhiteSpace_Flag);
    if(op != 0){
        if(op != EIC_Brainfuck_Flag && op != EIC_Funge_Flag && op != EIC_WhiteSpace_Flag){
            fprintf(stderr, "Mutliple EIC language flags set, incompatibility\n");
            return EXIT_FAILURE;
        }
    }

    op = funge_exec_flags & (Unefunge_Flag | Befunge_Flag | Trefunge_Flag);
    if(op != 0){
        if(op != Unefunge_Flag && op != Befunge_Flag && op != Trefunge_Flag){
            fprintf(stderr, "Mutliple Funge dimension flags set, incompatibility\n");
            return EXIT_FAILURE;
        }
    }

    op = funge_exec_flags & (Funge93_Flag | Funge96_Flag | Funge97_Flag | Funge98_Flag);
    if(op != 0){
        if(op != Funge93_Flag && op != Funge96_Flag && op != Funge97_Flag && op != Funge98_Flag){
            fprintf(stderr, "Mutliple or none Funge version flags set, incompatibility\n");
            return EXIT_FAILURE;
        }
    }
    
    if(((funge_exec_flags & Funge93_Flag) || (funge_exec_flags & Funge96_Flag)) && funge_exec_dimension > 2){
        fprintf(stderr, "'93 and '96 versions of Funge cannot operate on more than 2 dimensions\n");
        return EXIT_FAILURE;
    }

    if((funge_exec_dimension != 1 && (funge_exec_flags & (Unefunge_Flag)))
        || (funge_exec_dimension != 2 && (funge_exec_flags & (Befunge_Flag)))
        || (funge_exec_dimension != 3 && (funge_exec_flags & (Trefunge_Flag)))){
        fprintf(stderr, "Ambiguity between Funge operational dimensions. Check if --unefunge, --befunge, or --trefunge are not set together with --dimensions\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

char *get_extension(char *str){
    int count = 0, length = 0;
    while(*str != '\0'){
        ++count;
        ++length;
        ++str;
    }

    while(count-- > -1){
        if(*str == '.')
            return ++str;

        --str;
    }

    return NULL;
}

size_t hash_string(char *str){
    size_t hash = 5381;
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c;
    
    return hash;
}