CC=g++
INPUTFILE=rubik.in
CFLAGS=-std=c++14 -O2 -Wall -Werror -pedantic

run: rubik.out
	./rubik.out $(INPUTFILE)
	
rubik.out:
	$(CC) $(CFLAGS) rubik.cpp -o rubik.out

clean:
	rm rubik.out
