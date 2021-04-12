#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
// Считаем, что аргументов командной строки передано столько, сколько надо, а служебные символы стоят правильно и разделены пробеломи с названиями
// pr1 < f1.dat | pr2 > f2.dat ; pr3
// ./12.1b sh12.1b.2 u sh12.1b.1.txt l sh12.1b.2 b sh12.1b.2.txt dd sh12.1b.3
int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid, p;
    int stat, i=1;

    pipe(fd);
    if ((p = fork()) == 0)
    {
        int fd1 = open(argv[3], O_RDONLY);
        if (fd1 < 0)
        {
            fprintf(stderr, "Cannot open file1\n");
            exit(1);
        }
        dup2(fd1, 0);           //printf("%s\n", argv[7]);
        dup2(fd[1], 1);
        close(fd[1]);   close(fd[0]);
        execl(argv[1], argv[1], NULL); 
        perror("exec1!!!");
        close(fd1);
        exit(2);
    }
    else if (p == -1)
    {
        perror("Basta1");
        exit(3);
    }
    else if ((p = fork()) == 0)
    {
        int fd2 = open(argv[7], O_WRONLY | O_CREAT, 0777);
        if (fd2 < 0)
        {
            fprintf(stderr, "Cannot open file2\n");
            exit(4);
        }
        dup2(fd2, 1);           
        dup2(fd[0], 0);
        close(fd[1]);   close(fd[0]);
        execl(argv[5], argv[5], NULL); 
        perror("exec2!!!");
        close(fd2);
        exit(5);
    }
    else if (p == -1)
    {
        perror("Basta2");
        exit(6);
    }
    close(fd[1]);   close(fd[0]);
    while(wait(NULL) != -1) ;

    if ((p = fork()) == 0)
    {
        execl(argv[9], argv[9], NULL); 
        perror("exec3!!!");
        exit(7);
    }
    else if (p == -1)
    {
        perror("Basta3");
        exit(8);
    }
    while(wait(NULL) != -1) ;

    return(0);
}