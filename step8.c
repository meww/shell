#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLEN 256
#define MAXSPLIT 80 

void splitCmd(char *cmd, int *ac, char* av[]);
void strReplace(char* av, char* path, int ac);
void execCmd(char* av[], int ac, char* path);

int main(void)
{
    int ac;
    char cmd[MAXLEN];
    char path[MAXLEN];
    getcwd(path, MAXLEN);

    while (1) {
        char *av[MAXSPLIT] = {0};
        printf("mysh $ ");
        if (fgets(cmd, MAXLEN, stdin) == NULL) {
            printf("Input Error\n");
        }
        splitCmd(cmd, &ac, av);
        execCmd(av, ac, path);
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

void strReplace(char* av, char* path, int ac)
{
    char tmp[MAXLEN];
    char *p;
    int i;
    for (i = 0; i < ac; i++) {
        while ((p = strstr(av, "~")) != NULL) {
            *p = '\0';
            p += 1;
            strcpy(tmp, p);
            strcat(av, path);
            strcat(av, tmp);
        }
    }
}

void execCmd(char *av[], int ac, char *path)
{
    char* argv[MAXSPLIT] = {0};
    int i;
    int num_in = 0; 
    int num_out = 0;
    int fd_in, fd_out;
    pid_t child;
    int status;

    if (strcmp(av[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(av[0], "cd") == 0) {
        if (av[1] == NULL) {
            chdir(path);
        } else {
            strReplace(av[1], path, ac);
            chdir(av[1]);
        }
    }
    if ((child = fork()) < 0) {
        printf("fork error\n");
        exit(1);
    }
    if (child == 0) {
        for (i = 0; i < ac; i++) {
            if (strcmp(av[i], ">") == 0) {
                num_out = i;
            }
            if (strcmp(av[i], "<") == 0) {
                num_in = i;
            }
        }
        if (num_in == 0 && num_out == 0) {
            execvp(av[0], av);
        } else {
            if (num_in != 0) {
                /* error check */
                fd_in = open(av[num_in + 1], O_RDONLY);
                close(0);
                dup(fd_in);
                close(fd_in);
                if (num_out == 0) {
                    for (i = 0; i < num_in; i++) {
                        argv[i] = av[i];
                    }
                    argv[i] = NULL;
                }
            }
            if (num_out != 0) {
                /* error check */
                fd_out = open(av[num_out + 1], 
                    O_WRONLY|O_CREAT|O_TRUNC, 0644);
                close(1);
                dup(fd_out);
                close(fd_out);
                if (num_in == 0) {
                    for (i  = 0; i < num_out; i++) {
                        argv[i] = av[i];
                    }
                    argv[i] = NULL;
                }
            }
            if (num_in != 0 && num_out != 0) {
                if (num_in > num_out) {
                    for (i = 0; i < num_out; i++) {
                        argv[i] = av[i];
                    }
                    argv[i] = NULL;
                } else {
                    for (i = 0; i < num_in; i++) {
                        argv[i] = av[i];
                    }
                    argv[i] = NULL;
                }
            }
            execvp(argv[0], argv);
        } 
    } else {
        wait(&status);
    }
}
