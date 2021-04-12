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
    int i, j, fd1, stat;
    int fd[2];

    //fprintf(stderr, "%d", argc);
    if (argc < 5)
    {   fprintf(stderr, "Error!");    return(1);  }

    pipe(fd);
    if (fork() == 0)
    {
        dup2(fd[1], 1);
        close(fd[1]);   close(fd[0]);
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "Error2!"); 
        exit(1);
    }
    if (fork() == 0)
    {
        dup2(fd[0], 0);
        close(fd[1]);   close(fd[0]);
        execlp(argv[2], argv[2], NULL);
        fprintf(stderr, "Error3!"); 
        exit(1);
    }
    close(fd[1]);   close(fd[0]);
    while(wait(&stat) != -1) ;

    if (fork() == 0)
    {
        fd1 = open(argv[4], O_WRONLY|O_TRUNC|O_CREAT, 0777);
        dup2(fd1, 1);   close(fd1);
        execlp(argv[3], argv[3], NULL);
        fprintf(stderr, "Error4!"); 
        exit(1);
    }
    while(wait(&stat) != -1) ;


    return(0);
}