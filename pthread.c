/* Program generates one producer and two consumers. 
*  The producer generates N tasks, where N is a parameter passed in via the command line.
*  The task queue has two slots.
*  If the queue is empty, cosumers cannot extract tasks from the queue, while if the queue is full, the producer cannot put task in the queue.
*  Who inserts task and who extracts taskis printed out as well as how many tasks each consumer extracts at the end of execution*/

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
#include <sys/time.h>
# define BufferSize 2

/* initialize functions and variables*/
void *Producer();
void *Consumer1();
void *Consumer2();

int BufferIndex=0;
char *BUFFER;

int globalvariable; //make command line arg global
int notdone = 1;
int i;
int num_cons1;
int num_cons2;
int count_cons;

pthread_cond_t Buffer_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[1])
{   
    int N = atoi(argv[1]); //get command line number
    globalvariable = N; //make variable global
    pthread_t ptid,ctid1,ctid2;
    
    BUFFER=(char *) malloc(sizeof(char) * BufferSize);            
    
    /*create and join threads*/
    pthread_create(&ptid,NULL,Producer,NULL);
    pthread_create(&ctid1,NULL,Consumer1,NULL);
    pthread_create(&ctid2,NULL,Consumer2,NULL);
    
    pthread_join(ptid,NULL);
    pthread_join(ctid1,NULL);
    pthread_join(ctid2,NULL);

    /* print how many tasks each consumer extracts at the end */
    printf("Consumed by Comsumer 1: %d\n", num_cons1);
    printf("Consumed by Comsumer 2: %d\n", num_cons2);

    return 0;
}

void *Producer()
{    
    /*number of tasks = globalvariable*/
    for (i = 1; i <=globalvariable; i++)
    {
        pthread_mutex_lock(&mVar);
        if(BufferIndex==BufferSize)
        {    
            pthread_cond_wait(&Buffer_Not_Full,&mVar);
        }                        
        BUFFER[BufferIndex++]='@';
        printf("Produce : %d \n",BufferIndex); 
        pthread_mutex_unlock(&mVar);
        pthread_cond_signal(&Buffer_Not_Empty);
        
    }  
   
    
}

/*first consumer*/
void *Consumer1()
{
    while(notdone)
    {
        pthread_mutex_lock(&mVar); //lock
        while(BufferIndex<1) //if available for consumption
        {
            pthread_cond_wait(&Buffer_Not_Empty,&mVar);
        }                
        printf("Consume1 : %d \n",BufferIndex--); 
        count_cons++; //count how many have been consumed
        num_cons1++;
        pthread_mutex_unlock(&mVar); //unlock        
        pthread_cond_signal(&Buffer_Not_Full); 

	/*break out of loop when there is no more to consume*/
        if (count_cons >= globalvariable)
        {
        notdone = 0;
        }
    }
    /*print statement after tasks are done, only consumer1 or consumer2 will print, not both*/
    printf("Consumed by Consumer 1: %d\nConsumed by Consumer 2: %d\n", num_cons1, num_cons2);	
    exit(0); //cleanly exit
}

/*second consumer*/
void *Consumer2()
{
    while(notdone)
    {
        pthread_mutex_lock(&mVar);
        while(BufferIndex<1)
        {            
            pthread_cond_wait(&Buffer_Not_Empty,&mVar);
        }                
        printf("Consume2 : %d \n",BufferIndex--);
        count_cons++;
        num_cons2++;
        pthread_mutex_unlock(&mVar);        
        pthread_cond_signal(&Buffer_Not_Full); 
        if (count_cons >= globalvariable)
        {
	notdone = 0;
        }
    }
    printf("Consumed by Consumer 1: %d\nConsumed by Consumer 2: %d\n", num_cons1, num_cons2);	 
    exit(0);
}