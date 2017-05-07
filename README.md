# pthread-and-openmp
Classic producer/consumer and monte carlo estimation of pi
--------------------------------
Pthreads project: Producer-Consumer Problems

Generate one producer and two consumers. The producer generates N tasks, where N is a parameter passed in via the command line.

The task queue has two slots.

If the queue is empty, cosumers cannot extract tasks from the queue, while if the queue is full, the producer cannot put task in the queue.

Print out each step: (1) Who inserts task and who extracts task. (2) At the end of execution, print how many tasks each consumer extracts.

----------------------------------
OpenMP project: Implement Monte Carlo Estimation of Pi using OpenMP

Each thread uses a random function to generate its own number of samples (sample_points_per_thread in slide 9). Control the random number generator to produce reasonable number of samples with high accuracy and reasonable execution time for computing Pi.

Use an OpenMP reduction in the end to sum up all the samples from different threads and compute Pi.
----------------------------------
To Compile:

Pthread: gcc -lpthread -o pthread pthread.c
./pthread (#number of tasks)

OpenMP: gcc -fopenmp -o openmp openmp.c
./openmp (#of pthreads)
