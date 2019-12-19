ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: main.o
	$(CC) -o <program> main.o

main.o: main.c
	$(CC) -c main.c

run: all
	./<program>

clean:
	touch main.o
	touch a.out
	rm *.o
	rm a.out
