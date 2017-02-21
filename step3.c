#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 256
#define MAXSPLIT 80

void splitCmd(char *cmd, int *ac, char* av[]);
void print_args(int ac, char* av[]);
int countPipe(int ac, char* av[]);
void splitProc(char* av[], int *Pac, char* Pav[], int *count, int ac);

int main(void)
{
    int ac, Pac;
    int countProc;
    int count;
    int i;
    char cmd[MAXLEN];
    while (1) {
        count = 0;
        char *av[MAXSPLIT] = {0};
        char *Pav[MAXSPLIT] = {0};
        printf("mysh $ ");
        if (fgets(cmd, MAXLEN, stdin) == NULL) {
            printf("Input Error\n");
        }
        splitCmd(cmd, &ac, av);
        countProc = countPipe(ac, av) + 1;
        printf("Total Process = %d\n", countProc);
        for (i = 0; i < countProc; i++) {
            printf("Proc %d\n", i + 1);
            splitProc(av, &Pac, Pav, &count, ac);
            print_args(Pac, Pav);
        }
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

void print_args(int Pac, char* Pav[]) 
{
    int i = 0;
    printf("Pac = %d\n", Pac);
    for (i = 0; i < Pac; i++) {
        printf ("Pav[%d] = %s\n", i, Pav[i]);
    }
}

int countPipe(int ac, char* av[])
{
    int countPipe = 0;
    int i = 0;
    for (i = 0; i < ac; i++) {
        if (*av[i] == '|') {
            countPipe++;
        } 
    }
    return countPipe;
}

void splitProc(char* av[], int *Pac, char* Pav[], int* count, int ac)
{
    *Pac = 0;
    while (1) {
        if (*av[*count] == '|') {
            *count += 1;
            break;
        } else {
            Pav[*Pac] = av[*count];
            *Pac += 1;
            if (*count == ac - 1) {
                break;
            }
            *count += 1;
        }
    }
}

