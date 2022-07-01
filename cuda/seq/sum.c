#include<stdio.h>
#include<time.h>
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

int main()
{
	int *h_data = get_data((char *)"../reduce_data.txt");
	struct timespec start, end;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	int sum = get_sum(h_data, MAXN);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	printf("Sum=%d\n",sum);
	double run_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
	printf("Time taken = %lf msecs\n",run_time*1000);
}
