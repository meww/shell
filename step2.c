#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 256
#define MAXSPLIT 80 

void splitCmd(char *cmd, int *ac, char* av[]);
void print_args(int ac, char* av[]);

int main(void)
{
    int ac;
    char cmd[MAXLEN];
    while (1) {
        char *av[MAXSPLIT] = {0};
        printf("mysh $ ");
        if (fgets(cmd, MAXLEN, stdin) == NULL) {
            printf("Input Error\n");
        }
        splitCmd(cmd, &ac, av);
        print_args(ac, av);
    }
    return 0;
}

void splitCmd(char *cmd, int *ac, char* av[]) 
{
    int i = 0;
    *ac = 0;
    while (1) {
        if (cmd[i] == '\0') break;
        if (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n') {
            cmd[i] = '\0'; 
        } else {
            if (cmd[i - 1] == ' ' || cmd[i - 1] == '\t' || 
                    cmd[i - 1] == '\0' || i == 0) {
                av[*ac] = &cmd[i];
                *ac = *ac + 1;
            }
        }
        i++;
    }
}

void print_args(int ac, char* av[]) {
    int i = 0;
    for (i = 0; i < ac; i++) {
        printf ("%s\n", av[i]);
    }
}
