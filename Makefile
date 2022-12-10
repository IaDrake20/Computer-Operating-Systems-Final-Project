build:
	@gcc -o event.o -c event.c
	@gcc -o gqueue.o -c gqueue.c
	@gcc -o elf.o -c elf.c
        @gcc -o main main.c event.o gqueue.o elf.o -pthread -std=c11 -pedantic -lpthread -lrt

run:
	@./main

clean:
	@rm -f main
	@rm -f event.o
	@rm -f gqueue.o
	@rm -f elf.o

# run 'make dev' to run the program
dev: build run clean
