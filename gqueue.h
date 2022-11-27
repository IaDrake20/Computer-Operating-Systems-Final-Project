#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <pthread.h>

#define queue_magic_total_elems 100

struct queue
{
    pthread_mutex_t queue_lock;
    int beg, end;
    int count;
    int size;
    void *elems[queue_magic_total_elems];
};
typedef struct queue queue_t;

void queue_init(queue_t *q);
int queue_enqueue(queue_t *q, void *elem);
void *queue_dequeue(queue_t *q);
void *queue_front(queue_t *q);
void *queue_rear(queue_t *q);
bool queue_empty(queue_t *q);
bool queue_full(queue_t *q);
int queue_size(queue_t *q);

#endif