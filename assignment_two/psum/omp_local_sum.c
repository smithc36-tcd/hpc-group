#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

size_t size = 1;

void generate_random(double* input, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		input[i] = rand() / (double)(RAND_MAX);
	}
}

double serial_sum(double* x, size_t size)
{
	double ts = omp_get_wtime();
	double sum_val = 0.0;
	double local_sum[omp_get_max_threads()];
	for(int i = 0; i < omp_get_max_threads(); i++){
	        local_sum[i] = 0;
	}

#pragma omp parallel for
	for (int i = 0; i < size; i++) {		
	        local_sum[omp_get_thread_num()] += x[i];
	}

	for(int i = 0; i < omp_get_max_threads(); i++){
	        sum_val += local_sum[i];
	}
	printf("Time: %f\n", omp_get_wtime() - ts);
	return sum_val;
}


int main(int argc, char* argv[]) {
	printf("Max threads: %d\n", omp_get_max_threads());
	size = (int)atoi(argv[1]);	
	double* input = (double*)malloc(size * sizeof(double));
	generate_random(input, size);
	double sum = serial_sum(input, size);
	printf("Sum: %f\n", sum);
	free(input);
	return 0;
}
