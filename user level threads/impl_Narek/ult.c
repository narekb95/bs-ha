#include "ult.h"
#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <ucontext.h>
#include "strukturen.h"

#define STACK_SIZE 64*1024

void ult_callnextfunction();

typedef struct tcb_s {
    ucontext_t context;
    char heapData[STACK_SIZE];
    int tid;
} tcb_t;

tcb_t *main_context, *current_context;

enum ContextStatus {
    ext = 0, yld = 1, jn = 2, rd = 3
} contxStatus;

void ult_init(ult_f f) {
    tcb_t *tcb = malloc(sizeof(tcb_t));
    getcontext(&tcb->context);

    // create the new stack
    tcb->context.uc_link = 0;
    tcb->context.uc_stack.ss_flags = 0;
    tcb->context.uc_stack.ss_size = STACK_SIZE;
    tcb->context.uc_stack.ss_sp = tcb->heapData;


    if (self->cibtext.uc_stack.ss_sp == NULL) {
        fprintf(stderr, "error initializing the library\n");
    }
    makecontext(&self->context, f, 0);


    threadList thread = addThred(tcb);
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


    if (self->cibtext.uc_stack.ss_sp == NULL) {
        fprintf(stderr, "error initializing the library\n");
    }
    makecontext(&self->context, f, 0);


    threadList thread = addThred(tcb);
    tcb->tid = thread->tid;
    ds_queuepush(thread);
    return tcb->tid;
}

//enum ContextStatus { ext = 0, yld = 1, jn = 2, rd = 3} contxStatus;

//put context back to queue and call next
void ult_yield()//sawp context to main_context
{
    contxStatus = yld;
    swapcontext(current_context->context, main_context->context);
}

//delte thread and call next
void ult_exit(int status) {
    contxStatus = ext;
    swapcontext(current_context->context, main_context->context);
}

//put context in join blocked and call next
int ult_join(int tid, int *status) {
    contxStatus = jn;
    swapcontext(current_context->context, main_context->context);
}

//[todo]
//if data available then return data (maximally size)
//else put context in read blocked and call next
ssize_t ult_read(int fd, void *buf, size_t size) {
//    if(isfdleer)
//    {
//        contxStatus = rd;
//        //set fd
//        swapcontext(current_context, main_context);
//    }
//    else
//    {
//        //read data
//    }
//    return 0;
}

//[todo]
bool readyToRead(int fd);

bool alreadyJoined(int tid);

void ult_callnextfunction() {
    while (threadRoot != NULL) {
        readBlockedList *readBlockedTmp = readBlockRoot;
        while (readBlockedtmp != NULL) {
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
        while (joinBlockedtmp != NULL) {
            if (alreadyJoined(joinBlockedTmp->fd)) {
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
            current_context = elem->data;
            ds_queuepop();
            swapcontext(main_context->context, current_context->context);
            switch (ContextStatus) {
                case ext:
                    ds_removeThread(elem);
                    break;
                case yld:
                    ds_queuepush(elem);
                    break;
                case rd:
                    ds_addReadBlocked(elem);
                    break;
                case jn:
                    ds_addJoinBlocked(elem);
                    break;
            }
        }
    }
}
