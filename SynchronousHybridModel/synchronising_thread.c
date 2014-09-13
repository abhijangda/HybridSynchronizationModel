#include <stdlib.h>
#include <stdio.h>

#include "synchronising_thread.h"

SynchronisingThread *synchronising_thread_new (int size, void *data)
{
    SynchronisingThread *mainthread = malloc (sizeof (SynchronisingThread));

    mainthread->arrayOps = malloc (sizeof (Operation) * size);
    mainthread->condVars = malloc (sizeof (pthread_cond_t) * size);
    mainthread->arrayStartOp = malloc (sizeof (short int) * size);
    mainthread->data = data;

    for (int i = 0; i  < size; i++)
    {
        mainthread->arrayOps[i] = NULL;
        pthread_cond_init (&mainthread->condVars [i], NULL);
        mainthread->arrayStartOp[i] = 0;
    }

    mainthread->exit = 0;
    mainthread->size = size;
    
    return mainthread;
}

void synchronising_thread_operate (SynchronisingThread *mth, int index, Operation operation)
{
    mth->arrayOps [index] = operation;
    mth->arrayStartOp[index] = 1;
}

void synchronising_thread_exit (SynchronisingThread *mth, int exit)
{
    mth->exit = exit;
}

int synchronising_thread_start (SynchronisingThread *syncth)
{
    return pthread_create (&syncth->pthread_syncth, NULL, 
                           synchronising_thread_func, syncth);
}

void synchronising_thread_join (SynchronisingThread *syncth)
{
    pthread_join (syncth->pthread_syncth, NULL);
    return;
}

void *synchronising_thread_func (void *arg)
{
    SynchronisingThread *mainThread = arg;
    
    while (!mainThread->exit)
    {
        for (int i = 0; i < mainThread->size; i++)
        {
            if (mainThread->arrayStartOp[i])
            {
                Operation operation;

                operation = mainThread->arrayOps[i];
                operation (mainThread->data);
                mainThread->arrayOps[i] = NULL;
                mainThread->arrayStartOp[i] = 0;
                pthread_cond_signal (&mainThread->condVars[i]);
            }
        }
    }
}

void synchronising_thread_free (SynchronisingThread *syncthread)
{
    free (syncthread->arrayOps);
    free (syncthread->condVars);
    free (syncthread);
}
