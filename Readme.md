# ReadMe

Before running the code please update the dataset path in the respective program and sample-script

## OPENMP
openmp/kmeans_openmp.c contains the openmp implementation of kmeans algorithm.
openmp/seq/kmeans_seq.c contains the sequential implementation of kmeans algorithm.

## MPI
mpi/pmerge_sort_mpi.c contains the mpi implementation of parallel merge sort.
mpi/seq/quick_sort.c contains the sequential quick sort program.

## CUDA
cuda/reduction_cuda.cu contains the non shared memory implementation of reduction program using cuda.
cuda/shared/reduction_cuda.cu contains the shared memory implementation of reduction program using cuda.
cuda/seq/sum.c contains the sequential implementation of reduction program.