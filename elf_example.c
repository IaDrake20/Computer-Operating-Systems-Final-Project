#include "elf_public.h"
#include <stdio.h>
#include <unistd.h>

struct {
	uint32_t other_loop;
	int32_t count;
} state_ping = { .other_loop = UINT8_MAX, .count = 0 };

struct {
	int32_t number;
	uint32_t other_loop;	
} state_collatz_p1 = { .number = 0, .other_loop = UINT8_MAX}; 

elf_status_t handler_ping(uint32_t self_id, elf_event_t event) {

	if (state_ping.other_loop == UINT8_MAX) {           // will happen on very first event
		state_ping.other_loop = event.value.loop_id;    // give self reference to the other loop
		state_ping.count = 17;                          // init count to 17
		printf("ping initialized\n");                   // *note doesn't return, it then begins hitting the ball towards pong
	} else if (state_ping.count == 0) {
		printf("ping finished\n");
		// TODO: send quit event to main?
		return ELF_OK; // drop ball
	}

	state_ping.count -= 1;
	printf("  ping with count: %d\n", state_ping.count);
	// printf("  ping\n");
	sleep(1);
	elf_send(state_ping.other_loop, elf_event_token());
	// printf("elf send to other loop: %d\n", state_ping.other_loop);

	return ELF_OK;
}


struct {
	uint32_t other_loop;
} state_pong = { .other_loop = UINT8_MAX };

elf_status_t handler_pong(uint32_t self_id, elf_event_t event) {
	if (state_pong.other_loop == UINT8_MAX) {
		state_pong.other_loop = event.value.loop_id;    // similar to ping
		printf("pong initialized\n");
		return ELF_OK;                                  // *note DOES return, waits for first real message to begin playing 
	}

	printf("  pong\n");
	// printf("count is: %d\n", state_ping.count);
	sleep(1);
	elf_send(state_pong.other_loop, elf_event_token());

	return ELF_OK;
}

//Our example based on collatz conjecture... It takes in an int, if even divides by 2, otherwise multoply by 3 and add 1
elf_status_t handler_Collatz_P1(uint32_t selfID, elf_event_t event, int passed_num, int iterations){
	
	if(state_collatz_p1.other_loop == UINT8_MAX){
		state_collatz_p1.other_loop = event.value.loop_id;

	} else if(state_collatz_p1.number == 1) {
		printf("Finished calculating");
		return ELF_OK;
	}
	
	printf("Please enter a non-negative  whole number");
	scanf("%d", &state_collatz_p1.number);
	printf("\n");//in future just put in front of next print statement
	if(state_collatz_p1.number % 2 != 0){
		state_collatz_p1.number /= 2;
	}

	elf_send(state_collatz_p2.other_loop, elf_event_token());

	if(passed_num > 0 || passed_num != 0){
		printf("%d", state_collatz_p1.number);
	}

	return ELF_OKAY;
}

struct {
	uint32_t other_loop;
} state_collatz_p2 = { .other_loop = UINT8_MAX};

elf_status_t handler_Collatz_P2(uint32_t selfID, elf_event_t event, int iterations){
	state_collatz.number *= 3;
	state_collatz.number += 1;
	elf_send(state_collatz_p1.other_loop, elf_token_event());
}	


elf_status_t handler_main(uint32_t self_id, elf_event_t event) {
	// assume, we are triggered at least once
	printf("handler_main(): triggered\n");

	uint32_t ping_id = 1;  // was UINT32_MAX
	uint32_t pong_id = 2;  // was UINT32_MAX

	elf_init(&ping_id, handler_ping);
	elf_init(&pong_id, handler_pong);

	elf_send(pong_id, elf_event_loop_id(ping_id));
	elf_send(ping_id, elf_event_loop_id(pong_id));

	return ELF_OK; // TODO: change to ELF_OK (was ELF_ERROR)
}


int main() {
	elf_main(handler_main); // assume, this blocks main thread
	return 0;
}
