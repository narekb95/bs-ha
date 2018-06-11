#include <stdio.h>
#include "stats.h"

const char *FILE_NAME = "stats.txt";

const int SUCCESS_RETURN_CODE = 0;
const int ERROR_OPENING_FILE = 1;

int prepareStatsFile() {
    FILE *f = fopen(FILE_NAME, "wb");
    if (f == NULL) {
        return ERROR_OPENING_FILE;
    }
    fprintf(f, "Nothing to see here yet!\n");
    fclose(f);
    return SUCCESS_RETURN_CODE;
}

volatile void printStats() {
    printf("Stats:\n");

    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        perror("Unable to open stats.txt in printStats");
        return;
    }

    char statsContents[100];
    fgets(statsContents, sizeof(statsContents), f);
    printf("%s", statsContents);
    fclose(f);
}
