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
// pr1 arg1 arg2 | pr2 ; pr3 >> f1.dat
// ./12.1 sh12.1.1 arg1 arg2 u sh12.1.2 k sh12.1.3 dd sh12.1.3.txt
int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid, p;
    int stat, i=1;

    pipe(fd);
    if ((p = fork()) == 0)
    {                                       //printf("%s\n", argv[1]);
        dup2(fd[1], 1);
        close(fd[1]);   close(fd[0]);
        execl(argv[1], argv[1], argv[2], argv[3], NULL);
        perror("exec1!!!");
        exit(1);
    }
    else if (p == -1)
    {
        perror("Basta1");
        exit(2);
    }
    else if ((p = fork()) == 0)
    {                                       //printf("%s\n", argv[5]);
        dup2(fd[0], 0);
        close(fd[1]);   close(fd[0]);
        execl(argv[5], argv[5], NULL);            
        perror("exec2!!!");
        exit(3);
    } 
    else if (p == -1)
    {
        perror("Basta2");
        exit(4);
    }
    close(fd[1]);   close(fd[0]);
    while(wait(NULL) != -1) ;

    if ((p = fork()) == 0)
    {
        int fd1 = open(argv[9], O_APPEND | O_WRONLY | O_CREAT, 0777);
        if (fd1 < 0)
        {
            fprintf(stderr, "Cannot open file\n");
            exit(5);
        }
        dup2(fd1, 1);           //printf("%s\n", argv[7]);
        execl(argv[7], argv[7], NULL); 
        perror("exec3!!!");
        close(fd1);
    }
    else if (p == -1)
    {
        perror("Basta2");
        exit(6);
    }
    while(wait(NULL) != -1) ;

    return(0);
}