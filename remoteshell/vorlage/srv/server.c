//has always done as reply (a hand shake quasi)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int PORT = 9000;
const int STPORT = 5023;
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
char *itoa(int x);
void startShell(int port);
void sigintHandler(int sig_num);
static void die(const char* msg);
int endsWithDone(const char* str);

//always close file descriptors and free char arrays [todo]
int main(void)
{

    struct sockaddr_in srv_addr, cli_addr;
    socklen_t sad_sz = sizeof(struct sockaddr_in);
    int sfd, cfd;
    ssize_t bytes;
    char buf[256];
    
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(PORT);
    srv_addr.sin_addr.s_addr = INADDR_ANY;
#ifdef DEBUG
    printf("0\n");
#endif
    
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        die("Couldn't open the socket");
    }
#ifdef DEBUG
    printf("1\n");
#endif
    
    if (bind(sfd, (struct sockaddr*) &srv_addr, sad_sz) < 0)
    {
        die("Couldn't bind socket");
    }
#ifdef DEBUG
    printf("2\n");
#endif
    
    if (listen(sfd, 1) < 0)
    {
        die("Couldn't listen to the socket");
    }
#ifdef DEBUG
    printf("3\n");
#endif
    
    int port = STPORT;
    int pid;
    while(1)
    {
        
        cfd = accept(sfd, (struct sockaddr*) &cli_addr, &sad_sz);
        if (cfd < 0)
        {
            die("Couldn't accept incoming connection");
        }
        char *nxtport = itoa(port);
        if (write(cfd, nxtport, 4) < 0)
        {
            die("Couldn't send message");
        }
        if((pid = startProcess())== 0)
        {
            startShell(port);
            return 0;
        }
        port++;
    }
    
    close(cfd);
    close(sfd);
    return 0;
}

static void die(const char* msg)
{
    fputs(msg, stderr);
    putc('\n', stderr);
    exit(-1);
}

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
char *itoa(int x)
{
    char *a = malloc(sizeof(char) * 20);
    int i = 0;
    while(x)
    {
        a[i++] = (x%10) + '0';
        x/=10;
    }
    int j;
    for(j = 0; j * 2 < i; j++)
    {
        char x = a[j];
        a[j] = a[i-j-1];
        a[i-j-1] = x;
    }
    return a;
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

void startShell(int port)
{
    signal(SIGINT, sigintHandler);
    signal(SIGCHLD, terminatedChildhandler);
    char currentDir[maxCharBufferSize];
    char command[maxCharBufferSize];
    
    
    
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t sad_sz = sizeof(struct sockaddr_in);
    int sfd, cfd;
    ssize_t bytes;
    char buf[256];
    
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        die("Couldn't open the socket");
    }
    
    if (bind(sfd, (struct sockaddr*) &srv_addr, sad_sz) < 0)
    {
        die("Couldn't bind socket");
    }
    
    if (listen(sfd, 1) < 0)
    {
        die("Couldn't listen to the socket");
    }
    
    cfd = accept(sfd, (struct sockaddr*) &cli_addr, &sad_sz);
    if (cfd < 0)
    {
        die("Couldn't accept incoming connection");
    }
    dup2(cfd, STDOUT_FILENO);
    while(1)
    {
        if(getcwd(currentDir, sizeof(currentDir    )) == NULL)
        {
            fprintf(stderr, "Error getting current dir\n");
            exit (errno);
        }
        if (write(cfd, currentDir, strlen(currentDir)) < 0)
        {
            die("Couldn't send message");
        }
        
        int bytesRead;
        if ((bytesRead = read(cfd, command, maxCharBufferSize-1)) < 0)
        {
            die("Couldn't read message, please try again");
            continue;
        }
        if(write(cfd, "done!", sizeof("done!")) < 0)
        {
            die("can't reply done\n");
        }
        command[bytesRead] = '\0';
        
        command[strlen(command) - 1] = '\0';
#ifdef DEBUG
        fprintf(stderr, "read command: \"%s\"\n", command);
#endif
        
        int argc;
        int isSync;
        char** args = splitArguments(command, &argc, &isSync);
        if(args[0] == NULL)
        {
#ifdef DEBUG
            fprintf(stderr, "no command\n");
            fprintf(stderr, "writing done!!\n");
#endif
            if (write(cfd, "done!", strlen("done!")) < 0)
            {
                die("Couldn't send message");
            }
#ifdef DEBUG
            fprintf(stderr, "waiting for done reply\n");
#endif
            if (read(cfd, command, maxCharBufferSize) < 0)
            {
                die("continue\n");
            }
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
#ifdef DEBUG
            fprintf(stderr, "pipe\n");
#endif
            char **args1, **args2;
            args1 = malloc(sizeof(char*) * (maxNumOfArgs/2));
            args2 = malloc(sizeof(char*) * (maxNumOfArgs/2));
            if(args1 == NULL || args2 == NULL)
            {
                free(args1);
                free(args2);
                fprintf(stdout, "can't run pipe commands\n");
                exit (1);
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
            if(pipe(fd) != 0)
            {
                fprintf(stderr, "can't create pipe\n");
                exit(1);
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
#ifdef DEBUG
            fprintf(stderr, "cd branch\n");
#endif
            callCD(args, argc);
        }
        else if(strcmp(args[0], "wait") == 0)
        {
#ifdef DEBUG
            fprintf(stderr, "wait branch\n");
#endif
            callWait(args, argc);
        }
        else if(strcmp(args[0], "exit") == 0)
        {
#ifdef DEBUG
            fprintf(stderr, "exit branch\n");
#endif
            freeArgs(args, argc);
            close(cfd);
            exit(0);
        }
        else if(strcmp(args[0], "put") == 0)
        {
#ifdef DEBUG
            fprintf(stderr, "put branch in file \"%s\"\n", args[1]);
#endif
            FILE *file = fopen(args[1], "w");
            if(file == NULL)
            {
                die("Couldn't open file to write\n");
                continue;
            }
            int bytesRead;
            char *txt = malloc(maxCharBufferSize);
            if(txt == NULL)
            {
                die("can't allocat char array\n");
                return;
            }
            while(1)
            {
                if ((bytesRead = read(cfd, txt, maxCharBufferSize-1)) < 0)
                {
                    die("Couldn't read message, please try again");
                }
                txt[bytesRead] = '\0';
                if(endsWithDone(txt))
                {
#ifdef DEBUG
                    fprintf(stderr, "done writing\n");
                    txt[strlen(txt)-5]='\0';
                    fprintf(file, "%s", txt);
#endif
                    if (write(cfd, "done!", strlen("done!")) < 0)
                    {
                        die("couldn't reply done");
                    }
                    if ((bytesRead = read(cfd, txt, maxCharBufferSize-1)) < 0)
                    {
                        die("Couldn't read message, please try again");
                    }
                    break;
                }
#ifdef DEBUG
                fprintf(stderr, "writing..\n");
                fprintf(stderr, "%s", txt);
#endif
                fprintf(file, "%s", txt);
            }
            fclose(file);
        }
        else if(strcmp(args[0], "get") == 0)
        {
#ifdef DEBUG
            fprintf(stderr, "get branch\n");
            fprintf(stderr, "%s\n", args[1]);
#endif
            FILE *file = fopen(args[1], "r");
            if(file == NULL)
            {
                die("Couldn't open file to write\n");
                continue;
            }
            int bytesRead;
            char *buf = malloc(maxCharBufferSize);
            if(buf == NULL)
            {
                die("can't allocat char array\n");
                return;
            }
            
            while(fgets(buf, maxCharBufferSize, file) != NULL)
            {
#ifdef DEBUG
                fprintf(stderr, "writing data to file\n");
                fprintf(stderr, "%s\n", buf);
#endif
                if (write(cfd, buf, strlen(buf)) < 0)
                {
                    die("Couldn't send message");
                }
            }
            if (write(cfd, "done!", strlen("done!")) < 0)
            {
                die("Couldn't send message");
            }
            if(read(cfd, buf, sizeof(buf) - 1) < 0)
            {
                die("couldn't get donereply\n");
            }
            fclose(file);
        }
        else
        {
            int pid;
            if((pid = startProcess()) == 0)
            {
#ifdef DEBUG
                fprintf(stderr,"calling command \"%s\"\n", args[0]);
#endif
                execvp(args[0], args);
                fprintf(stderr, "Error calling command:\"%s\"\n", args[0]);
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
        
        //end of while should always be done
        done = 0;
        if (write(cfd, "done!", strlen("done!")) < 0)
        {
#ifdef DEBUG
            printf("writing done!!\n");
#endif
            die("Couldn't send message");
        }
        
#ifdef DEBUG
        fprintf(stderr, "waiting for done reply\n");
#endif
        if (read(cfd, command, maxCharBufferSize) < 0)
        {
            die("can't read done reply\n");
        }
    }
}

int endsWithDone(const char* str)
{
    if(strlen(str) < 5)
    {
        return 0;
    }
    int l = strlen(str);
    return str[l-1] == '!' && str[l-2] == 'e' && str[l-3] == 'n' && str[l-4] == 'o' && str[l-5] == 'd';
}
