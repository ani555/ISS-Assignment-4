#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<mpi.h>
#define N 100000


void gen_1L_ints(){
	FILE *f = fopen("sorting_data.csv","w");
	for(int i=0; i<N; i++){
		int n = rand()%1000000 - 9453;
		fprintf(f, "%d\n", n);
	}
}

int* get_data(char* file_name){
	FILE *f = fopen(file_name,"r");
	int *a = (int *)malloc(N*sizeof(int));
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

int* merge(int *merged, int *left, int *right, int sz_l, int sz_r){

	int i=0, j=0, k=0;
	while(i<sz_l && j<sz_r){
		
		if(left[i] <= right[j])
			merged[k++] = left[i++];
		else
			merged[k++] = right[j++];
	}
	while(i<sz_l)
		merged[k++] = left[i++];

	while(j<sz_r)
		merged[k++] = right[j++];
	return merged;
}

int compare(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

int get_root(int rank, int my_lvl){

	return (rank & (~(1<<my_lvl)));
}

int get_partner(int rank, int my_lvl){

	return (rank | (1<<my_lvl));
}
int* parallel_merge_sort(int *a, int *my_a, int my_size, int rank, int max_size, int nprocs){

//	printf("%d process entering parallel merge\n",rank);
	int *merged;
	qsort(my_a, my_size, sizeof(int), compare);
	int my_level = 0;
	int root_id, partner_id;
	int *root_arr;
	root_arr = my_a;
	int partner_size;
	float n = nprocs;
	while(n>1)
	{	

		root_id = get_root(rank, my_level);

		// If rank not equal to root then the process sends its a to root process

		if(rank != root_id){

			MPI_Send(root_arr, my_size, MPI_INT, root_id, 1, MPI_COMM_WORLD);
			break;
		}
		// if root process then accept the array from partner and merge the two arrays to form a new array pointed by the root process
		else{
			
			partner_id =  get_partner(rank, my_level);
			
			if(partner_id >= nprocs)
			{
				my_level++;
				n = n/2;
				continue;
			}
			
			int pmax_size = max_size*(1<<my_level);
			int *partner_arr = (int *)malloc(pmax_size*sizeof(int));
			int partner_size;
			MPI_Status status;
			MPI_Recv(partner_arr, pmax_size, MPI_INT, partner_id, 1, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &partner_size);
			
			int merged_sz = my_size + partner_size;
			merged = (int *)malloc(merged_sz*sizeof(int));
			merged = merge(merged, root_arr, partner_arr, my_size, partner_size);
			
			root_arr = merged;
			my_size = merged_sz;
			my_level++;
			free(partner_arr);

		}
		n = n/2;
	}
	if(rank==0)
		a = root_arr;
	return a;
}

int main(int argc, char* argv[]){

	MPI_Init(&argc, &argv);
	int rank, nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *a, *my_a, *my_size;
	if(rank == 0)
		a = get_data((char*)"merge_data.txt");

	my_size = (int *)malloc(nprocs*sizeof(int));
	// set length of array corresponding to each process
	int v = N/nprocs;
	for(int i=0; i<nprocs; i++)
		my_size[i] = v;

	// if number of elements not perfectly divisible by nprocs evenly distribute the remaining
	int rem = N%nprocs;
	int i=0;
	while(rem--){
		my_size[i]++;
		i++;
	}
	int max_size = (i>0)? my_size[i-1] : my_size[0];
	int *displ = (int *)malloc(nprocs*sizeof(int));
	displ[0] = 0;
	for(i=1; i<nprocs; i++)
		displ[i] = displ[i-1] + my_size[i-1];

	// allocate memory for my_a for each process with size = max elems that are assigned to a proc
	my_a = (int *)malloc(max_size*sizeof(int));
	
	MPI_Scatterv(a, my_size, displ, MPI_INT, my_a, max_size, MPI_INT, 0, MPI_COMM_WORLD);
	double start_time, my_time, exec_time;
	start_time = MPI_Wtime();
	if(rank==0)
	{
		a = parallel_merge_sort(a, my_a, my_size[rank], rank, max_size, nprocs);
	}
	else
		parallel_merge_sort(a, my_a, my_size[rank], rank, max_size, nprocs);
	my_time = MPI_Wtime() - start_time;
	MPI_Reduce(&my_time, &exec_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if(rank==0)
	{
		write_data((char*)"pmerge_result.csv",a);
		free(a);
		printf("Total execution time: %lf msecs\n",1000*exec_time);

	}
	free(my_size);
	free(displ);
	free(my_a);
	MPI_Finalize();
}
