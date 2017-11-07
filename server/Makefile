cc = gcc
cflag = --std=c99

all: main.c libchating.so
	$(cc) main.c -lchating -L. -o main

libchating.so: translatlib.o
	$(cc) translatlib.o --shared -fPIC -o libchating.so

translatlib.o: translatlib.h translatlib.c
	$(cc) -c translatlib.c -o translatlib.o --shared -fPIC

.PHONY: clean install

clean:
	-@ rm -f *.o

install:
	- cp *.so /usr/lib/

