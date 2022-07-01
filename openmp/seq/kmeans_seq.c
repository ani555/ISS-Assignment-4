#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<omp.h>
#define NLINES 500000
#define NCLUSTERS 20
#define MAX_ITERS 200
#define eps 1e-8

struct point
{
	double x, y;
};

struct cluster
{
	struct point centroid;
	int num_pts;
};

struct point* get_data(char *file_name){

	struct point* points = (struct point*) malloc(NLINES*sizeof(struct point));
	FILE *f = fopen(file_name, "r");
	for(int i=0; i<NLINES; i++){
		fscanf(f,"%lf,%lf",&points[i].x, &points[i].y);
	}
	return points;
}

void write_data(char *file_name, struct cluster* clusters){

	FILE *f = fopen(file_name, "w");
	fprintf(f, "CLUSTER_ID,NUM_PTS,CENTROID_X,CENTROID_Y\n");
	for(int i=0; i<NCLUSTERS; i++)
		fprintf(f,"%d,%d,%lf,%lf\n",i,clusters[i].num_pts,clusters[i].centroid.x,clusters[i].centroid.y);
}

void print_data(struct cluster* clusters){

	printf("CLUSTER_ID,NUM_PTS,CENTROID_X,CENTROID_Y\n");
	for(int i=0; i<NCLUSTERS; i++)
		printf("%d,%d,%lf,%lf\n",i,clusters[i].num_pts,clusters[i].centroid.x,clusters[i].centroid.y);
}

double get_dist(double x1, double y1, double x2, double y2){
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

bool converged(struct point* prev_centroids, struct cluster* clusters, int n_clusters)
{

	double sum = 0.0;
	for(int i=0; i<n_clusters; i++){
		sum += get_dist(prev_centroids[i].x, prev_centroids[i].y, clusters[i].centroid.x, clusters[i].centroid.y);
	}	
	if(sum<eps) return true;

	return false;
}

struct cluster* k_means(struct point* points, int n_points, int n_clusters){
	
	struct cluster* clusters = (struct cluster*) malloc(n_clusters*sizeof(struct cluster));
	struct point* prev_centroids = (struct point*) malloc(n_clusters*sizeof(struct point));
	int* cluster_index = (int *) malloc(n_points*sizeof(int));

	int iters = 0;
	int n_pts_blck = n_points/n_clusters;
	for(int i=0; i<n_clusters; i++)
	{
		int idx = n_pts_blck*i + rand()%n_pts_blck;
		clusters[i].centroid.x = points[idx].x;
		clusters[i].centroid.y = points[idx].y;
	}
	int i, j, k;
	for(i=0; i<MAX_ITERS; i++)
	{
		iters++;
		// cluster assignment step

		for(k=0; k<n_points; k++)
		{
			double min_dist = 1e9;
			int cluster_id;
			for(j=0; j<n_clusters; j++){
				double dist = get_dist(points[k].x, points[k].y, clusters[j].centroid.x, clusters[j].centroid.y);
				if(dist < min_dist)
				{
					cluster_id = j;
					min_dist = dist;
				}
			}
			cluster_index[k] = cluster_id;
		}
		
		// Move centroid step
		// initialize the old ones to zeros
		for(i=0; i<n_clusters; i++)
		{
			prev_centroids[i].x = clusters[i].centroid.x;
			prev_centroids[i].y = clusters[i].centroid.y;
			clusters[i].centroid.x = 0.0;
			clusters[i].centroid.y = 0.0;
			clusters[i].num_pts = 0;
		}
		// update new centroid with the average of cluster member points
		for(i=0; i < n_points; i++){
			clusters[cluster_index[i]].centroid.x += points[i].x;
			clusters[cluster_index[i]].centroid.y += points[i].y;
			clusters[cluster_index[i]].num_pts += 1;
		}

		for(i=0; i<n_clusters; i++){
			clusters[i].centroid.x /= clusters[i].num_pts;
			clusters[i].centroid.y /= clusters[i].num_pts;
		}

		if(converged(prev_centroids, clusters, n_clusters))
			break;
	}
	printf("# iterations : %d\n", iters);
	return clusters;

}
int main(int n, char *argv[]){

	struct point* points = get_data("../data_500k.csv");
	double start = omp_get_wtime();
	struct cluster* result = k_means(points, NLINES, NCLUSTERS);
	double end = omp_get_wtime();
	write_data("result.csv",result);
	//print_data(result);
	printf("Execution time: %g\n",(end-start)*1000);
}
