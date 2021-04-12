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
int main(int argc, char *argv[])     // || заменены на kk
{
    pid_t pid, p;
    int stat;

    if (strcmp(argv[2],"kk") == 0)
        if ((p = fork()) == 0)
        {
            execl(argv[1], argv[1], NULL);
            perror("exec1!!!");
            exit('1');
        }
        else if (p == -1)
        {
            perror("Basta1");
            exit('1');
        }
        else
        {
            pid = wait(&stat);
            if ((WIFEXITED(stat) == 0) || (WEXITSTATUS(stat) == '1')) 
            {
                if (p = fork() == 0)
                {
                    execl(argv[3], argv[3], NULL);
                    perror("exec2!!!");
                    exit(2);
                }
                else if (p == -1)
                {
                    perror("Basta2");
                    exit(3);
                }
                else
                {
                    pid = wait(NULL);
                }
            }
        }
    
    return(0);
}