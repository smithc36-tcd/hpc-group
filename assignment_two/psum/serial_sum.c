#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

size_t size = 0;

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

	for (size_t i = 0; i < size; i++) {
		sum_val += x[i];
	}
	printf("Time: %f\n", omp_get_wtime() - ts);
	return sum_val;
}


int main(int argc, char* argv[]) {
	size = (int)atoi(argv[1]);
	double* input = (double*)malloc(size * sizeof(double));
	generate_random(input, size);
	double sum = serial_sum(input, size);
	printf("Sum: %f\n", sum);
	free(input);

	return 0;
}
