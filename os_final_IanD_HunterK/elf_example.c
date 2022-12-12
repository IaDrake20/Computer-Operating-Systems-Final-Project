#include "elf_public.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

bool debug = false;

struct {
	uint32_t other_loop;
} state_collatz_p1 = {.other_loop = UINT8_MAX}; 

//Our example based on collatz conjecture... It takes in an int, if even divides by 2, otherwise multoply by 3 and add 1
elf_status_t handler_Collatz_P1(uint32_t self_id, elf_event_t event){
	
	// if other_loop is UINT8_MAX, the expected event contains other loops id, handle that
	if(state_collatz_p1.other_loop == UINT8_MAX){
		state_collatz_p1.other_loop = event.value.loop_id;
		return ELF_OK;
	}

	// if here, both loops should be initialized, so handle event with int
	int number = event.value.int32;
	assert(number != 0); //check null
	debug && printf("collatz1 received event with num: %d\n", number);
	printf("%d\n", number);

	// if number == 1, problem is solved, exit.
	if(number == 1) {
		printf("Finished calculating\n");
		// end both event loops
		elf_fini(state_collatz_p1.other_loop);
		elf_fini(self_id);

		// send message to main loop letting it know its finished.
		elf_send(0, elf_event_token());
		return ELF_OK;
	}

	// check if number is even, if so, divide by 2 and sent new event with new value to self
	if (number % 2 == 0) {
		debug && printf("number is even, collatz1 is dividing by 2, and sending event to self\n");
		sleep(1);
		int newNum = number / 2;
		elf_send(self_id, elf_event_int32(newNum));
	}
	// if number is not even, send event containing the number to other loop
	else {
		debug && printf("number is odd, collatz1 sending event to collatz2\n");
		sleep(1);
		elf_send(state_collatz_p1.other_loop, elf_event_int32(number));
	}

	return ELF_OK;
}

struct {
	uint32_t other_loop;
} state_collatz_p2 = { .other_loop = UINT8_MAX};

elf_status_t handler_Collatz_P2(uint32_t self_id, elf_event_t event){
	if(state_collatz_p2.other_loop == UINT8_MAX){
		state_collatz_p2.other_loop = event.value.loop_id;
		return ELF_OK;
	}

	// if event comes here with number, we always multiply by 3, then add 1, then send new num back to collatz1
	int number = event.value.int32;
	assert(number != 0); // check null
	debug && printf("collatz2 received event with num: %d\n", number);
	debug && printf("colltz2 multiplying num by 3 and adding 1...\n");
	// sleep(1);

	int newNum = number * 3 + 1;

	debug && printf("collatz2 sending event to collatz1\n");
	elf_send(state_collatz_p2.other_loop, elf_event_int32(newNum));

	return ELF_OK;
}	

struct {
	uint32_t iteration;
} state_collatz_main = { .iteration = 0 };

elf_status_t handler_collatz(uint32_t self_id, elf_event_t event) {

	// exit on second time this handler is called 
	// first event called by elf_main, second called by collatz_p1 when calculation finished
	if (state_collatz_main.iteration != 0) {
		return ELF_DONE;
	}
	uint32_t collatz_p1_id = 1;
	uint32_t collatz_p2_id = 2;

	elf_init(&collatz_p1_id, handler_Collatz_P1);
	elf_init(&collatz_p2_id, handler_Collatz_P2);

	// send event with loop id of other event loop to each loop to connect them
	elf_send(collatz_p1_id, elf_event_loop_id(collatz_p2_id));
	elf_send(collatz_p2_id, elf_event_loop_id(collatz_p1_id));

	// get input from user
	printf("Enter a positive int: ");
	int input;
	scanf("%d", &input);

	// send desired input to collatz_p1 to begin calculation
	elf_send(collatz_p1_id, elf_event_int32(input));

	state_collatz_main.iteration += 1;

	return ELF_OK;
}

// !!! switch debug to true at the top of the file to see detailed outputs.
int main() {
	// 15 is a good testing number for this (enter when prompted for input)
	elf_main(handler_collatz);
	return 0;
}
