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
// Считаем, что аргументов командной строки передано столько, сколько надо
int main(int argc, char *argv[])     // ; заменены на u
{
    pid_t pid, p;
    int stat, i=1;

    while (i < argc)
    {
        if ((p = fork()) == 0)
        {
            execl(argv[i], argv[i], NULL);
            perror("exec1!!!");
            exit('1');
        }
        else if (p == -1)
        {
            perror("Basta");
            exit('2');
        }
        else pid = wait(&stat);
        i+=2;
        if ((!argv[i-1]) || (strcmp(argv[i-1],"u") != 0)) break;
    }
    return(0);
}