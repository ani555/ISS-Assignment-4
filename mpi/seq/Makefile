CC=mpicc

all: quick_sort

quick_sort: quick_sort.o
	$(CC) -o quick_sort quick_sort.o -lm

pmerge_sort_mpi.o: quick_sort.c
	$(CC) -o quick_sort.o -c quick_sort.c -lm

clean:
	rm -f quick_sort.o quick_sort

