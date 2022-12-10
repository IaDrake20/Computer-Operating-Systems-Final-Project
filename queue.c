#include "queue.h"
#include <sempahore.h>
void init_queue(queue_t *tmp)
{
    tmp->beg = -1;
    tmp->end = 0;
    tmp->count_queues = 0;
}

void queue_event(elf_event_t *event, queue_t *q)
{
    sem_t q_sem;
    sem_init(&q_sem, 0, 1);
    int sem_wait(&q_sem);
    //pthread_mutex_lock(&q->queue_lock);
    q->events[q->end] = event;
    q->end++;
    if (q->end == magic_total_events)
    {
        q->end = 0;
    }
    q->count_queues++;
    //pthread_mutex_unlock(&q->queue_lock);
    int sem_post(&q_sem);
    sem_destroy(&q_sem);
}

elf_event_t *dequeue_event(queue_t *q)
{
    sem_t dq_sem;

    sem_init(&dq_sem, 0, 1);
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

    int sem_post(&dq_sem);
    pthread_mutex_unlock(&q->queue_lock);
    sem_destroy(&q_sem);
    return tmp;
}
