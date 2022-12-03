#include "elf_queue.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct elf_queue_s * elf_queue_t;


elf_status_t elf_queue_new(elf_queue_t *ref_queue);
elf_status_t elf_queue_delete(elf_queue_t *ref_queue);
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event);
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event);

struct elf_queue_s {
    size_t cap;                         // max capacity of queue
    size_t size;                        // current num events in queue
    int beg;                            // index of beginning of queue
    int end;                            // index of end of queue
    elf_event_t events[ELF_QUEUE_CAP];  // simple implementation
    pthread_mutex_t lock;               // for multi-threading
    pthread_cond_t empty_cond;          // for multi-threading
};

/*
 * Creates a new multi-threaded unbounded queue. Capacity is fixed for simplicity.
 */
elf_status_t elf_queue_new(elf_queue_t *ref_queue) {

    *ref_queue = calloc(1, sizeof(elf_queue_t));

    pthread_mutex_init(&((*ref_queue)->lock), NULL);
    pthread_cond_init(&((*ref_queue)->empty_cond), NULL);
    (*ref_queue)->cap = ELF_QUEUE_CAP;
    (*ref_queue)->size = 0;
    (*ref_queue)->beg = 0;
    (*ref_queue)->end = 0;
    return ELF_OK;
}

/*
 * Delete queue.
 */
elf_status_t elf_queue_delete(elf_queue_t *ref_queue) {

    pthread_mutex_destroy(&((*ref_queue)->lock));
    pthread_cond_destroy(&((*ref_queue)->empty_cond));

    free(ref_queue);
    *ref_queue = NULL; // invalidate caller pointer
    return ELF_OK;
}


/*
 * Enqueues a void pointer. If the queue has reached maximal capacity, ELF_FULL is returned;
 * else, ELF_OK is returns. DOES NOT BLOCK.
 */
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event) {

    pthread_mutex_lock(&(queue->lock));

    // if queue full, return ELF_FULL
    if (queue->size == queue->cap) {
        return ELF_FULL;
    }

    // add event to end of queue
    queue->events[queue->end] = event;
    queue->end++;

    // if queue reached its length, wrap around to beginning
    if (queue->end == queue->cap)
    {
        queue->end = 0;
    }
    queue->size++;

    // send signal that queue is no longer empty
    pthread_cond_signal(&queue->empty_cond);

    pthread_mutex_unlock(&(queue->lock));

    return ELF_OK;
}

/*
 * Dequeues a void pointer. If the queue is empty, the function BLOCKS. Return ELF_OK.
 */
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event) {

    pthread_mutex_lock(&(queue->lock));

    // if queue empty, block (wait for enqueue to signal)
    if (queue->size == 0) {
        pthread_cond_wait(&(queue->empty_cond), &queue->lock);
    }

    // dequeue element at beg of queue and store in ref_event
    *ref_event = queue->events[queue->beg];

    queue->beg++;
    // if queue reached its length, wrap around to beginning
    if (queue->beg > queue->cap)
    {
        queue->beg = 0;
    }
    queue->size--;

    pthread_mutex_unlock(&(queue->lock));

    return ELF_OK;
}
