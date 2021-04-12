#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#define BUF_SIZE 1

int main(int argc, char **argv)
{
    int i, N, c=0, sum=0;
    int fd[2];
    char s[BUF_SIZE];

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0)
    {
        fprintf(stderr, "Cannot open file\n");
        fd1 = 0;
    }
    if (argv[2] != NULL)
        N=atoi(argv[2]);
    else
    {
        fprintf(stderr, "Error\n");
        return(2);
    }

    for (i=0; i<=N; i++)
    {
        pipe(fd);
        switch (fork())
        {
            case -1: 
                perror("Basta\n"); exit(3);
            case 0:
            if (i==0)
            {
                dup2(fd[1], 1);
                while ((read(fd1, s, 1)) != 0)
                    printf("%s", s);
                close(fd[0]);   close(fd[1]);   close(fd1);
                exit(0);
            }
            else
            {
                while ((read(0, s, 1)) != 0)
                {   // Проверял программу, вводя символы со стандартного ввода, поэтому чтение и обработка такие
                    if (!isspace(s[0]))
                        c=c*10+atoi(s);
                    else {if (c % i == 0) sum+=c; c=0;}
                    write(fd[1], s, 1);
                }
                close(fd[0]);   close(fd[1]);   close(fd1);
                fprintf(stderr, "PROCESS %d: %d\n", i, sum);
                exit(0);
            }
        }
        dup2(fd[0], 0);
        close(fd[0]);   close(fd[1]);
    }
    while(wait(NULL) != -1) ;


    return(0);
}