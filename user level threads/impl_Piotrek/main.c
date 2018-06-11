#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <memory.h>
#include <unistd.h>

#include "ult.h"
#include "stats.h"

static bool readShellCommand();

static void shellThread() {
    puts("User Level Threads Demo");
    puts("-----------------------");
    printf("Available commands: \"stats\" or \"exit\"\n");

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
        puts("Invalid command!");
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
    while (true) {
        ssize_t readResult = read(devRandom, buffer, sizeof(buffer));
        ssize_t writeResult = write(devNull, buffer, sizeof(buffer));

        if (readResult < 0) perror("IO read error!");
        if (writeResult < 0) perror("IO write error!");
        printf("buffer: %s\n", buffer);

    }

//unreachable code
//    close(devRandom);
//    close(devNull);

}

static void myInit() {

    prepareStatsFile();

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

