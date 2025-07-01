#ifndef EIC_H
#define EIC_H

#define BF 5863245 // Brainfuck, Befunge
#define B 177671 // Brainfuck

#define WS 5863951 // Whitespace

#define BE 5863244 // Funge
#define B93 193485651 // Funge-93
#define BEFUNGE 229460483486753 // Funge

#define B96 193485654 // Funge-96

#define B97 193485655 // Funge-97

#define B98 193485656 // Funge-98
#define UF 5863872 // Unefunge
#define TF 5863839 // Trefunge

typedef enum e_funge_flags{
    Funge93_Flag = 1,
    Funge96_Flag = 2,
    Funge97_Flag = 4,
    Funge98_Flag = 8,
    Unefunge_Flag = 16,
    Befunge_Flag = 32,
    Trefunge_Flag = 64
}FungeFlags;

typedef enum e_eic_flags{
    EIC_IgnoreExtensions_Flag = 1,
    EIC_Brainfuck_Flag = 2,
    EIC_Funge_Flag = 4,
    EIC_WhiteSpace_Flag = 8
}EICFlags;

int define_flag(char *str, EICFlags *eic_flags, FungeFlags *funge_exec_flags, int *funge_exec_dimensions);

int check_flag_conflict(EICFlags eic_flags, FungeFlags funge_exec_flags, int funge_exec_dimensions);

char *get_extension(char *str);

size_t hash_string(char *str);

int main(int argc, char *argv[]);

#endif