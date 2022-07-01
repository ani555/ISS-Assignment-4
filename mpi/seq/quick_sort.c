#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 100000

int* get_data(char* file_name){
	FILE *f = fopen(file_name,"r");
	int *a = malloc(N*sizeof(int));
	for(int i=0; i<N; i++){
		fscanf(f,"%d",&a[i]);
	}
	return a;
}

void write_data(char* file_name, int* a){

	FILE *f = fopen(file_name, "w");
	for(int i=0; i<N; i++){
		fprintf(f, "%d\n", a[i]);
	}
}

void write_to_console(int* a){

	for(int i=0; i<N; i++){
		printf("%d\n", a[i]);
	}
}

int compare(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

int main(){
	int *a; 
	a = get_data("../merge_data.txt");

	clock_t start, end;
	start = clock();
	qsort(a, N, sizeof(int), compare);
	end = clock();
	double exec_time = ((double)(end-start)*1000)/CLOCKS_PER_SEC;
	write_data("qsort_result.csv",a);
	printf("Execution time: %lf\n",exec_time);
}
