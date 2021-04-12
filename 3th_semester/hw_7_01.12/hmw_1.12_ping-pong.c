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
int target_pid, curnum, status;
int fd[2];

void SigHndlr (int s)
{
    signal(SIGUSR1, SigHndlr);

    if (curnum < MAX_CNT)
    {   //Отец и сын играют
        read(fd[0], &curnum, sizeof(int));
        //printf("%d\t", curnum);
        curnum++;
        write(fd[1], &curnum, sizeof(int));
        kill(target_pid, SIGUSR1);
    }
    else if (target_pid == getppid())   
    {   //Сын завершается, получив соответствующий сигнал от отца или если переполнился счётчик
        fprintf(stderr, "Child ends\n");
        close(fd[1]); close(fd[0]);
        exit(0);
    }
    else
        //Отец, заметив переполнение счётчика посылает сигнал о завершении сыну
        kill(target_pid, SIGUSR1);
}

int main(int argc, char **argv)
{
    pipe(fd);
    signal(SIGUSR1, SigHndlr);
    curnum=0;

    if ((target_pid = fork()) == 0)
    {
        target_pid = getppid();
        write(fd[1], &curnum, sizeof(int));
        kill(target_pid, SIGUSR1);   // Начинаем пин-понг
        while(1) pause();
    }
    else
    {
        while (wait(&status) == -1) ;
        fprintf(stderr, "Parent ends\n");
        close(fd[1]); close(fd[0]);
        return(0);
    }

    return(0);
}