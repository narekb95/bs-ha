#include "ult.h"
#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ucontext.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>


static int newThreadId = 0;
static int currentThreadId = 0;
static int NUMBER_OF_ALL_THREADS = 3;

/* thread control block */
typedef struct tcb_s {
    ucontext_t ctx;
    bool done;
    int retCode;
} tcb_t;

struct timeval quantum;

static tcb_t *threads;

void saveDefaultContext(ult_f f);

void setSchedulerAlarms();

void time_handler(int signum);

void ult_init(ult_f f) {
    printf("ult_init\n");
    setSchedulerAlarms();
    saveDefaultContext(f);
    setcontext(&threads[0].ctx);
}

void saveDefaultContext(ult_f f) {
    threads = malloc(NUMBER_OF_ALL_THREADS * sizeof(tcb_t));

    // First, save current context
    getcontext(&threads[newThreadId].ctx);
    threads[newThreadId].ctx.uc_stack.ss_sp = malloc(8192);
    threads[newThreadId].ctx.uc_stack.ss_size = 8192;

    // This is the main context, the thread shall exit when it returns
    threads[newThreadId].ctx.uc_link = 0;

    // Now create the new context and specify what function it should run
    makecontext(&threads[newThreadId].ctx, f, 0);

    threads[newThreadId].done = 0;
    newThreadId++;
}

void setSchedulerAlarms() {
    struct sigaction sa;
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &time_handler;
    sa.sa_flags = 0;


    sigaction(SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after 250 msec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 250000;
    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;
    /* Start a virtual timer. It counts down whenever this process is
      executing. */
    puts("starting scheduler...");
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void time_handler(int signum) {
    puts("time_handler");
//    ult_yield();
}

int ult_spawn(ult_f f) {
    printf("Thread %d in ult_spawn for thread %d\n", currentThreadId, newThreadId);

    // First, create a valid execution context the same as the current one
    getcontext(&threads[newThreadId].ctx);

    // Now set up its stack
    threads[newThreadId].ctx.uc_stack.ss_sp = malloc(8192);
    threads[newThreadId].ctx.uc_stack.ss_size = 8192;

    // This is the context that will run when this thread exits
    threads[newThreadId].ctx.uc_link = &threads[currentThreadId].ctx;

    makecontext(&threads[newThreadId].ctx, f, 0);

    threads[newThreadId].done = false;
    return newThreadId++;
}

void ult_yield() {
    int oldThreadId = currentThreadId;
    currentThreadId = (currentThreadId + 1) % NUMBER_OF_ALL_THREADS;
    printf("Thread %d yielding to thread %d\n", oldThreadId, currentThreadId);
    printf("Thread %d calling swapcontext\n", oldThreadId);

    swapcontext(&threads[oldThreadId].ctx, &threads[currentThreadId].ctx);
    printf("Thread %d back from swapcontext\n", oldThreadId);
}

int ult_join(int tid, int *status) {
    printf("ult_join tid:%d, currentThreadId: %d\n", tid, currentThreadId);

    if (threads[tid].done) {
        status = &threads[tid].retCode;
        return 0;
    } else {
        ult_yield();
    }
}

void ult_exit(int status) {
    printf("Exiting thread %d...\n", currentThreadId);
    if (!threads[currentThreadId].done) {
        printf("Thread %d done succesfully!\n", currentThreadId);
        threads[currentThreadId].done = true;
        threads[currentThreadId].retCode = status;

        swapcontext(&threads[currentThreadId].ctx, 0);
        //ult_yield();
    } else {
        printf("Thread %d does not exist!\n", currentThreadId);
    }

}

ssize_t ult_read(int fd, void *buf, size_t size) {
    return 0;
}
