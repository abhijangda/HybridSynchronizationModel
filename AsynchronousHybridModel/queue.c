#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

Queue *queue_new ()
{
    Queue *q;

    q = malloc (sizeof (Queue));
    q->tail = 0;
    q->head = 0;
    q->array_ops = malloc (sizeof (Operation *));
    q->array_ops[0] = malloc (sizeof (Operation) * SIZE);
    q->n_array_ops = 1;
    pthread_mutex_init (&q->mutex, NULL);

    return q;
}

void queue_enqueue (Queue *q, void* (*op)(void *))
{
    int array = q->tail/SIZE;
    int index = q->tail - array*SIZE;
    
    q->array_ops [array][index] = op;    
    if (q->tail + 1 == q->n_array_ops*SIZE)
    {
        int head_array;
        int i;
        int n;
        Operation **arrayOpsTemp;
        Operation **to_be_freed;

        head_array = q->head/SIZE;
        n = q->n_array_ops + 1;
        arrayOpsTemp = malloc (sizeof (Operation *)*n);

        for (i = 0; i < head_array; i++)
        {
            arrayOpsTemp[i] = NULL;
        }

        for (i = head_array; i < q->n_array_ops; i++)
        {
            arrayOpsTemp[i] = q->array_ops [i];
        }
        
        arrayOpsTemp [i] = malloc (sizeof (Operation) * SIZE);
        to_be_freed = q->array_ops;
        q->n_array_ops = n;
        pthread_mutex_lock (&q->mutex);
        q->array_ops = arrayOpsTemp;

        for (i = 0; i < head_array; i++)
        {
            free (to_be_freed[i]);
        }

        free (to_be_freed);
        pthread_mutex_unlock (&q->mutex);
    }

    q->tail += 1;
}

int queue_dequeue (Queue *q, void *(**op)(void*))
{
    void* (*ops)(void *);
    int array;
    int index;

    if (q->tail == q->head)
    {
        return 0;
    }

    if (pthread_mutex_trylock (&q->mutex) != 0)
    {
        return -1;
    }

    array = q->head/SIZE;
    index = q->head - array*SIZE;
    *op = q->array_ops [array][index];
    q->head += 1;
    pthread_mutex_unlock (&q->mutex);

    return 1;
}

int queue_is_empty (Queue *q)
{
    if (q->tail == q->head)
    {
        return 1;
    }

    return 0;
}

void queue_free (Queue *q)
{
    int i;
    
    for (i = 0; i < q->n_array_ops; i++)
    {
        if (q->array_ops[i])
        {
            free (q->array_ops[i]);
        }
    }

    free (q->array_ops);
    free (q);
}
