build:
	@gcc -o event.o -c event.c
	@gcc -o queue.o -c queue.c
	@gcc -o elf.o -c elf.c
	@gcc -o main main.c event.o queue.o elf.o -pthread -std=c11 -pedantic

run:
	@./main

clean:
	@rm -f main
	@rm -f event.o
	@rm -f queue.o
	@rm -f elf.o

# run 'make dev' to run the program
dev: build run clean
