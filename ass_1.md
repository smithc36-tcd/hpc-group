# Assignment 1

## Exercise 1: Compiling and running on Dardel. 

### Task 1.1
Describe all steps to connect to Dardel, complile the code, and execute the 
code on computing nodes with SLURM (Interactive and batch).

### Task 1.2

How many computing nodes does Dardel have? 
- 554 Nodes [488 SNIC thin, 20 SNIC large, 8 SNIC Huge, 2 SNIC Giant, 36 KTH]

How many CPUs, CPU core, and memory does each computing node have? 
- 2 CPUs, 128 Physical cores with 256 virtual cores. Memory size varies with the type of node.

What is the total number of cores and memory of the Dardel computer?
- 70912 Total cores in the CPU partition. With 156.672TB of memory.  

What is the fastest supercomputer in the world? What are the fastest European 
and Asian supercomputers? What is their power usage?
- Frontier in the world [21,1kW]. Asian - Supercomputer Fugaku [29,899kW]. European - LUMI [6,016kW].  

## Exercise 2: Sustainability and supercomputers

### Task 2.1
Using the calculator at http://calculator.green-algorithms.org/. Evaluate the 
power usage and carbon footprint of running a simulation on 10 Dardel computing 
nodes, assuming that we only use CPU, all the memory on the nodes, and we 
neglect the network costs. The simulation runs on 10 computing nodes for 
12 hours.

- 12 Hours, CPU, 128 Cores, 256gb * 10 =  20.09kg CO2, 180.71kWh

## Exercise 3: Modeling Sparce Matrix-Vector Multiply. 

### Task 3.1
What is the performance in total execution time - do not consider data 
movement - according to your performance model on Dardel or your local 
computer for different sparse matrices = $10^2$, $10^4$, $10^6$, and 10^8?

Laptop - i7-10750H (Base 2.6Ghz - 5.0GHz)

Taking the base rate gives $1 / 2.6 \cdot 10^9$ operations/s

for each nnz:
    $$nnz = 49600 \implies 0.00001907s\\
    nnz = 4996000 \implies 0.019215s\\
    nnz = 499960000 \implies 0.192292s$$



Time for Sparse Ax, nrows=10000, nnz=49600, T = 0.000262s

Time for Sparse Ax, nrows=1000000, nnz=4996000, T = 0.028904s

Time for Sparse Ax, nrows=100000000, nnz=499960000, T = 0.598384s

What is the measured performance in total execution time and floating-point 
operations per second running spmv.c for different sizes = 102, 104, 106, 
and 108? Compare the results from the performance model and experimental 
results. Discuss the comparison in the report.


## Exercise 4: The memory mountain

### Task 4.1 
**Report the name of the processor and the size of the L1, L2, and L3 of the processor you are benchmarking. You can check the specs of your processor online.**

Processor: i7-10750H
L1 = 384kB
L2 = 1.5MB
L3 = 12MB

### Task 4.2

![Memory Mountain](./memory_mountain.png)

### Task 4.3
**What region (array size, stride) gets the most consistently high performance (ignoring spikes in the graph that are noisy results...)? What is the read bandwidth reported?**

Small arrays with low stride is the consistently most high performing region. The read bandwidth is up to $1.2 \cdot 10^5$ MB/s.


### Task 4.4
**What region (array size, stride) gets the most consistently low performance (Ignoring spikes in the graph that are noisy results...)? What is the read bandwidth reported?**

High stride and large array size is the consistently most low performing region. The read bandwidth is

### Task 4.5
**When you look at the graph for stride=1, you (should) see relatively high 
performance compared to stride=32. This is true even for large array sizes 
that are much larger than the L3 cache size. How is this possible, when the 
array cannot possibly all fit into the cache? Your explanation should include 
a brief overview of hardware prefetching as it applies to caches.**


## Exercise 5: Write a benchmark to measure preformance
### Task 5.1.1
What is the average runtime? 
- 0.00000405s

### Task 5.1.2
Increase N and compile the code, what is the average running time now? 
- 0.00000405s

### Task 5.2.1
why is the execution time like that in the previous question when the flag -O2 
is used? Answer this question using the information you find in the assembly code.

- The complier optimised teh loop out of the code. 

### Task 5.2.2
What is the average execution time without the `-O2` flag

For N = 5000:
- 0.00004816s 

For N = 50000: 
- 0.00050402s

### Task 5.3
What is the clock granularity on Dardel or your local computer?

```
# min dist = 9.54e-07, max dist = 4.05e-06, total time = 1.31e-05
1	4.05e-06
2	0.00e+00
3	0.00e+00
4	0.00e+00
5	0.00e+00
6	0.00e+00
7	0.00e+00
8	0.00e+00
9	9.54e-07
10	0.00e+00
11	0.00e+00
12	0.00e+00
13	0.00e+00
14	0.00e+00
15	0.00e+00
16	0.00e+00
17	0.00e+00
18	0.00e+00
19	0.00e+00
20	1.19e-06
21	0.00e+00
22	0.00e+00
23	0.00e+00
24	0.00e+00
25	0.00e+00
26	0.00e+00
27	0.00e+00
28	0.00e+00
29	0.00e+00
30	0.00e+00
31	9.54e-07
32	0.00e+00
33	0.00e+00
34	0.00e+00
35	0.00e+00
36	0.00e+00
37	0.00e+00
38	0.00e+00
39	0.00e+00
40	0.00e+00
41	0.00e+00
42	9.54e-07
43	0.00e+00
44	0.00e+00
45	0.00e+00
46	0.00e+00
47	0.00e+00
48	0.00e+00
49	0.00e+00
50	0.00e+00
51	0.00e+00
52	9.54e-07
53	0.00e+00
54	0.00e+00
55	0.00e+00
56	0.00e+00
57	0.00e+00
58	0.00e+00
59	0.00e+00
60	0.00e+00
61	0.00e+00
62	0.00e+00
63	9.54e-07
64	0.00e+00
65	0.00e+00
66	0.00e+00
67	0.00e+00
68	0.00e+00
69	0.00e+00
70	0.00e+00
71	0.00e+00
72	0.00e+00
73	0.00e+00
74	1.19e-06
75	0.00e+00
76	0.00e+00
77	0.00e+00
78	0.00e+00
79	0.00e+00
80	0.00e+00
81	0.00e+00
82	0.00e+00
83	0.00e+00
84	0.00e+00
85	9.54e-07
86	0.00e+00
87	0.00e+00
88	0.00e+00
89	0.00e+00
90	0.00e+00
91	0.00e+00
92	0.00e+00
93	0.00e+00
94	0.00e+00
95	0.00e+00
96	9.54e-07
97	0.00e+00
98	0.00e+00
99	0.00e+00
```


[Runtime on local machine]



