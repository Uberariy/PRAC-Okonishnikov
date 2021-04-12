#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
pid_t pidarray[3];
pid_t pid, p, p2;
int j, i, notblock, start;

void SigHndlr (int sig)
{
    if (pid == getpid())
    {
        if (!start) kill(pidarray[j], SIGUSR1);
        j = (j+1) % 3;
        kill(pidarray[j], SIGUSR1);
        start=0;
    }
    signal(SIGINT, SigHndlr);
}

void SigUsr(int sig)
{
    notblock = (notblock+1) % 2;
    signal(SIGUSR1, SigUsr);
}

void SigAlrmmm (int sig)
{   
    for (i=0; i<3; i++) kill(pidarray[i], SIGKILL);
    kill(getpid(), SIGKILL);
}

int main(int argc, char *argv[])
{
    int stat;

    j=-1;    start=1;   notblock=0;
    pid=getpid();
    signal(SIGALRM, SigAlrmmm);    signal(SIGINT, SigHndlr);   signal(SIGUSR1, SigUsr);
    for (i=0; i<3; i++)
    {
        if ((p = fork()) == 0)
        {
            p2 = getpid();
            while(1)
            {
                if (notblock)
                    printf("My PID: %d\n", p2);
                sleep(1);
            }
            exit(0);
        }
        pidarray[i] = p;
    }
    alarm(10);
    while(1) pause();

    return(0);
}