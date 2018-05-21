//todo: accept more than one pipe (max args array of array of array of chars [command][args][string] and pipe args[i] to args[i+1]
//todo: make num of args dynamic
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define sep " \t"
int done = 0;
const int maxNumOfArgs = 20;
const int maxCharBufferSize = 2000;

int runningpids[210005];

char** splitArguments(char *command, int *p_argc, int *p_isSync);
void freeArgs(char **args, const int argc);
int startProcess();
void callWait(char **args, const int argc);
void callCD(char **args, const int argc);


void sigintHandler(int sig_num)
{
    done = 1;
}
void terminatedChildhandler(int sig)
{
    int   status;
    int pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        runningpids[pid] = 0;
    }
}
int main(void)
{
    signal(SIGINT, sigintHandler);
    signal(SIGCHLD, terminatedChildhandler);
    char currentDir[maxCharBufferSize];
    char command[maxCharBufferSize];
	while(1)
	{
        if(getcwd(currentDir, sizeof(currentDir    )) == NULL)
        {
            fprintf(stderr, "Error getting current dir\n");
            return(errno);
        }
		printf("%s$ ", currentDir);
        
	if(fgets(command, maxCharBufferSize, stdin) == NULL)
	{
		fprintf(stderr, "can't read command, please try again\n");
	}
        command[strlen(command) - 1] = '\0';
        
        int argc;
        int isSync;
        char** args = splitArguments(command, &argc, &isSync);
        if(args[0] == NULL)
        {
            continue;
        }
        
        int i;
        for(i = 0; i < argc; i++)
        {
            if(strcmp(args[i], "|") == 0)
            {
                break;
            }
        }
        if(i != argc)
        {
            char **args1, **args2;
            args1 = malloc(sizeof(char*) * (maxNumOfArgs/2));
            args2 = malloc(sizeof(char*) * (maxNumOfArgs/2));
            if(args1 == NULL || args2 == NULL)
            {
                free(args1);
                free(args2);
                fprintf(stdout, "can't run pipe commands\n");
                return 1;
            }
            
            int j;
            for(j = 0; j < i; j++)
            {
                args1[j] = args[j];
            }
            args1[j] = NULL;
            for(j = i + 1; j < argc; j++)
            {
                args2[j - i - 1] = args[j];
            }
            args2[j-i-1] = NULL;
            
            int id1, id2;
            int fd[2];
            if(pipe(fd) == 0)
	    {
		fprintf(stderr, "can't create pipe\n");
		return 1;
	    }
            if((id1 = startProcess()) == 0)
            {
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                execvp(args1[0], args1);
                fprintf(stderr, "Error calling command: %s\n", args1[0]);
                exit(1);
            }
            if((id2 = startProcess()) == 0)
            {
                close(fd[1]);
                dup2(fd[0], 0);
                close(fd[0]);
                execvp(args2[0], args2);
                fprintf(stderr, "Error calling command: %s\n", args2[0]);
                exit(1);
            }
            close(fd[0]);
            close(fd[1]);
            if(isSync)
            {
                int stat;
                waitpid(id1, &stat, 0);
                waitpid(id2, &stat, 0);
            }
            else
            {
                printf("[%d]\n", id1);
                printf("[%d]\n", id2);
            }
        }
        else if(strcmp(args[0], "cd") == 0)
        {
            callCD(args, argc);
        }
        else if(strcmp(args[0], "wait") == 0)
        {
            callWait(args, argc);
        }
        else if(strcmp(args[0], "exit") == 0)
        {
            freeArgs(args, argc);
            exit(0);
        }
        else
        {
            int pid;
            if((pid = startProcess()) == 0)
            {
                execvp(args[0], args);
                fprintf(stderr, "Error calling command: %s\n", args[0]);
                exit(1);
            }
            else
            {
                if(isSync)
                {
                    int status;
                    waitpid(pid, &status, 0);
                }
                else
                {
                    printf("[%d]\n", pid);
                }
            }
        }
        done = 0;
	}
    
	return 0;
}


int startProcess()
{
    int pid = fork();
    if(pid < 0)
    {
        fprintf(stderr, "can't start new process\n");
    }
    else if(pid > 0)
    {
        
        runningpids[pid] = 1;
    }
    return pid;
}

char** splitArguments(char *command, int *p_argc, int *p_isSync)
{
    int argc = 0;
    int isSync = 1;
    char *arg;
    char **args = malloc(sizeof(char*) * maxNumOfArgs);
    if(args == NULL)
    {
        fprintf(stderr, "can't split command\n");
        return NULL;
    }
    arg = strtok(command, sep);
    while(arg != NULL)
    {
        args[argc] = malloc(sizeof(char) * (strlen(arg) + 1));
        if(args[argc] == NULL)
        {
            freeArgs(args, argc);
            fprintf(stderr, "can't split command\n");
            return NULL;
        }
        strcpy(args[argc], arg);
        argc++;
        arg = strtok(NULL, sep);
    }
    
    if(argc > 0)
    {
        char *last = args[argc-1];
        int lenLast = strlen(last);
        if(strcmp(last, "&") == 0)
        {
            argc--;
            free(args[argc]);
            isSync = 0;
        }
        else
        {
            if(lenLast > 1 && last[lenLast-1] == '&')
            {
                isSync = 0;
                last[lenLast - 1] = '\0';
            }
            
        }
    }
    *p_argc = argc;
    *p_isSync = isSync;
    args[argc] = NULL;
    return args;
}

void freeArgs(char **args, const int argc)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        free(args[i]);
    }
    free(args);
}

void callCD(char **args, const int argc)
{
    if(chdir(args[1]) != 0)
    {
	fprintf(stderr, "can't open directory\n");
    }
}

void callWait(char **args, const int argc)
{
    int i;
    int stillToBeStopped = 0;
    
    for(i = 1; i < argc; i++)
    {
        if(runningpids[atoi(args[i])])
        {
            stillToBeStopped++;
        }
        else
        {
            args[i][0] = '\0';
        }
    }
    
    while(stillToBeStopped)
    {
        usleep(10000);
        for(i = 1; i < argc; i++)
        {
            if(args[i][0] == '\0')
            {
                continue;
            }
            int pid = atoi(args[i]);
            if(runningpids[pid] == 0)
            {
                args[i][0] = '\0';
                stillToBeStopped--;
            }
        }
        if(done)
        {
            break;
        }
    }
}
