#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include<string.h>

#include "ult.h"

void die(char *txt);

int done;
int dataLength;

static void threadA() {
    char *str = malloc(100 * sizeof(char));
    while (!done) {
        ssize_t l = ult_read(0, str, 100);
        if (l == -1) {
            die("error reading user input\n");
        }
        str[l - 1] = '\0';

        if (strcmp(str, "exit") == 0) {
            done = 1;
            ult_exit(1);
        } else if (strcmp(str, "stat") == 0) {
            printf("Total data read: %d\n", dataLength);
        } else {
            fprintf(stderr, "command not defined\n");
        }
    }
}

static void threadB() {
    FILE *file = fopen("/dev/random", "r");
    if (file == NULL) {
        die("can't open file to read");
    }
    int fd = fileno(file);
    FILE *out = fopen("/dev/null", "w");
    if (out == NULL) {
        die("can't open file to write");
    }

    char *str = malloc(101 * sizeof(char));
    if (str == NULL) {
        die("can't malloc string");
    }
    while (!done) {
        ssize_t l = ult_read(fd, str, 100);
        if (l == -1) {
            die("error reading data\n");
        }
        str[l] = '\0';
        fprintf(out, "%s", str);
        dataLength += l;
    }
    ult_exit(2);
}

static void myInit() {
    int tids[2], i, status;

    printf("spawn A\n");
    tids[0] = ult_spawn(threadA);
    printf("spawn B\n");
    tids[1] = ult_spawn(threadB);

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


