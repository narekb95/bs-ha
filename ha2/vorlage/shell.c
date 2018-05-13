#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define sep " \t"
const int maxNumOfArgs = 20;
const int maxCharBufferSize = 2000; //toDo: make a struct for dynamic char arrays that grows dynamically (like a vector)

char **splitArguments(char *command, int *argc);
void freeArgs(char **args, const int argc);
int startProcess(const int isSync);
void runProcess(char *path, char **args, const int argc);
//void callWait(char **args, const int argc);
void callCD(char **args, const int argc);

int main(void)
{
    char currentDir[maxCharBufferSize];
    char command[maxCharBufferSize];
	while(1)
	{
        if(getcwd(currentDir, sizeof(currentDir    )) == NULL)
        {
            fprintf(stderr, "Error getting current dir fprintf\n");
            exit(errno);
        }
		printf("%s$ ", currentDir);
        
		fgets(command, maxCharBufferSize, stdin);
        command[strlen(command) - 1] = '\0';
        
        int argc;
        char** args = splitArguments(command, &argc);
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
            printf ("calling wait\n");
        }
        else if(strcmp(args[0], "exit") == 0)
        {
            freeArgs(args, argc);
            //todo: terminate all kid processes?
            exit(0);
        }
        else
        {
            if(startProcess(strcmp(args[argc-1], "&") != 0) == 0)
            {
                char *commandToCall = args[0];
                args[0] = currentDir;
                runProcess(commandToCall, args, argc);
                free(commandToCall);
                args[0] = NULL;
            }
        }
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
        if(isSync && pid > 0)
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

char** splitArguments(char *command, int *p_argc)
{
    int argc = 0;
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
    
    //checking if lsat symbol '&' ist concatinated to last arg
    //todo test if this step is working fine and the realloc is returning the right sizes
    if(argc > 0)
    {
        char *last = args[argc-1];
        int lenLast = strlen(last);
        if(lenLast > 1 && last[lenLast-1] == '&')
        {
            last[lenLast - 1] = '\0';
            last = realloc(last, lenLast);//realloc here is shrinking no need to null check
            //sizeof is normally len + 1 (for the '\0' symbol)
            args[argc] = malloc(2*sizeof(char));
            if(args == NULL)
            {
                freeArgs(args, argc);
                fprintf(stderr, "can't split command\n");
                return NULL;
            }
            args[argc][0] = '&';
            argc++;
        }
    }
    *p_argc = argc;
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
    printf("cd to path \"%s\"\n", args[1]);
    chdir(args[1]);
}

void runProcess(char *command, char **args, const int argc)
{
    printf("%s\n", command);
    int i;
    for(i = 0; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
    execvp(command, args);
}
