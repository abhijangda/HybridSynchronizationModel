#include <stdlib.h>
#include <stdio.h>

#include "synchronising_thread.h"

static void *synchronising_thread_func (void *arg);

SynchronisingThread * synchronising_thread_new (int size, void *data)
{
    SynchronisingThread *mainthread;
    int i;
 
    mainthread = malloc (sizeof (SynchronisingThread));
    mainthread->queues = malloc (sizeof (Queue *)*size);

    for (i = 0; i < size; i++)
    {
        mainthread->queues [i] = queue_new ();
    }

    mainthread->data = data;
    mainthread->exit = 0;
    mainthread->size = size;
    
    return mainthread;
}

void synchronising_thread_exit (SynchronisingThread *mth, int exit)
{
    mth->exit = exit;
}

void *synchronising_thread_func (void *arg)
{
    SynchronisingThread *mainThread;
    int allempty;

    mainThread = arg;
    allempty = 0;

    while (!mainThread->exit || !allempty)
    {
        int i;

        allempty = 1;

        for (i = 0; i < mainThread->size; i++)
        {
            Operation operation;
            int q = queue_dequeue (mainThread->queues[i], &operation);
            
            if (q == -1 || q == 0)
            {
                continue;
            }
            
            allempty = 0;
            operation (mainThread->data);
        }
    }
}

void synchronising_thread_free (SynchronisingThread *syncth)
{
    int i;

    for (i = 0; i < syncth->size; i++)
    {
        queue_free (syncth->queues[i]);
    }
    
    free (syncth->queues);
    free (syncth);
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
