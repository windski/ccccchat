CC = gcc
CFLAGS = -std=c99 -Wall

OBJS = translatlib.o utils.o main.o rb_tree.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

.c.o:
	$(CC) -c $(FLAGS) $< -o $*.o

.PHONY: clean install

clean:
	-@ rm -f *.o

install:
	- cp *.so /usr/lib/

