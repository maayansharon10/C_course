all:libvector

vector.o: vector.h vector.c
    gcc -c -Wall -Wvla -Wpedantic -Wextra -std=c99 vector.c -o vector.o

libvector: vector.c vector.h
	ar rcs libvector.a vector.o vector.h

ex4.tar: vector.c Makefile
	tar -cvf ex4.tar vector.c Makefile

tar: ex4.tar

clean:
	rm libvector

.PHONY: all tar clean