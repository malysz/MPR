#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

struct bucket {
	int count;
	double *values;
};

int compare(const void *first, const void *second) {
	double a = *((double*) first), b = *((double*) second);
	if (a < b) {
		return -1;
	} else if (a > b) {
		return 1;
	} else {
		return 0;
	}
}

void bucketSort(double array[], int n, int bucket_count) {
	double start, end;
	double split_time, sort_time, merge_time;

	int i, j, already;
	double left_range, right_range;
	double step = 1.0 / bucket_count;

	struct bucket *buckets = malloc(sizeof(struct bucket)*bucket_count);
	int *indexes = malloc(sizeof(int)*bucket_count);

	#pragma omp parallel for default(shared) private(i)
	for(i = 0; i < bucket_count; i++) {
		buckets[i].count = 0;
		buckets[i].values = malloc(sizeof(double)*n);
	}

	// spliting to buckets
	start = omp_get_wtime();
	#pragma omp parallel for default(shared) private(i, j, left_range, right_range, already) schedule(dynamic)
	for(i = 0; i < n; i++) {
		left_range = 0;
		right_range = step;
		j = 0;
		already = 0;
		while(!already) {
			if((array[i] >= left_range) && (array[i] <= right_range)) {
				already = 1;
				
				#pragma omp critical
				buckets[j].values[buckets[j].count++] = array[i];
			}
			j++;
			left_range += step;
			right_range += step;
		}
	}
	end = omp_get_wtime();
	split_time = end - start;
	
	indexes[0] = 0;
	for(i = 1; i < bucket_count; i++) {
		indexes[i] = indexes[i-1] + buckets[i-1].count;
	}

	// sorting buckets
	start = omp_get_wtime();
	#pragma omp parallel for default(shared) private(i, j)
	for(i = 0; i < bucket_count; i++) {
		qsort(buckets[i].values, buckets[i].count, sizeof(double), &compare);
		for(j = 0; j < buckets[i].count; j++) {
			array[indexes[i] + j] = buckets[i].values[j];
		}
		free(buckets[i].values);
	}
	end = omp_get_wtime();
	merge_time = end - start;
	printf("Split time: %lf sec, Sort & merge time: %lf\n", split_time, merge_time);
	free(buckets);
	free(indexes);
}

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage: %s [points] [max-threads]\n", argv[0]);
		exit(-1);
	}

	int n = atoi(argv[1]);
	int max_threads = atoi(argv[2]);
	int buckets;
	double *array;

	// seed random value generator
	unsigned int randval;
	FILE *f = fopen("/dev/urandom", "r");
	fread(&randval, sizeof(randval), 1, f);
	fclose(f);
	srand(randval);

	double seq_time;
	double speedup;
	
	for(int threads = 1; threads <= max_threads; threads++) {
		buckets = n / 1000 + 1;
		array = malloc(sizeof(double)*n);
		
		omp_set_num_threads(threads);

		int i;
		double fill_time;
		double start, end;		

		start = omp_get_wtime();
		#pragma omp parallel for default(shared) private(i)
		for(i = 0; i < n; i++) {
			array[i] = (rand()+0.0) / INT_MAX;
		}
		end = omp_get_wtime();
		fill_time = end - start;
		printf("\nMeasurements for %d threads:\nFilling array time: %lf sec, ", threads, fill_time);

		start = omp_get_wtime();
		bucketSort(array, n, buckets);
		end = omp_get_wtime();
		time = end - start;

		if(threads == 1) {
			seq_time = time;
		}


		printf("First 100 elements of sorted array: \n");
		for(i = 0; i < 100; i++) {
			printf("%lf ", array[i]);
		}
		printf("...\n");

		free(array);
	}
}
