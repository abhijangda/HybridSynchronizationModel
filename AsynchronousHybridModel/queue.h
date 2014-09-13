#ifndef __QUEUE_H__
#define __QUEUE_H__
#define SIZE 20

#include <pthread.h>
typedef void*(*Operation)(void *);

typedef struct _Queue
{
    int tail;
    int head;    
    Operation **array_ops;
    int n_array_ops;
    pthread_mutex_t mutex;
} Queue;

Queue *queue_new ();
void queue_enqueue (Queue *q, void* (*op)(void *));
//void *queue_dequeue (Queue *q);
int queue_dequeue (Queue *q, void*(**op)(void*));
int queue_is_empty (Queue *q);
void queue_free (Queue *q);

#endif /*__QUEUE_H__*/
