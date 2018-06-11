//
// Created by se2 on 11.06.18.
//

#include <stdio.h>
#include "stats.h"

const char *FILE_NAME = "stats.txt";

void prepareStatsFile() {
    FILE *f = fopen(FILE_NAME, "wb");
    if (f == NULL) {
        perror("Unable to open stats.txt");
    }
    fprintf(f, "Nothing to see here yet!\n");
    fclose(f);
}

volatile void printStats() {
    printf("Stats:\n");

    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        perror("Unable to open stats.txt");
        return;
    }
    char statsContents[100];
    fgets(statsContents, sizeof(statsContents), f);
    printf("%s", statsContents);
    fclose(f);
}
