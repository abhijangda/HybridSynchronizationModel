#include <stdlib.h>

#include "worker.h"

Worker *worker_new (SynchronisingThread *mth, int index,
                    void *(*thread_func) (void *arg))
{
    Worker *worker = malloc (sizeof (Worker));
    worker->mth = mth;
    worker->index = index;
    pthread_mutex_init (&worker->mutex, NULL);
    worker->worker_thread_func = thread_func;
    
    return worker;
}

void worker_operate (Worker *worker, Operation operation)
{
    synchronising_thread_operate (worker->mth, worker->index, operation);
    pthread_cond_wait (&worker->mth->condVars[worker->index], &worker->mutex);
}

int worker_start (Worker *worker)
{
    return pthread_create (&worker->pthread_worker, NULL, 
                           worker->worker_thread_func, worker);
}

void worker_join (Worker *worker)
{
    pthread_join (worker->pthread_worker, NULL);
    return;
}

void worker_free (Worker *worker)
{
    free (worker);
}
