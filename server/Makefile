include ../Makefile.header

OBJS = server.o utils.o socket.o thread_pool.o

server: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o server

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

.PHONY: clean all

clean:
	rm *.o server

