CC = gcc
OBJS = elementary.o
LFLAGS=-lSDL2

elementary: elementary.o
	$(CC) $^ $(LFLAGS) -o $@

clean:
	rm -f elementary $(OBJS)
