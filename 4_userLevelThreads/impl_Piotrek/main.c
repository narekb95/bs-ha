#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>

#include "ult.h"

void waitForCommands();

void processCommand(char *command);

void printStats();

const int BUFFER_SIZE = 1024;
long long int charsCopied = 0;
float timePassed = 0.0f;
bool shellRunning = true;

void printWaitingChar() {
    printf("> ");
    fflush(stdout);
}

///////// THREAD A //////////
static void shellThread() {
    puts("User Level Threads Demo");
    puts("-----------------------");
    puts("Available commands: \"stats\" and \"exit\"");
    waitForCommands();
    ult_exit(111);
}

void waitForCommands() {
    char command[20];
    printWaitingChar();

    while (shellRunning) {
        if (ult_read(0, command, sizeof(command)) > 0) {
            processCommand(command);
            memset(command, '\0', sizeof(command));
            printWaitingChar();
        }
    }
}

void processCommand(char *command) {
    if (strcmp(command, "stats\n") == 0) {
        printStats();
    } else if (strcmp(command, "exit\n") == 0) {
        shellRunning = false;
    } else {
        printf("Unknown command: %s", command);
    }
}

void printStats() {
    printf("Stats:\n");
    printf("Bytes copied: %lli\n", charsCopied);
    printf("Run time:     %f s\n", timePassed);
    printf("Throughput:   ");
    float speed = ((float) charsCopied / timePassed);

    if (speed > 1000000)
        printf("%f MByte/s\n", speed / (1024 * 1024));
    else if (speed > 1000)
        printf("%f KByte/s\n", speed / 1024);
    else
        printf("%f Byte/s\n", speed);
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

    char buffer[BUFFER_SIZE];
    clock_t startTime = clock();

    while (shellRunning) {
        ult_read(devRandom, buffer, BUFFER_SIZE);
        if (write(devNull, buffer, BUFFER_SIZE) < 0) {
            perror("error while writing to devNull");
        }
        charsCopied += BUFFER_SIZE;
        timePassed = (float) (clock() - startTime) / CLOCKS_PER_SEC;

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
