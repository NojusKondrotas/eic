#ifndef EIC_H
#define EIC_H

#define BF 5863245 // Brainfuck, Befunge-93
#define B 177671 // Brainfuck

#define WS 5863951 // Whitespace

#define BE 5863244 // Befunge-93
#define B93 193485651 // Befunge-93
#define BEFUNGE 229460483486753 // Befunge-93

#define B98 193485656 // Funge-98
#define TF 5863839 // Funge-98
#define UF 5863872 // Funge-98

char *get_extension(char *str);

size_t hash_string(char *str);

int main(int argc, char *argv[]);

#endif