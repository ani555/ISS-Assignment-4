CC=mpicc

all: pmerge_sort_mpi

pmerge_sort_mpi: pmerge_sort_mpi.o
	$(CC) -o pmerge_sort_mpi pmerge_sort_mpi.o -lm

pmerge_sort_mpi.o: pmerge_sort_mpi.c
	$(CC) -o pmerge_sort_mpi.o -c pmerge_sort_mpi.c -lm

clean:
	rm -f pmerge_sort_mpi.o pmerge_sort_mpi

