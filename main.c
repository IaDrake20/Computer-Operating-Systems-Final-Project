#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//not sure how many events to expect or whether we should add a resize to the queue
#define magic_total_events 100

struct queue{
        int beg, end;
        int count_queues;
        //array of whatever we are holding in the queues, I presume elf_event_t?
        struct elf_event_t* events[magic_total_events];
};

struct elf_event_t{
        int event_id;
        void *data;
};
struct elf_loop_t{
        int loop_id;
        //queue(type) queue
};
struct elf_status_t{
        //use enums for statuses?
};

struct elf_handler_t{
        //finish me lol
};
struct elf_event_loop{
        pthread_mutex_t lock;
        pthread_cond_t not_empty;
        queue_t queue;

};
void *pthread_action(void *args)
{
    printf("Hello from pthread!!\n");
    sleep(1);
}

int main()
{
    pthread_t p;
    pthread_create(&p, NULL, pthread_action, NULL);
    printf("Hello from main thread\n");
    pthread_join(p, NULL);
    printf("Hello after pthread join\n");
}
