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
        fprintf(stderr, "No input file\n");
        return EXIT_FAILURE;
    }

    size_t eic_flags = 0;

    size_t funge_exec_flags = 0;
    int funge_exec_dimensions = 0;
    
    char *extension = NULL;

    for(int i = 1; i < argc; ++i){
        if(argv[i][0] == '-'){
            if(define_flag(++argv[i], &eic_flags, &funge_exec_flags, &funge_exec_dimensions) == EXIT_FAILURE)
                return EXIT_FAILURE;
        }
        else{
            if(*(extension = get_extension(argv[i])) == '\0'){
                fprintf(stderr, "File does not contain an extension: %s\n", argv[i]);
                return EXIT_FAILURE;
            }
        }
    }

    size_t hash_result = hash_string(extension);

    if(check_flag_conflict(eic_flags, funge_exec_flags, funge_exec_dimensions) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    switch (hash_result)
    {
    case BF:
        //TO-DO: request additional confirmation concerning confusion between the execution of Brainfuck or Befunge-93
        break;
    case B:
        return execute_brainfuck(extension);
    case WS:
        return execute_whitespace(extension);
    case BE:
    case BEFUNGE:
        if((funge_exec_flags & 15) == 0) funge_exec_flags |= Funge93_Flag;
        if(!funge_exec_dimensions){
            funge_exec_flags |= Befunge_Flag;
            funge_exec_dimensions = 2;
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    case B93:
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0 && (funge_exec_flags & Funge93_Flag) == 0){
            fprintf(stderr, "Funge version must be 93' when running a .b93 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            if(!funge_exec_dimensions){
                funge_exec_dimensions = 2;
                funge_exec_flags |= Befunge_Flag;
            }
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    case B96:
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0 && (funge_exec_flags & Funge96_Flag) == 0){
            fprintf(stderr, "Funge version must be 96' when running a .b96 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            if(!funge_exec_dimensions){
                funge_exec_dimensions = 2;
                funge_exec_flags |= Befunge_Flag;
            }
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    case B97:
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0 && (funge_exec_flags & Funge97_Flag) == 0){
            fprintf(stderr, "Funge version must be 97' when running a .b97 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            if(!funge_exec_dimensions){
                funge_exec_dimensions = 3;
                funge_exec_flags |= Trefunge_Flag;
            }
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    case B98:
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0 && (funge_exec_flags & Funge98_Flag) == 0){
            fprintf(stderr, "Funge version must be 98' when running a .b98 file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            if(!funge_exec_dimensions){
                funge_exec_dimensions = 3;
                funge_exec_flags |= Trefunge_Flag;
            }
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    case UF:
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0 && funge_exec_dimensions > 1){
            fprintf(stderr, "Funge operational dimensions cannot be more than 1D when running a .uf file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            funge_exec_dimensions = 1;
            funge_exec_flags &= Unefunge_Flag;
            if((funge_exec_flags & 15) == 0) funge_exec_flags |= Funge93_Flag;
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    case TF:
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0 && (funge_exec_dimensions != 3 || funge_exec_dimensions != 0)){
            fprintf(stderr, "Funge operational dimensions must be 3D when running a .tf file (P.S. specify --ignoreext EIC flag to ignore extension)\n");
            return EXIT_FAILURE;
        }
        if((eic_flags & EIC_IgnoreExtensions_Flag) == 0){
            funge_exec_dimensions = 3;
            funge_exec_flags &= Trefunge_Flag;
            if((funge_exec_flags & 15) == 0) funge_exec_flags |= Funge98_Flag;
        }
        return execute_funge(extension, funge_exec_flags, funge_exec_dimensions);
    default:
        fprintf(stderr, "Unsupported file extension: %s\n", extension);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int define_flag(char *str, size_t *eic_flags, size_t *funge_exec_flags, int *funge_exec_dimensions){
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
    else if(strcmp(str, "-funge93") == 0){
        (*funge_exec_flags) |= Funge93_Flag;
        (*funge_exec_flags) |= Befunge_Flag;
    }
    else if(strcmp(str, "-funge96") == 0)
        (*funge_exec_flags) |= Funge96_Flag;
    else if(strcmp(str, "-funge97") == 0)
        (*funge_exec_flags) |= Funge97_Flag;
    else if(strcmp(str, "-funge98") == 0)
        (*funge_exec_flags) |= Funge98_Flag;
    else if(strcmp(str, "-unefunge") == 0){
        (*funge_exec_flags) |= Unefunge_Flag;
        if((*funge_exec_dimensions) != 0){
            fprintf(stderr, "Cannot set the number of Funge operational dimensions when they have been already set\n");
            return EXIT_FAILURE;
        }
        (*funge_exec_dimensions) = 1;
    }
    else if(strcmp(str, "-befunge") == 0){
        (*funge_exec_flags) |= Befunge_Flag;
        if((*funge_exec_dimensions) != 0){
            fprintf(stderr, "Cannot set the number of Funge operational dimensions when they have been already set\n");
            return EXIT_FAILURE;
        }
        (*funge_exec_dimensions) = 2;
        
    }
    else if(strcmp(str, "-trefunge") == 0){
        (*funge_exec_flags) |= Trefunge_Flag;
        if((*funge_exec_dimensions) != 0){
            fprintf(stderr, "Cannot set the number of Funge operational dimensions when they have been already set\n");
            return EXIT_FAILURE;
        }
        (*funge_exec_dimensions) = 3;
    }
    else{
        fprintf(stderr, "Unknown EIC interpreter flag: -%s\n", str);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int check_flag_conflict(size_t eic_flags, size_t funge_exec_flags, int funge_exec_dimensions){
    int op = eic_flags ^ (EIC_Brainfuck_Flag | EIC_Funge_Flag | EIC_WhiteSpace_Flag);
    if(op == EIC_Brainfuck_Flag || op == EIC_Funge_Flag || op == EIC_WhiteSpace_Flag || op == 0){
        fprintf(stderr, "Mutliple EIC language flags set, incompatibility\n");
        return EXIT_FAILURE;
    }

    op = funge_exec_flags ^ (Unefunge_Flag | Befunge_Flag | Trefunge_Flag);
    if(op == Unefunge_Flag || op == Befunge_Flag || op == Trefunge_Flag || op == 0){
        fprintf(stderr, "Mutliple Funge dimension flags set, incompatibility\n");
        return EXIT_FAILURE;
    }

    op = (funge_exec_flags & 15);
    if(op != Funge93_Flag || op != Funge96_Flag || op != Funge97_Flag || op != Funge98_Flag){
        fprintf(stderr, "Mutliple Funge version flags set, incompatibility\n");
        return EXIT_FAILURE;
    }

    if((op & Funge93_Flag || op & Funge96_Flag) && funge_exec_dimensions > 2){
        fprintf(stderr, "93' and 96' versions of Funge cannot operate on more than 2 dimensions\n");
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