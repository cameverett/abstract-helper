CC=g++
CFLAGS=-std=c++14 -O2 -Wall -Werror -pedantic

run: build
	./faces.out
	
build:
	$(CC) $(CFLAGS) faces.cpp -o faces.out

clean:
	rm faces.out
