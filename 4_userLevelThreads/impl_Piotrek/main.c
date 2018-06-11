#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <memory.h>
#include <unistd.h>

#include "ult.h"

void waitForCommands();
void processShellCommand(char *command);

int charsCopied = 0;
bool shellRunning = true;

void printStats() {
    printf("Stats:\n");
    printf("Chars copied: %d\n", charsCopied);
}

void printWaitingChar() {
    printf("> ");
    fflush(stdout);
}

///////// THREAD A //////////
static void shellThread() {
    puts("User Level Threads Demo");
    puts("-----------------------");
    puts("Available commands: \"stats\" and \"exit\"");
    printWaitingChar();
    waitForCommands();
    ult_exit(111);
}

void waitForCommands() {
    char command[20];
    while (shellRunning) {
        ult_read(0, command, sizeof(command));
        processShellCommand(command);
        memset(command, '\0', sizeof(command));
    }
}

void processShellCommand(char *command) {
    if (strcmp(command, "stats\n") == 0) {
        printStats();
        printWaitingChar();
    } else if (strcmp(command, "exit\n") == 0) {
        shellRunning = false;
    } else {
        printf("Unknown command: %s", command);
        printWaitingChar();
    }
}

///////// THREAD B //////////
static void copyThread() {
    int devRandom = open("/dev/random", O_RDONLY);
    if (devRandom < 0) {
        perror("unable to open /dev/random");
        return;
    }

    int devNull = open("/dev/null", O_WRONLY);
    if (devNull < 0) {
        perror("unable to open /dev/null");
        return;
    }

    char buffer[1];
    while (shellRunning) {
        ult_read(devRandom, buffer, sizeof(buffer));
        if (write(devNull, buffer, sizeof(buffer)) < 0) {
            perror("error while writing to devNull");
        }
        charsCopied++;
    }
    ult_exit(222);
}

static void myInit() {
    int tids[2], i, status;

    printf("spawn A\n");
    tids[0] = ult_spawn(shellThread);
    printf("spawn B\n");
    tids[1] = ult_spawn(copyThread);

    for (i = 0; i < 2; ++i) {
        printf("waiting for tids[%d] = %d\n", i, tids[i]);
        fflush(stdout);

        if (ult_join(tids[i], &status) < 0) {
            fprintf(stderr, "join for %d failed\n", tids[i]);
            ult_exit(-1);
        }

        printf("(status = %d)\n", status);
    }

    ult_exit(0);
}

int main() {
    ult_init(myInit);
    return 0;
}
