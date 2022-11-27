#ifndef __ELF_H__
#define __EFL_H__

// #include "queue.h"
#include "gqueue.h"
#include "event.h"
#include <pthread.h>

typedef struct elf_loop
{
    int loop_id;
    // queue(type) queue
} elf_loop_t;

// use enums for statuses? For now am using int
typedef int elf_status_t;

typedef struct elf_handler
{
    int tmp;
    // not sure what instructions want here
} elf_handler_t;

typedef struct elf_event_loop
{
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    queue_t queue;
} elf_event_loop_t;

#endif /* __ELF_H__ */
