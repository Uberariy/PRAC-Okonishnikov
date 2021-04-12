#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pid_t pid, p;
    int i, j, fd1, stat, N, k;
    int fd[2];
    char **sargv;

    if (argc<2)
    {   fprintf(stderr, "Error!\n");    return(1);   }

    N = atoi(argv[1]);  //fprintf(stderr, "%d", N);
    sargv = (char **)malloc((N+2)*(sizeof(char *)));
    sargv[0] = (char *)malloc((6)*(sizeof(char)));
    strcpy(sargv[0], "echo");

    for (i = 1; i<=N; i++)
    {
        sargv[i] = (char *)malloc(sizeof(char)*(N/10+3));
    }
    for (i = 1; i<=N; i++)
    {
        k=i;
        sprintf(sargv[i], "%d", i);
    }
    sargv[N+1] = NULL;

    if (fork() == 0)
    {
        execvp(sargv[0], sargv);
        perror("exec!!");
        exit(1);
    }
    else (wait(&stat));

    for (i = 0; i<=N+1; i++)
    {
        free(sargv[i]);
    }
    free(sargv);

    return(0);
}