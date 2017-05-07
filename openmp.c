/* Command line argument: ./openmp (input:number of threads)*/

/* Program takes user chosen number of threads and uses random function to generate number of samples for each thread.
* Program then randomly generates (x,y) points within (0.5, 0.5) and uses Monte Carlo estimation to find pi.
* OpenMp reduction is used to sum up samples from each thread. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

int compute_pi(int numThreads);

int main(int argc, char *argv[]){
    int numThreads = atoi(argv[1]);
    compute_pi(numThreads);
    return 0;
}


int compute_pi(int numThreads){  
    double x,y,z,count,pi;
    int seed; 
    int i = 0;
    srand(time(NULL));
    long int samples = (int)rand()%9999999+1;                                                                                   
    count = 0;                   
      
      #pragma omp parallel num_threads(numThreads) shared(samples) private(i,x,y,z) reduction(+:count)
{
      #pragma omp for schedule(dynamic)                                                                      
      for ( i=0; i<samples; i++) {                                                                             
        x = (double)rand_r(&seed)/RAND_MAX - 0.5; //generate x,y in (-.5,.5)                                                                         
        y = (double)rand_r(&seed)/RAND_MAX - 0.5;                                                                          
        z = x*x+y*y;                                                                                         
        if (z<=0.25) {count+=1;}  //increment count if within .25                                                                           
      }                                                                 
}                                                                                                                                              
  pi=(double)count/samples*4; //calculate pi                                                                                  
  printf("# of trials= %d, estimate of pi is %lf \n",samples,pi);

  return 0; 
}
