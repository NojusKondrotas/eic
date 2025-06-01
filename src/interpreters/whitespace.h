#define SPACE 32
#define TAB 9
#define LF 10

int execute_whitespace(char *);

void handle_flow_control(FILE *, ssize_t *, Label *);

int handle_stack_manip(FILE *, ssize_t **, size_t *, size_t *);

void handle_io(ssize_t *, ssize_t *);

void handle_arithmetic(ssize_t *);