#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
//Игра завершается по нажатию ^C, так как его обработка установлена по умолчанию
#define MAX_CNT 100000 //Игра длится примерно 2-4 секунды на моём компе
int target_pid, curnum, status, pid, ppid;
int fd[2];

void SigHndlr (int s)
{
    signal(SIGUSR1, SigHndlr);

    if (curnum < MAX_CNT)
    {   //Три процесса играют
        read(fd[0], &curnum, sizeof(int));
        //printf("|%d|\t", curnum);
        curnum++;
        write(fd[1], &curnum, sizeof(int));
        kill(target_pid, SIGUSR1);
    }
    else if (getpid() != ppid)   
    {   //Сын завершается, получив соответствующий сигнал от отца или если переполнился счётчик
        fprintf(stderr, "Child ends\n");
        kill(target_pid, SIGUSR1);
        close(fd[1]); close(fd[0]);
        exit(0);
    }
    else
        //Отец, заметив переполнение счётчика посылает сигнал о завершении сыну
        kill(target_pid, SIGUSR1);
}

int main(int argc, char **argv)
{
    ppid = getpid();
    pipe(fd);
    signal(SIGUSR1, SigHndlr);
    curnum=0;

    if ((target_pid = fork()) == 0)
    {
        read(fd[0], &target_pid, sizeof(int));  //printf("1 my: %d target: %d\n", getpid(), target_pid);
        write(fd[1], &curnum, sizeof(int));
        kill(target_pid, SIGUSR1);   // Начинаем воллейбол
        while(1) pause();
    }
    else if (fork() == 0)
    {
        target_pid = getppid();
        pid = (int)getpid();                    //printf("2 my: %d target: %d\n", getpid(), target_pid);
        write(fd[1], &pid, sizeof(int));
        while(1) pause();
    }
    else
    {                                           //printf("PAR my: %d target: %d\n", getpid(), target_pid);
        while (wait(&status) == -1) ;
        fprintf(stderr, "Parent ends\n");
        close(fd[1]); close(fd[0]);
        return(0);
    }

    return(0);
}
