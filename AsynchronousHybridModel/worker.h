#include "synchronising_thread.h"

#ifndef __WORKER_H__
#define __WORKER_H__

typedef struct _Worker
{
    int index;
    Queue *queue;
    SynchronisingThread *sync_thread;
    void * (*worker_thread_func) (void *arg);
    pthread_t pthread_worker;
} Worker;

Worker *worker_new (SynchronisingThread *q, int index, 
                    void * (*thread_func) (void *arg));
void worker_operate (Worker *w, Operation operation_func);
int worker_start (Worker *worker);
void worker_join (Worker *worker);
void worker_free (Worker *worker);

#endif /* __WORKER_H__ */
