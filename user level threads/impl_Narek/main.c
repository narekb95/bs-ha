#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <memory.h>

#include "ult.h"

static bool readShellCommand();

static void printStats();

static void shellThread() {
    puts("User Level Threads Demo");
    puts("-----------------------");
    printf("Available commands: \"stats\" or \"exit\"\n");

    while(readShellCommand());
    ult_exit(0);
}

static bool readShellCommand() {
    char *command[100];
    printf("> ");
    fgets((char *) command, sizeof(command), stdin);

    if (strcmp((const char *) command, "stats\n") == 0) {
        printStats();
        return true;
    } else if (strcmp((const char *) command, "exit\n") == 0) {
        return false;
    } else {
        puts("Invalid command!");
        return true;
    }
}

static void printStats() {
    printf("Stats: \n");
}

static void copyThread() {
    ult_exit(0);
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

