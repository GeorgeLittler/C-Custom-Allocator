CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wshadow -pedantic

all: demo

allocator.o: allocator.c allocator.h
	$(CC) $(CFLAGS) -c allocator.c

demo.o: demo.c allocator.h
	$(CC) $(CFLAGS) -c demo.c

demo: allocator.o demo.o
	$(CC) $(CFLAGS) -o demo allocator.o demo.o

clean:
	rm -f *.o demo
