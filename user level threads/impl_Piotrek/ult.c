#include "ult.h"
#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ucontext.h>

static int newThreadId = 0;
static int currentThreadId = 0;
static int NUMBER_OF_ALL_THREADS = 3;

enum threadStatus {
    RUNNABLE, FINISHED
};

/* thread control block */
typedef struct tcb_s {
    ucontext_t ctx;
    int status;
    int exitCode;
} tcb_t;

static tcb_t *threads;


void ult_init(ult_f f) {
    printf("ult_init\n");

    //the threads array should be dynamic, in order to add many ULTs
    threads = malloc(NUMBER_OF_ALL_THREADS * sizeof(tcb_t));

    ult_spawn(f);
    setcontext(&threads[0].ctx);
}

int ult_spawn(ult_f f) {
    // First, create a valid execution context the same as the current one
    getcontext(&threads[newThreadId].ctx);

    // Now set up its stack
    threads[newThreadId].ctx.uc_stack.ss_sp = malloc(8192);
    threads[newThreadId].ctx.uc_stack.ss_size = 8192;

    // This is the context that will run when this thread exits
    threads[newThreadId].ctx.uc_link = newThreadId > 0 ? &threads[0].ctx : 0;

    // Now create the new context and specify what function it should run
    makecontext(&threads[newThreadId].ctx, f, 0);

    threads[newThreadId].status = RUNNABLE;
    return newThreadId++;
}

void ult_yield() {
    int oldThreadId = currentThreadId;
    currentThreadId = (currentThreadId + 1) % 3;
    swapcontext(&threads[oldThreadId].ctx, &threads[currentThreadId].ctx);
}

int ult_join(int tid, int *status) {
    if (threads[tid].status != RUNNABLE) return -1;
    while (threads[tid].status != FINISHED) {
        ult_yield();
    }
    *status = threads[tid].exitCode;
    return 0;
}

void ult_exit(int status) {
//    printf("Exiting thread %d...\n", currentThreadId);
    threads[currentThreadId].status = FINISHED;
    threads[currentThreadId].exitCode = status;
}

void die(char *txt) {
    printf("%s\n", txt);
    exit(-1);
}

int readyToRead(unsigned int fd) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    int isReady;

    if ((isReady = select(fd + 1, &rfds, NULL, NULL, &tv)) == -1) {
        die("error calling select\n");
    }
    return isReady;
}

ssize_t ult_read(int fd, void *buf, size_t size) {
    if (!readyToRead((unsigned int) fd)) {
        ult_yield();
    }
    return read(fd, buf, size);
}
