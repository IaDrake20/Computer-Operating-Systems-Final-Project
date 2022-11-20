build:
	@gcc main.c -pthread -o main -std=c11 -pedantic

run:
	@./main

clean:
	@rm -f main

# run 'make dev' to run the program
dev: build run clean