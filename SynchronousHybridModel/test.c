#include <stdio.h>
#include <stdlib.h>

#include "synchronising_thread.h"
#include "worker.h"
static int N;
typedef struct _Data
{
    long value;
} Data;

void *operation_func (void *data)
{
    Data *data2;
    int i;
    
    data2 = data;
    for (i = 0; i < N; i++)
    {
       data2->value += 1;
    }
    return NULL;
}

void *worker_thread_func (void *arg)
{
    Worker *worker;
    int i;
    
    worker = arg;

    for (i = 0; i < 100; i++)
    {
        worker_operate (worker, operation_func);
    }
}

int main ()
{
    float time = 0;
    int valid = 1;
    //FILE *f = fopen ("synclocklessresultsN", "a");
    int n = 10;
    N = 10000;
    //while ((N+=1000) <= 100000)
    {
        int i = 0;
        time = 0;
        while (i++ < 10){
        clock_t t1, t2;
        Data data;
        SynchronisingThread *mainThread;
        Worker **workers;
        data.value = 0;
        t1 = clock ();  
        
        mainThread = synchronising_thread_new (n, &data);
        int a = synchronising_thread_start (mainThread);
        if (a)
        {
           printf ("Error creating Main Thread");
        }

        workers = malloc (sizeof (Worker *) *n);
        int i;
        for (i = 0; i < n; i++)
        {
            workers[i] = worker_new (mainThread, i, worker_thread_func);
            int ans = worker_start (workers[i]);
    
            if (ans)
            {
                printf ("Error creating thread %d", i);
            }
        }    
        
        for (i = 0; i < n; i++)
        {
            worker_join (workers[i]);
            free (workers[i]);
        }
        free (workers);
        synchronising_thread_exit (mainThread, 1);
        synchronising_thread_join (mainThread);
        synchronising_thread_free (mainThread);
        //free (mainThread);
        t2 = clock ();
        printf ("%d\n", data.value);
        time += (((float)t2 - (float)t1) / 1000000.0 ) * 1000;
        }
        printf ("Timeout: %f for N: %d\n", time/i, N);
        //fprintf (f, "Timeout: %f for N: %d\n", time/i, N);
        //fflush(f);
     }
    //fclose (f);
    return 0;
}
