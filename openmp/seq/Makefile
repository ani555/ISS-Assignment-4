CC=gcc

all: kmeans_seq

kmeans_seq: kmeans_seq.o
	$(CC) -fopenmp -o kmeans_seq kmeans_seq.o -lm

kmeans_seq.o: kmeans_seq.c
	$(CC) -fopenmp -o kmeans_seq.o -c kmeans_seq.c -lm

clean:
	rm -f kmeans_seq.o kmeans_seq
