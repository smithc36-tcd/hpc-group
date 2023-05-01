#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi/mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int local_count = 0, provided;
    double x, y, z, pi;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Hint 1 
    int iter_per_process = NUM_ITER / size;

    // Hint 2
    srand(SEED * rank); 


    double start_time = MPI_Wtime();
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < iter_per_process; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));

        // Check if point is in unit circle
        if (z <= 1.0)
        {
            local_count++;
        }
    }

    // Hint 4: Seperate variable for count
    int global_count;
    // Hint 3: Gather the counts
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

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

