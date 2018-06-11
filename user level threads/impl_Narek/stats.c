#include <stdio.h>
#include "stats.h"

int charsCopied = 0;

volatile void printStats() {
    printf("Stats:\n");
    printf("Chars copied: %d\n", charsCopied);
}

void incrementCopiedChars() {
    charsCopied++;
}

