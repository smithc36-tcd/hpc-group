#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

size_t size = 1;

typedef struct{ double val; char pad[248]; } tvals;

void generate_random(tvals* input, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		input[i].val = rand() / (double)(RAND_MAX);
	}
}

double serial_sum(tvals* x, size_t size)
{
	double ts = omp_get_wtime();
	double sum_val = 0.0;
	double local_sum[omp_get_max_threads()];
	for(int i = 0; i < omp_get_max_threads(); i++){
	        local_sum[i] = 0;
	}

#pragma omp parallel for
	for (int i = 0; i < size; i++) {		
	        local_sum[omp_get_thread_num()] += x[i].val;
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
	tvals* input = (tvals*)malloc(size * sizeof(tvals));
	generate_random(input, size);
	double sum = serial_sum(input, size);
	printf("Sum: %f\n", sum);
	free(input);
	return 0;
}
