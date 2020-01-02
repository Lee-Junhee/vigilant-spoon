ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control.o write.o info.h 
	$(CC) -o control control.o 
	$(CC) -o write write.o

control.o: control.c info.h 
	$(CC) -c control.c 

write.o: write.c info.h
	$(CC) -c write.c 

run: all
	./control

clean:
	touch main.o
	touch a.out
	rm *.o
	rm a.out
