include ../Makefile.header

# OBJS = server.o utils.o socket.o thread_pool.o
SRC := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRC))

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

server: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o server

.PHONY: clean all

clean:
	rm *.o server

