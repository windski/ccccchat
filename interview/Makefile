CC = gcc
CFLAGS = -Wall -std=c11 -Werror
LDFLAGS = -lm

OBJS = server.o socket.o client.o utils.o

server: server.o socket.o utils.o
	$(CC) server.o socket.o utils.o -o server

client: client.o
	$(CC) $(LDFLAGS) client.o -o client

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

.PHONY: clean

clean:
	@rm $(OBJS) server client


