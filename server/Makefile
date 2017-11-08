cc = gcc
cflag = --std=c99

all: main.c libchating.so
	$(cc) main.c -lchating -L. -o main

libchating.so: translatlib.o utils.o
	$(cc) translatlib.o utils.o --shared -fPIC -o libchating.so

translatlib.o: translatlib.c
	$(cc) -c translatlib.c -o translatlib.o -fPIC

utils.o: utils.c
	$(cc) -c utils.c -o utils.o -fPIC

.PHONY: clean install

clean:
	-@ rm -f *.o

install:
	- cp *.so /usr/lib/

