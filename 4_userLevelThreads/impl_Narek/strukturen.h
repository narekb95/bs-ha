#ifndef STRUCKT_H
#define STRUCKT_H

struct tcb_s;

void ds_initStrukts();

int tidCounter;
typedef struct threadList_ {
    struct threadList_ *next, *prev;
    struct tcb_s *tcb;
    int tid;
} threadList;
threadList *threadRoot;

threadList *ds_addThread(struct tcb_s *data);

void ds_removeThread(threadList *elem);

threadList *ds_findElement(int tid);


typedef struct readBlockedList_ {
    struct readBlockedList_ *next, *prev;
    threadList *data;
    int fd;
} readBlockedList;
readBlockedList *readBlockRoot;

void ds_addReadBlocked(threadList *data, int fd);

void ds_removeReadBlocked(readBlockedList *elem);


typedef struct joinBlockedList_ {
    struct joinBlockedList_ *next, *prev;
    threadList *data;
    int tid;
} joinBlockedList;
joinBlockedList *joinBlockRoot;

void ds_addJoinBlocked(threadList *data, int tid);

void ds_removeJoinBlocked(joinBlockedList *elem);


typedef struct queue_ {
    struct queue_ *next;
    threadList *data;
} queue;
queue *queueFirst, *queueLast;

void ds_queuepush(threadList *elem);

void ds_queuepop();

threadList *ds_queueFront();

int ds_isQueueEmpty();

#endif
