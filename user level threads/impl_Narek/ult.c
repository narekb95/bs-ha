#include "ult.h"
#include "strukturen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#include <ucontext.h>


#define STACK_SIZE (8*1024)

int exitcodes[10000];

typedef struct tcb_s {
    ucontext_t context;
    char heapData[STACK_SIZE];
    int tid;
} tcb_t;

tcb_t *main_context, *current_context;

enum ContextStatus {
    ext = 0, yld = 1, jn = 2, rd = 3
} contxStatus;
int waitingfd, waitingtid, exitcode;


int readyToRead(int fd);

int alreadyJoined(int tid);

void die(char *txt);

void callnextfunction();

void callnextfunction() {
    while (threadRoot != NULL) {
        readBlockedList *readBlockedTmp = readBlockRoot;
        while (readBlockedTmp != NULL) {
            if (readyToRead(readBlockedTmp->fd)) {
                ds_queuepush(readBlockedTmp->data);
                readBlockedList *tmp = readBlockedTmp;
                readBlockedTmp = readBlockedTmp->next;
                ds_removeReadBlocked(tmp);
            } else {
                readBlockedTmp = readBlockedTmp->next;
            }
        }


        joinBlockedList *joinBlockedTmp = joinBlockRoot;
        while (joinBlockedTmp != NULL) {
            if (alreadyJoined(joinBlockedTmp->tid)) {
                ds_queuepush(joinBlockedTmp->data);
                joinBlockedList *tmp = joinBlockedTmp;
                joinBlockedTmp = joinBlockedTmp->next;
                ds_removeJoinBlocked(tmp);
            } else {
                joinBlockedTmp = joinBlockedTmp->next;
            }
        }

        if (!ds_isQueueEmpty()) {
            threadList *elem = ds_queueFront();
            current_context = elem->tcb;
            ds_queuepop();
            swapcontext(&main_context->context, &current_context->context);
            switch (contxStatus) {
                case ext:
                    exitcodes[elem->tid] = exitcode;
                    ds_removeThread(elem);
                    break;
                case yld:
                    ds_queuepush(elem);
                    break;
                case rd:
                    ds_addReadBlocked(elem, waitingfd);
                    break;
                case jn:
                    ds_addJoinBlocked(elem, waitingtid);
                    break;
            }
        }
    }
}

void die(char *txt) {
    printf("%s\n", txt);
    exit(-1);
}

void ult_init(ult_f f) {
    ds_initStrukts();
    main_context = malloc(sizeof(tcb_t));
    tcb_t *tcb = malloc(sizeof(tcb_t));

    getcontext(&tcb->context);
    tcb->context.uc_link = 0;
    tcb->context.uc_stack.ss_flags = 0;
    tcb->context.uc_stack.ss_size = STACK_SIZE;
    tcb->context.uc_stack.ss_sp = tcb->heapData;


    if (tcb->context.uc_stack.ss_sp == NULL) {
        die("error initializing the library\n");
    }
    makecontext(&tcb->context, f, 0);

    threadList *thread = ds_addThread(tcb);
    tcb->tid = thread->tid;
    ds_queuepush(thread);

    callnextfunction();
}

int ult_spawn(ult_f f) {
    tcb_t *tcb = malloc(sizeof(tcb_t));
    getcontext(&tcb->context);

    // create the new stack
    tcb->context.uc_link = 0;
    tcb->context.uc_stack.ss_flags = 0;
    tcb->context.uc_stack.ss_size = STACK_SIZE;
    tcb->context.uc_stack.ss_sp = tcb->heapData;


    if (tcb->context.uc_stack.ss_sp == NULL) {
        fprintf(stderr, "error initializing the library\n");
    }
    makecontext(&tcb->context, f, 0);


    threadList *thread = ds_addThread(tcb);
    tcb->tid = thread->tid;
    ds_queuepush(thread);
    return tcb->tid;
}

void ult_yield() {
    contxStatus = yld;
    swapcontext(&current_context->context, &main_context->context);
}

void ult_exit(int status) {
    printf("utl_exit %d", status);
    contxStatus = ext;
    exitcode = status;
    swapcontext(&current_context->context, &main_context->context);
}

int ult_join(int tid, int *status) {
    if (ds_findElement(tid)) {
        contxStatus = jn;
        waitingtid = tid;
        swapcontext(&current_context->context, &main_context->context);
    }
    return *status = exitcodes[tid];
}

//for efficiency reasons select should be called for a set over all fds we are waiting on and then check
//the set for every thread instead of calling select for every single thread
int readyToRead(int fd) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    int isReady;

    if ((isReady = select(fd + 1, &rfds, NULL, NULL, &tv)) == -1) {
        die("error calling select\n");
    }
    return isReady;
}

int alreadyJoined(int tid) {
    return (ds_findElement(tid) == NULL);
}

ssize_t ult_read(int fd, void *buf, size_t size) {
    if (!readyToRead(fd)) {
        contxStatus = rd;
        waitingfd = fd;
        swapcontext(&current_context->context, &main_context->context);
    }
    return read(fd, buf, size);
}
