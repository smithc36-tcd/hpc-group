# Exercise 1 - MPI HelloWorld

**1. Write the code in C.**

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int rank, size, provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  // Get rank ID
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // Get number of MPI processes
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello World from rank %d from %d processes!\n", rank, size);

  MPI_Finalize();
  return 0;
}
```

**2. How do you compile it, which compiler and flags have you used if any?** 

To compile on Dardel we use the following command: `cc HelloWorld.c -o HelloWorld`
No extra compiler flags are needed as `cray-mpich` is loaded by deafult on Dardel.

To compile on a local machine we can use 

```
mpicc HelloWorld.c -o HelloWorld
```

Here we use `mpicc` which is a compiler wrapper for MPI to make compilation easier. 
On my local machine I could equivelantly use 

```
gcc HelloWorld.c -o HelloWorld -lmpi
```

to compile.

**3. How do you run the MPI code on Dardel?**

To run MPI code on Dardel we simply use the command `srun -n 4 ./HelloWorld`, which 
will run 4 processes.

**4. How do you change the number of MPI processes?**

To change the number of processes we change the -n, which determines the number of 
processes that will be launched for the job. 

**5. Which functions do you use for retrieving the rank of an MPI process and the total number of processes?**

The rank of a process is given by the `MPI_Comm_rank(MPI_Comm comm, int* rank)` method.
Where comm is the MPI communicator, often `MPI_COMM_WORLD`, and the rank ID will be assigned 
to the given rank parameter. 

The total number of processes is obtained similarly to above. We use the 
`MPI_Comm_size(MPI_Comm comm, int* size)` command. Where the total number of processes will be returned to the 
given size parameter. 

**6. What are the names of the most used MPI implementations?**

The most commonly use implementations of MPI are:

- MPICH 
- OpenMPI

# Exercise 2 -

# Exercise 3 - 1D Domain Decomposition with Blocking Communication

**1. Assume periodic boundary conditions, e.g. the first and last process will communicate. Implement the communication for the 1D domain decomposition using**

* MPI blocking point-to-point communication. 
We communicate ghost cells by sending and receiving to/from the left and right domain corresponding to the rank.
```
	// communicate ghost cells
	if (rank % 2 == 0) {
		// send to right and receive from left
		MPI_Send(&f[nxn_loc - 3], 1, MPI_DOUBLE, (rank + size + 1) % size, 0, MPI_COMM_WORLD);
		MPI_Recv(&f[0], 1, MPI_DOUBLE, (rank + size - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// send to left and receive from right
		MPI_Send(&f[2], 1, MPI_DOUBLE, (rank + size - 1) % size, 0, MPI_COMM_WORLD);
		MPI_Recv(&f[nxn_loc - 1], 1, MPI_DOUBLE, (rank + size + 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else {
		// receive from left and send to right
		MPI_Recv(&f[0], 1, MPI_DOUBLE, (rank + size - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&f[nxn_loc - 3], 1, MPI_DOUBLE, (rank + size + 1) % size, 0, MPI_COMM_WORLD);
		// receive from right and send to left
		MPI_Recv(&f[nxn_loc - 1], 1, MPI_DOUBLE, (rank + size + 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&f[2], 1, MPI_DOUBLE, (rank + size - 1) % size, 0, MPI_COMM_WORLD);
  }
```
The sending and receiving is split into two parts for safety. Each node only sends or receives at any time, and not both so there is no chance of deadlock. However, this is not really necessary since the buffers are very small. The code works just as well without ping-pong messaging.

* MPI non-blocking point-to-point communication.
Non-blocking communication is similar, with the difference being the use of `Isend` and `Ireceive`:
```
MPI_Request requests[4];
MPI_Isend(&f[2], 1, MPI_DOUBLE, (rank + size - 1) % size, 0, MPI_COMM_WORLD, &requests[0]);
MPI_Isend(&f[nxn_loc - 3], 1, MPI_DOUBLE, (rank + size + 1) % size, 0, MPI_COMM_WORLD, &requests[1]);
MPI_Irecv(&f[0], 1, MPI_DOUBLE, (rank + size - 1) % size, 0, MPI_COMM_WORLD, &requests[2]);
MPI_Irecv(&f[nxn_loc - 1], 1, MPI_DOUBLE, (rank + size + 1) % size, 0, MPI_COMM_WORLD, &requests[3]);
MPI_Waitall(4, requests, MPI_STATUS_IGNORE);
```

**2. Test the results by checking the correct values are on the ghost cells and the derivative of sin(x) on the edges of the domain is correct (the derivative of sin(x) is cos(x)). Show that your code produce the correct results (especially at the ghost cells).**

The correct values of f are shown at the ghost cells:
```
My rank 0 of 8
Here are my values for f including ghost cells
-0.049068
0.000000
0.049068
0.098017
...
0.634393
0.671559
0.707107
0.740951

My rank 1 of 8
Here are my values for f including ghost cells
0.671559
0.707107
0.740951
0.773010
...
0.995185
0.998795
1.000000
0.998795

...

My rank 7 of 8
Here are my values for f including ghost cells
-0.740951
-0.707107
-0.671559
-0.634393
...
-0.098017
-0.049068
0.000000
0.049068
```
Between the processors of rank 0 and 7 we also see the periodic boundary condition is fulfilled.

For the derivative we get the following values
```
My rank 0 of 8
Here are my values for dfdx
0.999598
0.998394
0.994785
...
0.772700
0.740654
0.706823

...

My rank 3 of 8
Here are my values for dfdx
...
-0.994785
-0.998394
-0.999598

My rank 4 of 8
Here are my values for dfdx
-0.999598
-0.998394
-0.994785

...

My rank 7 of 8
Here are my values for dfdx
0.706823
0.740654
0.772700
...
0.994785
0.998394
0.999598
```
At the start and end of the domain, at rank 0 ($x=0$) and rank 7 ($x=2\pi$) respectively the derivative is $~cos(0)=1$. In the middle of the domain, between the end and start nodes of rank 3 and 4 ($x=\pi$), the derivative is $~cos(\pi)=-1$.

**3. Why `MPI_Send` and `MPI_Recv` are called "blocking "communication?**

The `MPI_Send` and `MPI_Recv` functions must wait until the buffer has been emptied or filled respectively before returning (this could be done by a temporary system buffer so the send function doesn't necessarily have to wait for the corresponding receive on the other node). This means no logic can be executed while the functions wait to return, and could be problematic if the buffers are large.

# Exercise 4 - Calculate PI with MPI

## 4.1 Collective reduce 

| Processes |        8 |       16 |       32 |      64 |     128 |      256 |      384 |
|-----------|---------:|---------:|---------:|--------:|--------:|---------:|---------:|
| Seconds   | 3.510213 | 1.788219 | 0.901785 | 0.75959 | 0.42112 | 0.140007 | 0.142021 |


![graph](task4/mpi_reduce_graph.png)

As can be seen in the above graph, the preformance improves with increasing the number of Processes
up to 256 where the preformance plateaus. 
