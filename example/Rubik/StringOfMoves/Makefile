CC=g++
CFLAGS=-std=c++14 -O2 -Wall -Werror -pedantic

run: build
	./string.out
	
build: ../../../abstract_algebra.hpp
	$(CC) $(CFLAGS) stringOfMoves.cpp -o string.out

clean:
	rm string.out
