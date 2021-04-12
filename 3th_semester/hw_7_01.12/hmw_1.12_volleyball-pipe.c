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
#define MAX_CNT 10000 //Игра длится примерно 2-4 секунды на моём компе
int curnum, status;
int fd0[2], fd1[2], fd2[2];

int main(int argc, char **argv)
{
    pipe(fd0);  pipe(fd1);  pipe(fd2);
    curnum=0;

    if (fork() == 0)
    {
        close(fd0[0]);  close(fd0[1]);  close(fd1[1]);  close(fd2[0]);  
        while(curnum < MAX_CNT)
        {   //printf("1: |%d|\t", curnum);
            read(fd1[0], &curnum, sizeof(int));
            curnum++;
            write(fd2[1], &curnum, sizeof(int));
        }
        fprintf(stderr, "Child ends\n");
        close(fd1[0]);  close(fd2[1]);
        exit(0);
    }
    else if (fork() == 0)
    {
        close(fd0[0]);  close(fd1[0]);  close(fd1[1]);  close(fd2[1]);
        while(curnum < MAX_CNT)
        {   //printf("2: |%d|\t", curnum);
            read(fd2[0], &curnum, sizeof(int));
            curnum++;
            write(fd0[1], &curnum, sizeof(int));
        }
        fprintf(stderr, "Child ends\n");
        close(fd0[1]);  close(fd2[0]);
        exit(0);
    }
    else
    {
        close(fd0[1]);  close(fd1[0]);  close(fd2[0]);  close(fd2[1]);
        write(fd1[1], &curnum, sizeof(int));
        while(curnum < MAX_CNT)
        {   //printf("Par: |%d|\t", curnum);
            read(fd0[0], &curnum, sizeof(int));
            curnum++;
            write(fd1[1], &curnum, sizeof(int));
        }
        fprintf(stderr, "Parent ends\n");
        close(fd0[0]);  close(fd1[1]);  
        while(wait(NULL) != -1) ;
        return(0);
    }

    return(0);
}