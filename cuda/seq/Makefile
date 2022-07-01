CC=gcc

all: sum

sum: sum.o
	$(CC) -o sum sum.o -lrt

sum.o: sum.c
	$(CC) -o sum.o -c sum.c -lrt

clean:
	rm -f sum.o sum

