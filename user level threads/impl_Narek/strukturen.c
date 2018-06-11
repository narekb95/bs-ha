#include <stdlib.h>
#include "strukturen.h"

// [todo] on exit check if element still in queue take it out
void ds_initStrukts() {
    readBlockRoot = NULL;
    joinBlockRoot = NULL;
    queueFirst = NULL;
    queueLast = NULL;
    threadRoot = NULL;
}

void ds_queuepush(threadList *data) {
    queue *elem = malloc(sizeof(queue));
    if (isQueueEmpty()) {
        queueFirst = queueLast = elem;
    } else {
        queueLast->next = elem;
        queueLast = elem;
    }
    elem->data = data;
    elem->next = NULL;
}

void ds_queuepop() {
    queue *first = queueFirst;
    queueFirst = queueFirst->next;
    free(first);
}

threadList *ds_queueFront() {
    return queueFirst->data;
}

int ds_isQueueEmpty() {
    return (queueFirst == NULL);
}

int ds_addThread(struct tcb_s *data) {
    threadList *elem = malloc(sizeof(threadList));
    elem->next = threadRoot;
    threadRoot->prev = elem;
    elem->tcb = data;
    elem->prev = NULL;
    threadRoot = elem;
    return (elem->tid = tidCounter++);
}

void ds_removeThread(threadList *elem) {
    if (elem->prev != NULL) {
        (elem->prev)->next = elem->next;
    } else {
        threadRoot = elem->next;
    }
    if (elem->next != NULL) {
        (elem->next)->prev = elem->prev;
    }
    free(elem);
}

threadList *ds_findElement(int tid) {
    threadList *tmp;
    for (tmp = threadRoot; tmp != NULL; tmp = tmp->next) {
        if (tmp->tid == tid) {
            return tmp;
        }
    }
    return NULL;
}

//read blocked
void ds_addReadBlocked(threadList *data, int fd) {
    readBlockedList *elem = malloc(sizeof(readBlockedList));
    elem->next = readBlockRoot;
    readBlockRoot->prev = elem;
    elem->data = data;
    elem->prev = NULL;
    elem->fd = fd;
    readBlockRoot = elem;
}

void ds_removeReadBlocked(readBlockedList *elem) {
    if (elem->prev != NULL) {
        (elem->prev)->next = elem->next;
    } else {
        readBlockRoot = elem->nxt;
    }
    if (elem->next != NULL) {
        (elem->next)->prev = elem->prev;
    }
    free(elem);
}


//joind block
void ds_addJoinBlocked(threadList *data, int tid) {
    joinBlockedList *elem = malloc(sizeof(joinBlockedList));
    elem->next = joinBlockRoot;
    joinBlockRoot->prev = elem;
    elem->data = data;
    elem->prev = NULL;
    elem->tid = tid;
    joinBlockRoot = elem;
}

void ds_removeJoinBlocked(joinBlockedList *elem) {
    if (elem->prev != NULL) {
        (elem->prev)->next = elem->next;
    } else {
        joinBlockRoot = elem->nxt;
    }
    if (elem->next != NULL) {
        (elem->next)->prev = elem->prev;
    }
    free(elem);
}
