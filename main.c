#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//may need to just add a resize function to queue, but for now this'll work
#define magic_total_events 100

struct queue{
	pthread_mutex_t queue_lock;
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
	//use enums for statuses? For now am using int
	int status;
};

struct elf_handler_t{
	//not sure what instructions want here
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

void init_queue(struct queue* tmp){
	tmp->beg = -1;
	tmp->end = 0;
	tmp->count_queues = 0;
}

void queue_event(struct elf_event_t* event, struct queue* q){
	pthread_mutex_lock(&q->queue_lock);
	q->events[q->end] = event;
	q->end++;
	if(q->end == magic_total_events){
		q->end = 0;
	}	
	q->count_queues++;
	pthread_mutex_unlock(&q->queue_lock);
}

struct elf_event_t* dequeue_event(struct queue* q){
	pthread_mutex_lock(&q->queue_lock);
	struct elf_event_t* tmp = 0;
	if(q->count_queue > 0){
		tmp = q->events[q->beg];
		q->beg++;
		if(q->beg > magic_total_events){
			q->beg = 0;
		}
		q->count_queues--;
	}
	pthread_mutex_unlock(&q->queue_lock);
	return tmp;
}
int main()
{
    pthread_t p;
    pthread_create(&p, NULL, pthread_action, NULL);
    printf("Hello from main thread\n");
    pthread_join(p, NULL);
    printf("Hello after pthread join\n");
}
