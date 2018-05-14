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
const int maxCharBufferSize = 2000; //toDo: make a struct for dynamic char arrays that grows dynamically (like a vector)

int runningpids[210005];

char** splitArguments(char *command, int *p_argc, int *p_isSync);
void freeArgs(char **args, const int argc);
int startProcess(const int isSync);
void callWait(char **args, const int argc);
void callCD(char **args, const int argc);


void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    done = 1;
}
void terminatedChildhandler(int sig)
{
    //signal(SIGCHLD, terminatedChildhandler);
    printf("handling sigchld %d\n", sig);
    int   status;
    int pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("reseting child %d\n", pid);
        runningpids[pid] = 0;   // Or whatever you need to do with the PID
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
            fprintf(stderr, "Error getting current dir fprintf\n");
            return(errno);
        }
		printf("%s$ ", currentDir);
        
		fgets(command, maxCharBufferSize, stdin);
        command[strlen(command) - 1] = '\0';
        
        int argc;
        int isSync;
        char** args = splitArguments(command, &argc, &isSync);
        if(args[0] == NULL)
        {
            continue;
        }
        
        if(strcmp(args[0], "cd") == 0)
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
            //todo: terminate all kid processes?
            exit(0);
        }
        else
        {
            if(startProcess(isSync) == 0)
            {
                printf("is sync: %d\n", isSync);
                execvp(args[0], args);
            }
        }
        done = 0;
	}
    
	return 0;
}


int startProcess(int isSync)
{
    int pid = fork();
    if(pid < 0)
    {
        fprintf(stderr, "can't start new process\n");
    }
    else if(pid > 0)
    {
        
        runningpids[pid] = 1;
        if(isSync)
        {
            int status;
            wait(&status);
        }
        else
        {
            printf("[%d]\n", pid);
        }
    }
    return pid;//calling function is responsible to handle as a parent or child
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
    
    //check if last argument is "&" or the symbol is concatinated to last argument
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
    chdir(args[1]);
}

void callWait(char **args, const int argc)
{
    int i;
    int stillToBeStopped = 0;
    
    for(i = 1; i < argc; i++)
    {
        printf("process %s ", args[i]);
        if(runningpids[atoi(args[i])])
        {
            printf("is rnning\n");
            stillToBeStopped++;
        }
        else
        {
            printf("is already stopped\n");
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
                printf("Porcess %d done, still habe %d to go\n", pid, stillToBeStopped);
            }
        }
        if(done)
        {
            printf("not waiting anymore\n");
            break;
        }
    }
    printf("exiting wait with c = %d\n" ,stillToBeStopped);
}
