#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define CHUNK_SIZE 100

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

		struct timeval start, end;
                long secs_used, micros;

		if(schedule == 1) {
			int i;

			gettimeofday(&start, NULL);

			#pragma omp parallel for schedule(static, 1) private(i)
			for(i = 0; i < n; i++) {
				array[i] = (rand()+0.0) / INT_MAX;
			}

			gettimeofday(&end, NULL);
		}
		if(schedule == 2) {
			int i;

                        gettimeofday(&start, NULL);

                        #pragma omp parallel for schedule(static, CHUNK_SIZE) private(i)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        gettimeofday(&end, NULL);
                }
		if(schedule == 3) {
			int i;

                        gettimeofday(&start, NULL);

                        #pragma omp parallel for schedule(dynamic, 1) private(i)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        gettimeofday(&end, NULL);
                }
		if(schedule == 4) {
			int i;

                        gettimeofday(&start, NULL);

                        #pragma omp parallel for schedule(dynamic, CHUNK_SIZE) private(i)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        gettimeofday(&end, NULL);
                }
		if(schedule == 5) {
			int i;

                        gettimeofday(&start, NULL);

                        #pragma omp parallel for schedule(guided) private(i)
                        for(i = 0; i < n; i++) {
                                array[i] = (rand()+0.0) / INT_MAX;
                        }

                        gettimeofday(&end, NULL);
                }

		secs_used = (end.tv_sec - start.tv_sec); // avoid overflow by subtracking first
		micros = end.tv_usec - start.tv_usec;
		double time = secs_used + 0.0;
		time += ((micros + 0.0) / 1000000);

		if(threads == 1) {
			seq_time = time;
		}

		speedup = seq_time / time;

		printf("%d,%d,%lf,%lf,%d\n", n, threads, time, speedup, schedule);

		free(array);
	}
}
