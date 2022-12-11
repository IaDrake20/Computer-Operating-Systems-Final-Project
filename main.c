#include "elf.h"
// #include "queue.h"
#include "event.h"
#include "gqueue.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// void *pthread_action(void *args)
// {
// 	printf("Hello from pthread!!\n");
// 	sleep(1);
// }
int main()
{
	// initialize the queue
	queue_t q;
	queue_init(&q);
	printf("queue initialized.\n");

	// add an int to queue
	int elem = 4;
	queue_enqueue(&q, &elem);//(IAN): not getting past this point as thread refuses to unlock
	printf("int added to queue (4).\n");

	// remove the int from the queue
	int elem2;
	elem2 = *(int *)queue_dequeue(&q);
	printf("int removed from queue with value: %d.\n", elem2);

	// try adding and removing a char
	char c = 'c';
	queue_enqueue(&q, &c);
	char c2;
	c2 = *(char *)queue_dequeue(&q);
	printf("char added and removed from queue with data: %c.\n", c2);

	// try adding a 3 things, removing 2, adding 1, removing 2, to see if it holds things correctly
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	queue_enqueue(&q, &a1);
	queue_enqueue(&q, &a2);
	queue_enqueue(&q, &a3);
	printf("3 ints added to queue (1,2,3)\n");

	int r1, r2, r3, r4;
	r1 = *(int *)queue_dequeue(&q);
	r2 = *(int *)queue_dequeue(&q);
	printf("2 ints removed from queue with values: %d, %d\n", r1, r2);

	queue_enqueue(&q, &a4);
	printf("another int added to queue (currently should have 3,4)\n");

	r3 = *(int *)queue_dequeue(&q);
	r4 = *(int *)queue_dequeue(&q);
	printf("removed last 2 ints from queue with values: %d, %d\n", r3, r4);

	// try using a struct in the queue (this would be like an event)
	typedef struct Point
	{
		int x;
		int y;
	} Point;

	Point *p = (Point *)malloc(sizeof(Point));
	p->x = 5;
	p->y = 10;
	printf("point struct instance created\n");

	queue_enqueue(&q, p);
	printf("point added to queue with values: (x=5, y=10)\n");

	Point *p2;
	p2 = (Point *)queue_dequeue(&q);
	printf("point removed from queue with values: (x=%d, y=%d)\n", p2->x, p2->y);

	// test queue_empty function
	bool empty = queue_empty(&q);
	printf("is the queue empty?: %s\n", empty ? "yes" : "no");

	queue_enqueue(&q, p);
	printf("point inserted back into the queue\n");

	empty = queue_empty(&q);
	printf("is the queue empty?: %s\n", empty ? "yes" : "no");

	Point *p3;
	p3 = (Point *)queue_dequeue(&q);
	printf("point removed from queue again with values: (x=%d, y=%d)\n", p3->x, p3->y);

	// test putting an event in the queue
	elf_event_t *e = (elf_event_t *)malloc(sizeof(elf_event_t));
	int e_data = 7;
	init_event(e, &e_data);
	printf("event initialized\n");

	int e_data_r = *(int *)get_data(e);
	printf("data retrieved from event: %d\n", e_data_r);

	queue_enqueue(&q, e);
	printf("elf_event_t inserted into queue with data=7\n");

	elf_event_t *e2 = (elf_event_t *)malloc(sizeof(elf_event_t));
	e2 = (elf_event_t *)queue_dequeue(&q);
	printf("elf_event_t removed from queue\n");

	int e2_data_r = *(int *)get_data(e2); // segmentation fault here
	printf("data retrieved from the removed elf_event_t: %d.\n", e2_data_r);

	// todo:
	// test if circularity of queue works


}
