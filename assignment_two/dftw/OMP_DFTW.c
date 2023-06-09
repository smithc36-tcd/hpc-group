// the code calculates a DFT of a random complex number input and
// then an IDFT. The IDFT result should be the input vector
// to compile with gcc
// gcc -Wall -O2 -fopenmp -o DFTW DFTW.c
// written by stef

// exercise

#include "math.h"   // sine and cosine
#include "omp.h"    // openmp library like timing
#include "stdio.h"  // printf
#include "stdlib.h" // malloc and rand for instance. Rand not thread safe!
#include "time.h"   // time(0) to get random seed

// two pi
#define PI2 6.28318530718
// this for the rounding error, increasing N rounding error increases
// 0.01 precision good for N > 8000
#define R_ERROR 0.01

#define N_runs 10

// main routine to calculate DFT
int DFT(int idft, double *xr, double *xi, double *Xr_o, double *Xi_o, int N);

// set the input array with random number
int fillInput(double *xr, double *xi, int N);

// set to zero the input vector
int setOutputZero(double *Xr_o, double *Xi_o, int N);

// check if x = IDFT(DFT(x))
int checkResults(double *xr, double *xi, double *xr_check, double *xi_check,
                 double *Xr_o, double *Xi_r, int N);

// print the results of the DFT
int printResults(double *xr, double *xi, int N);

int main(int argc, char *argv[]) {
  // size of input array
  int N = 10000; // 8,000 is a good number for testing
  printf("DFTW calculation with N = %d \n", N);

  // Allocate array for input vector
  double *xr = (double *)malloc(N * sizeof(double));
  double *xi = (double *)malloc(N * sizeof(double));
  fillInput(xr, xi, N);

  // for checking purposes
  double *xr_check = (double *)malloc(N * sizeof(double));
  double *xi_check = (double *)malloc(N * sizeof(double));
  setOutputZero(xr_check, xi_check, N);

  // Allocate array for output vector
  double *Xr_o = (double *)malloc(N * sizeof(double));
  double *Xi_o = (double *)malloc(N * sizeof(double));
  setOutputZero(Xr_o, Xi_o, N);

  // set time variables
  double start_time, run_time, total_time;
  total_time = 0.0;

  // Calc average time and standard deviation
  for (int i = 0; i < N_runs; i++) {

    // start timer
    start_time = omp_get_wtime();

    // DFT
    int idft = 1;
    DFT(idft, xr, xi, Xr_o, Xi_o, N);
    // IDFT
    idft = -1;
    DFT(idft, Xr_o, Xi_o, xr_check, xi_check, N);

    run_time = omp_get_wtime() - start_time;
    total_time += run_time;

    // check the results: easy to make correctness errors with openMP
    checkResults(xr, xi, xr_check, xi_check, Xr_o, Xi_o, N);

    // reset variables
    fillInput(xr, xi, N);
    setOutputZero(Xr_o, Xi_o, N);
    setOutputZero(xr_check, xi_check, N);
  }

  double mean = total_time / (double)N_runs;
  total_time = 0.0f;

  // Calcualte the std dev
  for (int i = 0; i < N_runs; i++) {

    // start timer
    start_time = omp_get_wtime();

    // DFT
    int idft = 1;
    DFT(idft, xr, xi, Xr_o, Xi_o, N);
    // IDFT
    idft = -1;
    DFT(idft, Xr_o, Xi_o, xr_check, xi_check, N);

    run_time = omp_get_wtime() - start_time;
    total_time += pow(run_time - mean, 2);

    // check the results: easy to make correctness errors with openMP
    checkResults(xr, xi, xr_check, xi_check, Xr_o, Xi_o, N);
    fillInput(xr, xi, N);
    setOutputZero(Xr_o, Xi_o, N);
    setOutputZero(xr_check, xi_check, N);
  }

  double std_dev = sqrt(total_time / (double)N_runs);

  printf("Mean running time across %d runs: %.6f seconds\n", N_runs, mean);
  printf("Standard deviation of running time for %d runs: %.6f seconds\n",
         N_runs, std_dev);

  // take out the garbage
  free(xr);
  free(xi);
  free(Xi_o);
  free(Xr_o);
  free(xr_check);
  free(xi_check);

  return 0;
}

// DFT/IDFT routine
// idft: 1 direct DFT, -1 inverse IDFT (Inverse DFT)
int DFT(int idft, double *xr, double *xi, double *Xr_o, double *Xi_o, int N) {
  int k, n;
#pragma omp for parallel simd reduction(+ : Xr_o[:N], Xi_o[:N]) private(n)
  for (k = 0; k < N; k++) {
    for (n = 0; n < N; n++) {
      double factor = n * k * PI2 / N;
      double cos_v = cos(factor);
      double sin_v = sin(factor);
      // Real part of X[k]
      /*Xr_o[k] += xr[n] * cos_val + idft * xi[n] * sin_val;*/
      Xr_o[k] += xr[n] * cos_v + idft * xi[n] * sin_v;
      // Imaginary part of X[k]
      Xi_o[k] += -idft * xr[n] * sin_v + xi[n] * cos_v;
    }
  }

  // normalize if you are doing IDFT
  if (idft == -1) {
#pragma omp parallel for simd
    for (int n = 0; n < N; n++) {
      Xr_o[n] /= N;
      Xi_o[n] /= N;
    }
  }
  return 1;
}

// set the initial signal
// be careful with this
// rand() is NOT thread safe in case
int fillInput(double *xr, double *xi, int N) {
  srand(time(0));
  for (int n = 0; n < 100000; n++) // get some random number first
    rand();
  for (int n = 0; n < N; n++) {
    // Generate random discrete-time signal x in range (-1,+1)
    // xr[n] = ((double)(2.0 * rand()) / RAND_MAX) - 1.0;
    // xi[n] = ((double)(2.0 * rand()) / RAND_MAX) - 1.0;
    // constant real signal
    xr[n] = 1.0;
    xi[n] = 0.0;
  }
  return 1;
}

// set to zero the output vector
int setOutputZero(double *Xr_o, double *Xi_o, int N) {
  for (int n = 0; n < N; n++) {
    Xr_o[n] = 0.0;
    Xi_o[n] = 0.0;
  }
  return 1;
}

// check if x = IDFT(DFT(x))
int checkResults(double *xr, double *xi, double *xr_check, double *xi_check,
                 double *Xr_o, double *Xi_r, int N) {
  // x[0] and x[1] have typical rounding error problem
  // interesting there might be a theorem on this
  for (int n = 0; n < N; n++) {
    if (fabs(xr[n] - xr_check[n]) > R_ERROR)
      printf("ERROR - x[%d] = %f, inv(X)[%d]=%f \n", n, xr[n], n, xr_check[n]);
    if (fabs(xi[n] - xi_check[n]) > R_ERROR)
      printf("ERROR - x[%d] = %f, inv(X)[%d]=%f \n", n, xi[n], n, xi_check[n]);
  }
  // onty print if there is an error
  /*printf("Xre[0] = %f \n", Xr_o[0]);*/
  return 1;
}

// print the results of the DFT
int printResults(double *xr, double *xi, int N) {
  for (int n = 0; n < N; n++)
    printf("Xre[%d] = %f, Xim[%d] = %f \n", n, xr[n], n, xi[n]);
  return 1;
}
