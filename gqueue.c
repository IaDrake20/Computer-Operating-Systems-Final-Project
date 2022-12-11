#include "gqueue.h"
#include <pthread.h>
#include <stdio.h>

// initialize a queue
void queue_init(queue_t *q)
{
    q->beg = 0;
    q->end = 0;
    q->count = 0;
}

// add an element to the back of the queue
// returns 0 when queue is full, 1 on successful add
// TODO: lock isn't being opened by the thread
int queue_enqueue(queue_t *q, void *elem)
{
    if (queue_full(q))
    {
        return 0;
    }

    printf("Locking mutex");
    pthread_mutex_lock(&q->queue_lock);

    q->elems[q->end] = elem;
    q->end++;
    if (q->end == queue_magic_total_elems)
    {
        q->end = 0;
    }
    q->count++;

    pthread_mutex_unlock(&q->queue_lock);
    return 1;
}

// remove an element from the front of the queue
// returns ptr to dequeue'd element, or 0 if queue is empty
void *queue_dequeue(queue_t *q)
{
    if (queue_empty(q))
    {
        return 0;
    }

    pthread_mutex_lock(&q->queue_lock);

    void *elem = 0;
    elem = q->elems[q->beg];

    q->beg++;
    if (q->beg > queue_magic_total_elems)
    {
        q->beg = 0;
    }
    q->count--;

    pthread_mutex_unlock(&q->queue_lock);
    return elem;
}

// get the element at the front of the queue (next to be removed) without removing it
// returns element at the front of the queue, or 0 if queue empty
void *queue_front(queue_t *q)
{
    if (queue_empty(q))
    {
        return 0;
    }

    return q->elems[q->beg];
}

// get the element at the back of the queue (most recently added) without removing it
// returns element at the back of the queue, or 0 if queue empty
void *queue_rear(queue_t *q)
{
    if (queue_empty(q))
    {
        return 0;
    }

    return q->elems[q->end];
}

// returns true if queue is empty, else false
bool queue_empty(queue_t *q)
{
    return q->count == 0;
}

// returns true if queue is full, else false
bool queue_full(queue_t *q)
{
    return q->count >= queue_magic_total_elems;
}

// returns the size of the queue (current num elements)
int queue_size(queue_t *q)
{
    return q->count;
}