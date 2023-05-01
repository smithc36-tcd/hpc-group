#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi/mpi.h>
#include <omp.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int count = 0;
    double x, y, z, pi;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    // Get the rank and size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divide work among processes
    int iter_per_process = NUM_ITER / size;

    double start_time = MPI_Wtime();

    // Calculate PI following a Monte Carlo method
    #pragma omp parallel
    {
        unsigned int seed = SEED * rank * omp_get_thread_num();
        int local_count = 0;

        #pragma omp for
        for (int iter = 0; iter < iter_per_process; iter++)
        {
            // Generate random (X,Y) points
            x = (double)rand_r(&seed) / (double)RAND_MAX;
            y = (double)rand_r(&seed) / (double)RAND_MAX;
            z = sqrt((x*x) + (y*y));

            // Check if point is in unit circle
            if (z <= 1.0)
            {
                local_count++;
            }
        }

        #pragma omp atomic
        count += local_count;
    }

    int global_count;
    // Gather the counts from all processes
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Estimate Pi and display the result
    if (rank == 0) {
        pi = ((double)global_count / (double)NUM_ITER) * 4.0;
        double end_time = MPI_Wtime();

        printf("The execution time is %f\n", end_time - start_time);
        printf("The result is %f\n", pi);
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}

