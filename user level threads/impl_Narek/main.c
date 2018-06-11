#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <memory.h>
#include <unistd.h>

#include "ult.h"
#include "stats.h"

static bool readShellCommand();

static void shellThread() {
    puts("User Level Threads Demo");
    puts("-----------------------");
    printf("Available commands: \"stats\" and \"exit\"\n");

    while (readShellCommand());
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
        printf("Unknown command: %s", (char *) command);
        return true;
    }
}

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
    ssize_t readRes = 0;
    do {
        ssize_t readRes = ult_read(devRandom, &buffer, sizeof(buffer));
        if (readRes) {
            printf("reading success! char:%s", buffer);
            incrementCopiedChars();
        }
    } while (readRes > 0);

    close(devRandom);
    close(devNull);
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
