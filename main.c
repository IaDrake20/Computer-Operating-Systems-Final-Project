#include "elf.h"
#include "queue.h"
#include "event.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// void *pthread_action(void *args)
// {
// 	printf("Hello from pthread!!\n");
// 	sleep(1);
// }

// sometimes throws this error:
// 		main: tpp.c:82: __pthread_tpp_change_priority: Assertion `new_prio == -1 || (new_prio >= fifo_min_prio && new_prio <= fifo_max_prio)' failed.
// sometimes hangs
// sometimes works (until dequeue, then segmentation fault)
void test_queue()
{
	// initialize the queue
	queue_t q;
	init_queue(&q);
	printf("queue initialized.\n");

	// add event to queue
	elf_event_t e;
	init_event(&e, (void *)1);
	queue_event(&e, &q);
	printf("event added to queue (event contains data: 1).\n");

	// remove the event from the queue
	elf_event_t e2;
	e2 = *dequeue_event(&q);
	int *data = (int *)get_data(&e2); // segmentation fault here
	printf("event removed from queue with data: %d.\n", *data);
}

int main()
{
	test_queue();

	// pthread_t p;
	// pthread_create(&p, NULL, pthread_action, NULL);
	// printf("Hello from main thread\n");
	// pthread_join(p, NULL);
	// printf("Hello after pthread join\n");
	return 0;
}
