#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct a{
	int number;
	void* data;
	int index_num;
	int is_alloc;
};
#define num_elements 100

pthread_mutex_t events_lock = PTHREAD_MUTEX_INITIALIZER;
struct a events[num_elements];

struct a* alloc(){
	struct a* temp = 0;
	pthread_mutex_lock(&events_lock);
	for(int i=0; i<num_elements; i++){
		if(events[i].is_alloc == 0){
			temp = &events[i];
			temp->index_num = i;
			temp->data = 0;
			temp->number = 0;
			temp->is_alloc = 1;
			break;
		}
	}
	pthread_mutex_unlock(&events_lock);
	return temp;
}

void a_free(struct a* var){
	pthread_mutex_lock(&events_lock);
	if(var->is_alloc){
		var->is_alloc = 0;
	}
	pthread_mutex_unlock(&events_lock);
}

void init_events(){
	for(int i = 0; i < num_elements; i++){
		events[i].is_alloc = 0;
	}
}

void* work(void* data){
	for(int i = 0; i < 10000; i++){
		struct a* busy_work[76];
		for(int j = 0; j <= 75; j++){
			busy_work[j] = alloc();
		}

		for(int j = 0; j <= 75; j++){
			if(busy_work[j] != 0){
				a_free(busy_work[j]);
			}
		}
	}
}

struct queue{
	pthread_mutex_t queues_lock;
	struct a* events[num_elements];
	int beg, end;
	int count_queues;

};

void init_queue(struct queue* temp){
	temp->beg = -1;
	temp->end = 0;
	temp->count_queues = 0;
	//temp->queues_lock PTHREAD_MUTEX_INITIALIZER;
}

void queue_event(struct a* event, struct queue* tmp_queue  ){
	pthread_mutex_lock(&tmp_queue->queues_lock);
	tmp_queue->events[tmp_queue->end] = event;
	tmp_queue->end++;
	if(tmp_queue->end == num_elements){
		tmp_queue->end = 0;
	}
	tmp_queue->count_queues++;
	pthread_mutex_unlock(&tmp_queue->queues_lock);
}

struct a* dequeue_event(struct queue* tmp_queue){
	pthread_mutex_lock(&tmp_queue->queues_lock);
	struct a* tmp = 0;
	if(tmp_queue->count_queues > 0){
		tmp = tmp_queue->events[tmp_queue->beg];
		tmp_queue->beg++;
		if(tmp_queue->beg > num_elements){
			tmp_queue->beg = 0;
		}
		tmp_queue->count_queues--;
	}
	pthread_mutex_unlock(&tmp_queue->queues_lock);
	return tmp;	
}



void test_queues(){
	struct queue q;
	init_queue(&q);
	for(int i = 0; i < 5; i++){
		struct a* tst1 = alloc();
		tst1->number = i;
		queue_event(tst1, &q);
	}	

	for(int i = 0; i < 5; i++){
		struct a* tst1 = dequeue_event(&q);
		printf("%d\n", tst1->number);
		a_free(tst1);
	}
}

int main(int argc, char** argv){
	pthread_t thrd1;
	pthread_t thrd2;

	init_events();
	struct a* st_a;
	struct a* st_b;
	while(st_a = alloc()){
		printf("index = %d\n", st_a->index_num);
		if(st_a->index_num == 50){
			st_b = st_a;
		}
	}
	printf("No more free events\n");
	a_free(st_b);
	st_b = alloc();
	printf("%d\n",st_b->index_num);
	a_free(st_b);

	void* x;

	pthread_create(&thrd1, 0, work, 0);
	pthread_create(&thrd2, 0, work, 0);

	pthread_join(thrd1, &x);
        pthread_join(thrd2, &x);

	test_queues();	
	return 0;
}
#if 0
event types;
	1. generate num
	2. subtract 1
	3. print result
1 queue taking multiple event types from threads
Threads
	1. put events in queue (say 500)
		allocate event
		send to queue with relevant info (num is generated number)

	2. service the queue
#endif
