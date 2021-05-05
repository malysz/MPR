#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define CHUNK_SIZE 500

int main(int argc, char **argv) {
	if(argc != 4) {
		printf("Usage: %s <points> <max threads> <schedule type>\n", argv[0]);
		printf("Available schedule types:\n1 - [static, 1]\n2 - [static, chunkSize]\n3 - [dynamic, 1]\n4 - [dynamic, chunkSize]\n5 - [guided]\n");
		exit (-1);
	}

	int n = atoi(argv[1]);
	int max_threads = atoi(argv[2]);
	int schedule = atoi(argv[3]);
	double *array;

	unsigned int randval;
	FILE* f = fopen("/dev/urandom", "r");
	fread(&randval, sizeof(randval), 1, f);
	fclose(f);
	srand(randval);

	double seq_time, speedup;

	for(int threads = 1; threads <= max_threads; threads++) {
		array = malloc(sizeof(double)*n);
		omp_set_num_threads(threads);

		double start, end;

		if(schedule == 1) {
			int i;

			start = omp_get_wtime();
			
			#pragma omp parallel for default(shared) private(i) schedule(static, 1)
			for(i = 0; i < n; i++) {
				array[i] = (rand()+0.0) / INT_MAX;
			}

			end = omp_get_wtime();
		}
		if(schedule == 2) {
			int i;

                        start = omp_get_wtime();

                        #pragma omp parallel for default(shared) private(i) schedule(static, CHUNK_SIZE)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        end = omp_get_wtime();
                }
		if(schedule == 3) {
			int i;

                        start = omp_get_wtime();

                        #pragma omp parallel for schedule(dynamic, 1) private(i)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        end = omp_get_wtime();
                }
		if(schedule == 4) {
			int i;

                        start = omp_get_wtime();

                        #pragma omp parallel for default(shared) private(i) schedule(dynamic, CHUNK_SIZE)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        end = omp_get_wtime();
                }
		if(schedule == 5) {
			int i;

                        start = omp_get_wtime();

                        #pragma omp parallel for default(shared) private(i) schedule(guided)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        end = omp_get_wtime();
                }

		double time = end - start;
		
		if(threads == 1) {
			seq_time = time;
		}

		speedup = seq_time / time;

		printf("%d,%d,%lf,%lf,%d\n", n, threads, time, speedup, schedule);

		free(array);
	}
}
