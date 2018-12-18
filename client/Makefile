include ../Makefile.header

.PHONY: clean 

OBJS = client.o

server: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o client 

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

clean:
	rm *.o client

