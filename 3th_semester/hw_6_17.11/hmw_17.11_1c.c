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
// pr1 | pr2 | ... | prn
// ./12.1c sh12.1c | sh12.1c | ... | sh12.1c
int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid, p;
    int stat, i=1;
    char c;

    for (i = 1; i < argc; i+=2)
    {
        pipe(fd);
        if ((p = fork()) == 0)
        {                                      // printf("%s\n", argv[i]);
            dup2(fd[1], 1);
            close(fd[1]);   close(fd[0]);
            execl(argv[i], argv[i], NULL);
            perror("exec!!!");
            exit(1);
        }
        else if (p == -1)
        {
            perror("Basta");
            exit(2);
        }
        dup2(fd[0], 0);
        close(fd[1]);   close(fd[0]);
    }
    while((c=getchar()) != EOF) putchar(c);
    while(wait(NULL) != -1) ;

    return(0);
}