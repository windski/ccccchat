CC = gcc
CFLAGS = -Wall

OBJS = server.o socket.o client.o

server: server.o socket.o
	$(CC) server.o socket.o -o server

client: client.o
	$(CC) client.o -o client

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

.PHONY: clean

clean:
	@rm $(OBJS) server client


