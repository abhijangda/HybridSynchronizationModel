#include <stdlib.h>

#include "worker.h"

Worker *worker_new (SynchronisingThread *syncth, int index, 
                    void * (*thread_func) (void *arg))
{
    Worker *worker;
    
    worker = malloc (sizeof (Worker));
    worker->sync_thread = syncth;
    worker->index = index;
    worker->worker_thread_func = thread_func;

    return worker;
}

void worker_free (Worker *worker)
{
    free (worker);
}

void worker_operate (Worker *worker, Operation operation_func)
{
    queue_enqueue (worker->sync_thread->queues[worker->index], operation_func);
}

int worker_start (Worker *worker)
{
    return pthread_create (&worker->pthread_worker, NULL, 
                           worker->worker_thread_func, worker);
}

void worker_join (Worker *worker)
{
    pthread_join (worker->pthread_worker, NULL);
}
