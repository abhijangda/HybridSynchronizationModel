#include "synchronising_thread.h"

#ifndef __WRITER_H__
#define __WRITER_H__

typedef struct Worker
{
    int index;
    struct SynchronisingThread *mth;
    pthread_mutex_t mutex;
    pthread_t pthread_worker;
    void * (*worker_thread_func) (void *arg);
} Worker;

Worker *worker_new (SynchronisingThread *mth, int index,
                    void *worker_thread_func (void *arg));
void *worker_thread_func (void *arg);
void worker_operate (Worker *worker, Operation operation_func);
void worker_join (Worker *worker);
int worker_start (Worker *worker);

#endif /*__WRITER_H__*/
