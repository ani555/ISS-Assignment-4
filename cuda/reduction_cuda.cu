#include<stdio.h>
#include<stdlib.h>
#define MAXN 20000

int* get_data(char* file_name){
	FILE *f = fopen(file_name,"r");
	int *a = (int *)malloc(MAXN*sizeof(int));
	for(int i=0; i<MAXN; i++){
		fscanf(f,"%d",&a[i]);
	}
	return a;
}

int get_sum(int *a, int n){

	int sum=0;
	for(int i=0; i<n; i++)
		sum+=a[i];

	return sum;
}


__global__ void reduce(int *g_idata, int *res, int num_elems){

	int tid = threadIdx.x;
	
	int n = num_elems/blockDim.x;


	// after this step we have reduced 100000 to 1000 elems and we have 1K total threads
	res[tid]=0;
	for(int i=0; i<n; i++)
		res[tid] += g_idata[tid*n+i];
	__syncthreads();


	// start with N/2 threads and keep movivng up the tree
	// finally result is obtained in res[0];
	for(int step=1; step<blockDim.x; step<<=1)
	{
		int idx = 2*tid*step;

		if(idx+step < blockDim.x){

			res[idx] += res[idx+step];
		}
		__syncthreads();
	}

}


int main(){
	int *h_data, *d_data, *res;
	int result;
	int num_elems = MAXN;
	int size = num_elems*sizeof(int);
	h_data = get_data((char *)"reduce_data.txt");
	int threads_per_block = 1000;
	int num_blocks = 1;
	dim3 dimBlock(threads_per_block);
	dim3 dimGrid(num_blocks);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaMalloc(&d_data, size);
	cudaMalloc(&res, threads_per_block*sizeof(int));
	cudaMemcpy(d_data, h_data, size, cudaMemcpyHostToDevice);

	cudaEventRecord(start);
	reduce<<<dimGrid, dimBlock>>>(d_data, res, num_elems);
	cudaEventRecord(stop);
	cudaMemcpy(&result, res, sizeof(int), cudaMemcpyDeviceToHost);
	cudaEventSynchronize(stop);
	float exec_time = 0;
	cudaEventElapsedTime(&exec_time, start, stop);
	
	printf("Cuda Reduction result: %d\n",result);
	int sum = get_sum(h_data, num_elems);
	printf("Actual answer: %d\n",sum);
	printf("Kernel execution time in msec: %f\n", exec_time);
	cudaFree(d_data);
	free(h_data);
}
