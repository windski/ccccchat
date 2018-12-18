include Makefile.header

client: client/client
	make -C client

server: server/server
	make -C server


.PHONY: clean all


clean:
	- make clean -C client
	- make clean -C server

