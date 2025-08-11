CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wshadow -pedantic
DBGFLAGS = -DALLOCATOR_DEBUG

all: demo bench

allocator.o: allocator.c allocator.h
	$(CC) $(CFLAGS) -c allocator.c

allocator_dbg.o: allocator.c allocator.h
	$(CC) $(CFLAGS) $(DBGFLAGS) -c allocator.c -o allocator_dbg.o

demo.o: demo.c allocator.h
	$(CC) $(CFLAGS) -c demo.c

bench.o: bench.c allocator.h
	$(CC) $(CFLAGS) -c bench.c

demo: allocator_dbg.o demo.o
	$(CC) $(CFLAGS) -o demo allocator_dbg.o demo.o

bench: allocator.o bench.o
	$(CC) $(CFLAGS) -o bench allocator.o bench.o

clean:
	rm -f *.o demo bench
