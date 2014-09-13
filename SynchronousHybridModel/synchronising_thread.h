#include <pthread.h>

#ifndef __SYNCHRONISING_THREAD_H__
#define __SYNCHRONISING_THREAD_H__

typedef void * (*Operation) (void *);

struct SynchronisingThread
{
    void *data;
    int exit;
    int size;
    Operation *arrayOps;
    pthread_cond_t *condVars;
    pthread_t pthread_syncth;
    short int *arrayStartOp;
};

typedef struct SynchronisingThread SynchronisingThread;

SynchronisingThread *synchronising_thread_new (int size, void *data);
void synchronising_thread_operate (SynchronisingThread *mth, int i, Operation operation);
void synchronising_thread_exit (SynchronisingThread *mth, int exit);
void *synchronising_thread_func (void *arg);
int synchronising_thread_start (SynchronisingThread *syncth);
void synchronising_thread_join (SynchronisingThread *syncth);
void synchronising_thread_free (SynchronisingThread *syncthread);
#endif /*__SYNCHRONISING_THREAD_H__*/
