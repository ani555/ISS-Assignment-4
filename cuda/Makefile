CC=nvcc

all: reduction_cuda

reduction_cuda: reduction_cuda.o
	$(CC) -o reduction_cuda reduction_cuda.o

reduction_cuda.o: reduction_cuda.cu
	$(CC) -o reduction_cuda.o -c reduction_cuda.cu

clean:
	rm -f reduction_cuda.o reduction_cuda

