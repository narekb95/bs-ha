#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define sep " \t"


char** splitArguments(char *command, int &argc);
void freeArgs(char **args, const int& argc);
int startProcess(const int& isSync);
void runProcess(const char **command);
void callWait(const char **command);
void callCD(const char **command);

int main(void)
{
    char currentDir[2000];
    char command[2000];
	if(getcwd(currentDir, sizeof(currentDir	)) == NULL)
	{
		fprintf(stderr, "Error getting current dir fprintf\n");
		exit(errno);
	}
	while(1)
	{
		printf("%s$ ", currentDir);
        
		geline(command, 2000, stdin);
        
        int argc;
        char** args = splitArguments(command, argc);
        
        if(strcmp(args[0], "cd") == 0)
        {
            
        } else if(strcmp(args[0], "wait") == 0)
        {
            
        } else if(strcmp(args[0], "exit") == 0)
        {
            freeArgs(args, argc);
            //todo: terminate all kid processes?
            exit(0);
        }
        else
        {
            if(startProcess(args[argc-1] == '&') == 0)
            {
                runProcess(args);
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
        return;
    }
    
    if(pid > 0)
    {
        if(isSync && pid > 0)
        {
            int status;
            wait(status);
        }
        else
        {
            printf("[%d]\n", pid);
        }
    }
    return pid;//calling function is responsible to handle as a parent or child
}

char** splitArguments(char *command, int &argc)
{
    argc = 0;
    char *arg;
    char **args = malloc(sizeof(char*) * 20);
    if(args == NULL)
    {
        fprintf(stderr, "can't split command\n");
        return NULL;
    }
    arg = strtok(command, argc);
    while(arg != NULL)
    {
        args[argc] = malloc(sizeof(char) * (strlen(arg) + 1));
        if(args[argc] == NULL)
        {
            freeArgs(args, argc);
            fprintf(stderr, "can't split command\n");
            return NULL;
        }
        strcopy(args[argc], arg);
        argc++;
        arg = strtok(NULL, arg);
    }
    
    //checking if lsat symbol '&' ist concatinated to last arg
    //todo test if this step is working fine and the realloc is returning the right sizes
    char *last = args[strlen-1];
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
    
    return args;
}

void freeArgs(char **args, const int& argc)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        free(args[i]);
    }
    free(args);
}

void callCD(const char **command)
{
    chdir(command[1]);
}

