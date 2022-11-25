#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "event.h"
#include <pthread.h>

#define magic_total_events 100

typedef struct queue
{
    pthread_mutex_t queue_lock;
    int beg, end;
    int count_queues;
    elf_event_t *events[magic_total_events];
} queue_t;

void init_queue(queue_t *tmp);
void queue_event(elf_event_t *event, queue_t *q);
elf_event_t *dequeue_event(queue_t *q);

#endif /* __QUEUE_H__ */