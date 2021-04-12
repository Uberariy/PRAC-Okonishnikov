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
int main(int argc, char *argv[])     // ; заменены на p
{
    pid_t pid, p;
    int stat, i=1, cCrash=0, cOC=0, cPRandOC=0;

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
        i+=2;
        if ((!argv[i-1]) || (strcmp(argv[i-1],"p") != 0)) break;
    }

    while (wait(&stat) != -1)
    {
        if (WIFEXITED(stat) == 0) cPRandOC++;
        else if (WEXITSTATUS(stat) == 0) cCrash++;
        else cOC++;
    }

    printf("\ncCrash: %d\tcOC: %d\tcPRandOC: %d\tsum: %d\n", cCrash, cOC, cPRandOC, (cCrash + cOC + cPRandOC));
    return(0);
}