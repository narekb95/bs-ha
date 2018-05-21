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

typedef char* stringTemplate;

int done = 0;
const char *SEPARATOR = " \t";
const int MAX_ARGS_NUMBER = 20;
const int MAX_CHAR_BUFFER_SIZE = 2000;
stringTemplate ERROR_UNKNOWN_COMMAND = "Unknown command: %s";

int runningpids[210005];

char **splitArguments(char *command, int *p_argc, int *p_isSync);

void freeArgs(char **args, int argc);

int startProcess();

void callWait(char **args, int argc);

void callCD(char **args, int argc);

void printCurrentWorkingDir();

void errorAndExit(char errorMessage[100], int errorCode);

void getCommand(char *command);

int getPipePosition(int argc, char *const *args);

void errorMessage(stringTemplate errorMessage, const char *value);

void sigintHandler(int sig_num) {
    done = 1;
}

void terminatedChildhandler(int sig) {
    int status;
    int pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        runningpids[pid] = 0;
    }
}

int main() {
    signal(SIGINT, sigintHandler);
    signal(SIGCHLD, terminatedChildhandler);
    char command[MAX_CHAR_BUFFER_SIZE];

    while (1) {
        printCurrentWorkingDir();
        getCommand(command);

        int argc;
        int isSync;
        char **args = splitArguments(command, &argc, &isSync);
        if (args[0] == NULL) {
            continue;
        }

        int pipePosition = getPipePosition(argc, args);
        if (pipePosition != argc) {
            char **args1 = malloc(sizeof(char *) * MAX_ARGS_NUMBER);
            if (args1 == NULL) errorAndExit("Malloc for args1 failed!", -1);

            char **args2 = malloc(sizeof(char *) * MAX_ARGS_NUMBER);
            if (args2 == NULL) errorAndExit("Malloc for args2 failed!", -1);

            int j;
            for (j = 0; j < pipePosition; j++) {
                args1[j] = args[j];
            }
            args1[j] = NULL;
            for (j = pipePosition + 1; j < argc; j++) {
                args2[j - pipePosition - 1] = args[j];
            }
            args2[j - pipePosition - 1] = NULL;

            int pid1, pid2;
            int fd[2];
            if (pipe(fd)) {
                errorAndExit("Unable to call pipe()\n", errno);
            }
            if ((pid1 = startProcess()) == 0) {
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                execvp(args1[0], args1);
                errorMessage(ERROR_UNKNOWN_COMMAND, args1[0]);
            }
            if ((pid2 = startProcess()) == 0) {
                close(fd[1]);
                dup2(fd[0], 0);
                close(fd[0]);
                execvp(args2[0], args2);
                errorMessage(ERROR_UNKNOWN_COMMAND, args2[0]);
            }
            close(fd[0]);
            close(fd[1]);
            if (isSync) {
                int stat;
                waitpid(pid1, &stat, 0);
                waitpid(pid2, &stat, 0);
            } else {
                printf("[%d]\n", pid1);
                printf("[%d]\n", pid2);
            }
        }
        if (strcmp(args[0], "cd") == 0) {
            callCD(args, argc);
        } else if (strcmp(args[0], "wait") == 0) {
            callWait(args, argc);
        } else if (strcmp(args[0], "exit") == 0) {
            freeArgs(args, argc);
            exit(0);
        } else {
            int pid;
            if ((pid = startProcess()) == 0) {
                execvp(args[0], args);
                errorMessage(ERROR_UNKNOWN_COMMAND, args[0]);
            } else {
                if (isSync) {
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    printf("[%d]\n", pid);
                }
            }
        }
        done = 0;
    }
}

void errorMessage(stringTemplate errorMessage, const char *value) {
    fprintf(stderr, errorMessage, value);
    exit(1);
}

int getPipePosition(int argc, char *const *args) {
    int position;
    for (position = 0; position < argc; position++) {
            if (strcmp(args[position], "|") == 0) {
                break;
            }
        }
    return position;
}

void getCommand(char *command) {
    if (!fgets(command, MAX_CHAR_BUFFER_SIZE, stdin)) {
            errorAndExit("Unable to get the command", errno);
        }
    command[strlen(command) - 1] = '\0';
}

void printCurrentWorkingDir() {
    char currentWorkingDir[MAX_CHAR_BUFFER_SIZE];
    if (!getcwd(currentWorkingDir, sizeof(currentWorkingDir))) {
        errorAndExit("Error getting current working dir!\n", errno);
    }
    printf("%s$ ", currentWorkingDir);
}


int startProcess() {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "can't start new process\n");
    } else if (pid > 0) {
        runningpids[pid] = 1;
    }
    return pid;
}

char **splitArguments(char *command, int *p_argc, int *p_isSync) {
    int argc = 0;
    int isSync = 1;
    char *arg;
    char **args = malloc(sizeof(char *) * MAX_ARGS_NUMBER);
    if (args == NULL) {
        errorAndExit("Can't split command\n", -1);
    }
    arg = strtok(command, SEPARATOR);
    while (arg != NULL) {
        args[argc] = malloc(sizeof(char) * (strlen(arg) + 1));
        if (args[argc] == NULL) {
            freeArgs(args, argc);
            fprintf(stderr, "Can't split command\n");
            return NULL;
        }
        strcpy(args[argc], arg);
        argc++;
        arg = strtok(NULL, SEPARATOR);
    }

    if (argc > 0) {
        char *last = args[argc - 1];
        size_t lenLast = strlen(last);
        if (strcmp(last, "&") == 0) {
            argc--;
            free(args[argc]);
            isSync = 0;
        } else {
            if (lenLast > 1 && last[lenLast - 1] == '&') {
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

void freeArgs(char **args, const int argc) {
    for (int i = 0; i < argc; i++) {
        free(args[i]);
    }
    free(args);
}

void callCD(char **args, const int argc) {
    int retCode = chdir(args[1]);
    if (retCode) errorAndExit("Unable to change directory!\n", 0);
}

void callWait(char **args, const int argc) {
    int stillToBeStopped = 0;

    for (int i = 1; i < argc; i++) {
        if (runningpids[atoi(args[i])]) {
            stillToBeStopped++;
        } else {
            args[i][0] = '\0';
        }
    }

    while (stillToBeStopped) {
        usleep(10000);
        for (int i = 1; i < argc; i++) {
            if (args[i][0] == '\0') {
                continue;
            }
            int pid = atoi(args[i]);
            if (runningpids[pid] == 0) {
                args[i][0] = '\0';
                stillToBeStopped--;
            }
        }
        if (done) {
            break;
        }
    }
}

void errorAndExit(char errorMessage[100], int errorCode) {
    fprintf(stderr, "%s", errorMessage);
    exit(errorCode);
}
