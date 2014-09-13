#include "queue.h"

#ifndef __SYNCHRONISING_THREAD_H__
#define __SYNCHRONISING_THREAD_H__

typedef struct _SyncThread
{
    Queue **queues;
    void *data;
    int exit;
    int size;
    pthread_t pthread_syncth;
} SynchronisingThread;

SynchronisingThread * synchronising_thread_new (int size, void *data);
void synchronising_thread_exit (SynchronisingThread *mth, int exit);
int synchronising_thread_start (SynchronisingThread *syncth);
void synchronising_thread_join (SynchronisingThread *syncth);
void synchronising_thread_free (SynchronisingThread *syncth);
#endif /* __SYNCHRONISING_THREAD_H__ */
