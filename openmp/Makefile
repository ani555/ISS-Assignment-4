CC=gcc

all: kmeans_openmp

kmeans_openmp: kmeans_openmp.o
	$(CC) -fopenmp -o kmeans_openmp kmeans_openmp.o -lm

kmeans_openmp.o: kmeans_openmp.c
	$(CC) -fopenmp -o kmeans_openmp.o -c kmeans_openmp.c -lm

clean:
	rm -f kmeans_openmp.o kmeans_openmp
