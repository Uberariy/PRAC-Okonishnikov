#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define RED "\x1b[31m"
#define WHITE "\x1b[38m"
#define PURPLE "\x1b[38;2;253;159;197m"
#define COLORENDS "\x1b[0m"
#define BACKGROUND_RED "\x1b[48;2;110;20;12m"

struct list
{
    char* word;
    int size;
    int number;
    int istoken;        // 1 if token, 0 if not
    struct list *next;
};

struct list *curlistadd (struct list *L, char *w, int s, int n, int t)
{
    if (L==NULL)
    {
        L = (struct list *)malloc(sizeof(struct list));
        L -> word = (char *)malloc(s+1);

        strcpy(L -> word, w);
        L -> number = n;
        L -> size = s;
        L -> istoken = t;                           //printf("|%s|%d\t", L -> word, L -> istoken);
        L -> next = NULL;
    }
    else L -> next = curlistadd (L -> next, w, s, n, t);
    return L;
}

void listTOsargv (struct list *L, char ****sargv, int i, int j)
{
    if (L != NULL)
    {                                               //printf("\ti:%d, j:%d, %s", i, j, L -> word);
        if (L -> istoken)
        {                                           //printf("\n");
            free((*sargv)[i][j]);     (*sargv)[i][j] = NULL;
            j=0;    i++;
        }
        else
        {                                           //printf("   Writing word\n");
            strcpy ((*sargv)[i][j], L -> word);
            j++;
        }        
        listTOsargv (L -> next, sargv, i, j);
    }
    else
    {                                               //printf("\ti:%d, j:%d\n", i, j);
        free((*sargv)[i][j]);    (*sargv)[i][j] = NULL;
        j=0;    i++;                                //printf("\ti:%d, j:%d Ending\n", i, j);
        free((*sargv)[i][j]);    (*sargv)[i][j] = NULL;
    }
}

void listTOtoken (struct list *L, char ***sargv, int j)
{
    if (L != NULL)
    {
        if (L -> istoken)
        {
            strcpy ((*sargv)[j++], L -> word);
        }
        listTOtoken (L -> next, sargv, j);
    }
    else (*sargv)[j] = NULL;
}

void freelist (struct list *L)
{
    if (L!=NULL)
    {
        freelist (L -> next);
        free (L -> word);
        free (L);
    }
}

int ifistoken (char c)
{
    if ((c == '>') || (c == '<') || (c == '|')) return(1);
    else return(0);
}

void assemblyline (char ***sargv, char **stokenv, int stokenc)
{
    #include <fcntl.h>
    int i=0, j=0, pid, fd[2], prev=0, fd1;
    while (i<=stokenc)
    {
        if ((i==stokenc) || (strcmp(stokenv[i], "|") == 0))
        {
            pipe(fd);
            switch (pid = fork())
            {
                case -1: 
                    perror("Basta!!");
                    return;
                case 0: 
                    if (i!=stokenc)
                        dup2(fd[1], 1);
                    close(fd[0]);   close(fd[1]);
                    for (j=prev; ((j<stokenc) && (strcmp(stokenv[j], "|") != 0)); j++)
                    {                                                       //fprintf(stderr, "|%d|", j);
                        if (strcmp(stokenv[j], "<") == 0)
                        {
                            fd1 = open(sargv[j+1][0], O_RDONLY);            //fprintf(stderr, "*%s*", sargv[j+1][0]);
                            if (fd1 < 0) fprintf(stderr, BACKGROUND_RED "%s: No such file or directory" COLORENDS "\n", sargv[j+1][0]);
                            else { dup2(fd1, 0);   close(fd1); }
                        }
                        else if (strcmp(stokenv[j], ">") == 0)
                        {
                            fd1 = open(sargv[j+1][0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
                            if (fd1 < 0) fprintf(stderr, BACKGROUND_RED "%s: No such file or directory" COLORENDS "\n", sargv[j+1][0]);
                            else {  dup2(fd1, 1);   close(fd1); }
                        }
                        else if (strcmp(stokenv[j], ">>") == 0)
                        {
                            fd1 = open(sargv[j+1][0], O_WRONLY | O_APPEND, 0777);
                            if (fd1 < 0) fprintf(stderr, BACKGROUND_RED "%s: No such file or directory" COLORENDS "\n", sargv[j+1][0]);
                            else {  dup2(fd1, 1);   close(fd1); }
                        }
                        else fprintf(stderr, BACKGROUND_RED "HAL9000: syntax error near unexpected token '%s'" COLORENDS "\n", sargv[j+1][0]);
                    }
                    execvp(sargv[prev][0], sargv[prev]);
                    fprintf(stderr, BACKGROUND_RED "%s: command not found" COLORENDS "\n", sargv[prev][0]);
                    exit(1);
            }
            dup2(fd[0], 0);
            close(fd[0]);   close(fd[1]);
            prev=i+1;
    
        }
        i++;
    }
    while(wait(NULL) != -1) ;
    return;
}

int main(int argc, char *argv[])
{
    struct list *curlist;          // List to determine maxsize of a word in a single cmd
    char *curpath, *curpathtmp, *curstr, *curpathelem;
    char shellname[]="HAL9000:";
    char c, workroot='~';

    int sargc, stokenc, curstrsize, strmax, curpathsize;
    int quotesflag, begincmd, endflag, nextflag, readprev, tokenflag;                  // Flags for quotes in a command line; for start of cmd;
    int tmpflag=1;                        // Minor flags
    int i, j, k, fd1;                           // Counters
    char **stokenv; 
    char ***sargv;                      // Argv of a cmd

    pid_t pid;

    curpathsize = 2;
    curpath = (char *)malloc(2);
    *curpath = '\0';

    while(1)
    {
        curstr = (char *)malloc(2);

        printf(RED "%s" COLORENDS PURPLE, shellname);
        if (workroot != '1') putchar(workroot);
        printf("%s" COLORENDS, curpath);
        printf("$ ");

        readprev=1;
        tokenflag=0;
        curlist=NULL;

        endflag=0; 
        nextflag=0;
        quotesflag=0;
        begincmd=1;

        strmax=0;
        sargc=0;
        stokenc=0;

        c = getchar();
        while (c != '\n')     // Read the line
        {
            if (begincmd)          // If EOF is met in the beginning without a '\n'
                if (c == EOF)
                {
                    printf("exit\n");
                    endflag=1;
                    break;
                }
            curstrsize = 0;
            
            if (ifistoken(c))
            {
                if (tokenflag || begincmd)
                {
                    nextflag=1;
                    fprintf(stderr, BACKGROUND_RED "%s syntax error near unexpected token '%c'" COLORENDS "\n", shellname, c);
                }
                while (ifistoken(c))   // Token input
                {
                    curstr[curstrsize] = c;
                    curstrsize++;
                    curstr = realloc(curstr, curstrsize+1);
                    c = getchar();
                    readprev=0;
                }
                tokenflag=1;
                stokenc++;
            }
            else
            {
                while (((!ifistoken(c) && (c != ' ') && (c != '\t') && (c != EOF)) || (quotesflag)) && (c != '\n'))
                {
                    begincmd = 0;
                    if (c=='"')
                    {
                        quotesflag = (quotesflag + 1) % 2;
                    }
                    else
                    {                
                        curstr[curstrsize] = c;
                        curstrsize++;
                        curstr = realloc(curstr, curstrsize+1);
                    }
                    c = getchar();
                    readprev=0;
                    tokenflag=0;
                }
            }

            if (curstrsize>0)
            {
                curstr[curstrsize] = '\0';                          //printf("sargc = %d\n", sargc);
                curlist = curlistadd(curlist,curstr,curstrsize,sargc,tokenflag);   // Create Dynamic memmory for curlist
                if (strmax < curstrsize) strmax = curstrsize;
                curstrsize = 0;
                sargc++;
                free(curstr);
                curstr = (char *)malloc(2);
            }

            if (c == '\n') break;
            if (readprev) c = getchar();
            else readprev=1;
        }
        if (curstr != NULL) free(curstr);
        if (endflag) break;
        if (nextflag) 
        {
            freelist(curlist);  
            continue;
        }

                                                                    //printf("%d, %d, %d\n", stokenc, sargc, strmax);
        stokenv = (char **)malloc((stokenc+1)*(sizeof(char *))); 
        for (i = 0; i<stokenc; i++)
            stokenv[i] = (char *)malloc(strmax+2);
        listTOtoken (curlist, &(stokenv), 0);
                                                                    //for(i = 0; i<=stokenc; i++)    printf("%s\t", stokenv[i]);
                                                    //printf("\n");
                                                                    //printf("%d, %d, %d", stokenc, sargc, strmax);
        sargv = (char ***)malloc((stokenc+2)*(sizeof(char **)));      //printf("clear\t");  // Create Dynamic memory for argv
        for (i = 0; i<stokenc+2; i++)
        {                                                           //printf("%d, %d, %d", stokenc, sargc, strmax);
            sargv[i] = (char **)malloc((sargc+2)*(sizeof(char *)));     //printf("i %d\t", i);    // ЗАМЕНИТЬ НА МАКСИМАЛЬНОЕ КОЛИЧЕСТВО СЛОВ В СТРОКЕ
            for (j = 0; j<sargc+2; j++)
            {
                sargv[i][j] = (char *)malloc(strmax+2);     //printf("i %d, j %d\t", i, j);
            }
        }                                                           //printf("Memmory made\n");
        listTOsargv (curlist, &(sargv), 0, 0);

                           //                         for(i = 0; i<=stokenc; i++)
                             //                       { 
                               //                         printf("\nСтрока %d\t", i);
                                 //                       for(j=0; j<sargc+2; j++)
                                   //                        printf("|%s|\t", sargv[i][j]);
                                     //               }printf("\n");

        int multiple = 0, i = 0;
        while (i < stokenc) 
        {
            if (strcmp(stokenv[i], "|") == 0) 
            {   
                multiple=1;     break;
            }
            i++;
        }

        if ((sargv[stokenc][0] == NULL) && (stokenc>0)) 
            fprintf(stderr, BACKGROUND_RED "%s syntax error near one of unexpected token" COLORENDS "\n", shellname);
        else
        if (multiple) 
        {
            if (fork() == 0) 
            {
                assemblyline(sargv, stokenv, stokenc);
                return(0);
            }
            else while(wait(NULL) != -1) ;
        }
        else 
        {
            if (sargc == 0) ;
            else if (strcmp(sargv[0][0], "exit") == 0)
            {
                endflag=1;
            }
            else if (strcmp(sargv[0][0], "cd") == 0)
            {   
                if ((sargc==1) || (strcmp(sargv[0][1],"~") == 0))
                {
                    chdir(getenv("HOME"));
                    workroot = '~';
                    *curpath = '\0';
                    curpathsize = 2;
                }
                else if (chdir(sargv[0][1]) == 0)
                { 
                    curpathsize += strmax+2;                             //printf("curstrmax: %d\n", curstrmax);
                    curpath = (char *)realloc(curpath, curpathsize);        //printf("curpathsize: %d\n", curpathsize);
                    if (sargv[0][1][0] == '/') 
                    {
                        free(curpath);
                        curpath = (char *)malloc(curpathsize); 
                        curpath[0] = '\0';
                        tmpflag = 0;
                        if (sargv[0][1][1] == '\0') workroot = '/';
                        else workroot = '1';
                    }
                    for(i=0; ((i<strmax) && (sargv[0][1][i] != '\0')); i++) 
                    {
                        j = 0;
                        curpathelem = (char *)malloc(strmax+2);
                        while((sargv[0][1][i] != '/') && (sargv[0][1][i] != '\0'))
                        {
                            curpathelem[j] = sargv[0][1][i];
                            i++; j++;                                       //printf("curpathelem: %s\n", curpathelem);
                        }
                        curpathelem[j] = '\0';                              //printf("curpath: %s\n", curpath);
                        if (strcmp(curpathelem, "..") == 0)
                        {                                                   
                            for (k=0; (curpath[k] != '\0'); k++)
                                if (curpath[k] == '/') j=k;                 //printf("k %d   j %d\n", k, j); // Может привести к ошибкt стрката 
                            for (; j<=k; j++)
                            {
                                curpath[j] = '\0';
                            }
                        }
                        else 
                        {
                            if (tmpflag) strcat(curpath, "/");
                            else tmpflag = 1;
                            strcat(curpath, curpathelem);
                        }
                        free(curpathelem);
                    }                
                }
                else 
                {
                    fprintf(stderr, BACKGROUND_RED "cd: %s: Not such file or directory" COLORENDS "\n", sargv[0][1]);
                }
            }
            else
            {   
                if ((pid = fork()) == 0)
                {        
                    for (j=0; j < stokenc; j++)
                    {           //fprintf(stderr, "|%d|", j);
                        if (strcmp(stokenv[j], "<") == 0)
                        {
                            fd1 = open(sargv[j+1][0], O_RDONLY);    //fprintf(stderr, "*%s*", sargv[j+1][0]);
                            if (fd1 < 0) fprintf(stderr, BACKGROUND_RED "%s: No such file or directory" COLORENDS "\n", sargv[j+1][0]);
                            else {  dup2(fd1, 0);   close(fd1); }
                        }
                        else if (strcmp(stokenv[j], ">") == 0)
                        {
                            fd1 = open(sargv[j+1][0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
                            if (fd1 < 0) fprintf(stderr, BACKGROUND_RED "%s: No such file or directory" COLORENDS "\n", sargv[j+1][0]);
                            else {  dup2(fd1, 1);   close(fd1); }
                        }
                        else if (strcmp(stokenv[j], ">>") == 0)
                        {
                            fd1 = open(sargv[j+1][0], O_WRONLY | O_APPEND, 0777);
                            if (fd1 < 0) fprintf(stderr, "%s: No such file or directory" COLORENDS "\n", sargv[j+1][0]);
                            else {  dup2(fd1, 1);   close(fd1); }
                        }
                        else fprintf(stderr, BACKGROUND_RED "%s syntax error near unexpected token '%s'" COLORENDS "\n", shellname, sargv[j+1][0]);
                    }
                    execvp(sargv[0][0], sargv[0]);
                    fprintf(stderr, BACKGROUND_RED "%s: command not found" COLORENDS "\n", sargv[0][0]);
                    exit(1);
                }
                else if (pid == -1)
                {
                    perror("Basta!!");
                    exit(2);
                }
                else
                {
                    int stat;
                    wait(&stat);
                    if (WIFEXITED(stat) == 0)
                    if (WEXITSTATUS(stat) != 1)
                    fprintf(stderr, BACKGROUND_RED "%s: exited with error" COLORENDS "\n", sargv[0][0]);
                }
            }
        }   




        for (i = 0; i<stokenc; i++)
            free(stokenv[i]);
        free(stokenv);
                                                    //printf("%d, %d, %d", stokenc, sargc, strmax);
        for (i = 0; i<stokenc+2; i++)               // Clear Dynamic memory for argv
        {
            for (j = 0; j<sargc+2; j++)
            {
                free(sargv[i][j]);      //printf("i %d, j %d\t", i, j);
            }
            free(sargv[i]); //printf("i %d\t", i);
        }
        free(sargv);    //printf("clear\t");
        
        freelist(curlist);                      // Clear Dynamic memory for curlist

        if (endflag) break;
    }

    free(curpath);
    return(0);
}