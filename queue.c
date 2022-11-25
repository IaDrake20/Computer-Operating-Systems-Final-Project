#include "queue.h"

void init_queue(queue_t *tmp)
{
    tmp->beg = -1;
    tmp->end = 0;
    tmp->count_queues = 0;
}

void queue_event(elf_event_t *event, queue_t *q)
{
    pthread_mutex_lock(&q->queue_lock);
    q->events[q->end] = event;
    q->end++;
    if (q->end == magic_total_events)
    {
        q->end = 0;
    }
    q->count_queues++;
    pthread_mutex_unlock(&q->queue_lock);
}

elf_event_t *dequeue_event(queue_t *q)
{
    pthread_mutex_lock(&q->queue_lock);
    elf_event_t *tmp = 0;
    if (q->count_queues > 0)
    {
        tmp = q->events[q->beg];
        q->beg++;
        if (q->beg > magic_total_events)
        {
            q->beg = 0;
        }
        q->count_queues--;
    }
    pthread_mutex_unlock(&q->queue_lock);
    return tmp;
}
