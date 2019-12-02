/* 
  Simple Pthread Program to find the sum of a vector.
  Uses mutex locks to update the global sum. 
  Author: Purushotham Bangalore
  Date: Jan 25, 2009

  To Compile: gcc -O -Wall pthread_psum.c -lpthread
  To Run: ./a.out 1000 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct 
{
  double sum;
  int N,size;
  long tid;
  double * a;

}thread_data;

thread_data *createThreadData(double * a, double sum, int N, int size, long tid) {
  thread_data *p = malloc(sizeof(thread_data));

  p->a = a;

  p->sum = sum;

  p->N = N;

  p->size = size;

  p->tid = tid;

  return p;

}

sem_t mutex;

double sum=0.0;

void *compute(void *argu) {

    thread_data * arg = (thread_data*)argu;

    arg = (thread_data *)arg;

    double *ret = NULL;

    int myStart, myEnd, myN, i;
    long tid = (long)arg->tid;
    int size = arg->size;

    // determine start and end of computation for the current thread
    myN = arg->N/size;
    myStart = tid*myN;
    myEnd = myStart + myN;
    if (tid == (size-1)) myEnd = arg->N;

    // compute partial sum
    for (i=myStart; i<myEnd; i++) 
       arg->sum += arg->a[i];

    //printf("a=, sum=%lf, N=%d, size=%d, tid=%ld\n", arg->sum, arg->N, arg->size, arg->tid);

    // grab the lock, update global sum, and release lock
    sem_wait(&mutex);
    sum += arg->sum;
    sem_post(&mutex);

    ret = &arg->sum;

    pthread_exit(ret);

    return (NULL);
}

int main(int argc, char **argv) {
    long i;
    pthread_t *tid;

    int    N, size;
    double *a = NULL;
    void *ret;

    if (argc != 3) {
       printf("Usage: %s <# of elements> <# of threads>\n",argv[0]);
       exit(-1);
    }

    N = atoi(argv[1]); // no. of elements
    size = atoi(argv[2]); // no. of threads

    // allocate vector and initialize
    tid = (pthread_t *)malloc(sizeof(pthread_t)*size);
    a = (double *)malloc(sizeof(double)*N); 

    for (i=0; i<N; i++)
      a[i] = (double)(i + 1);

    // create threads
    for ( i = 0; i < size; i++){
      thread_data * item = createThreadData(a, 0, N, size, i);
      pthread_create(&tid[i], NULL, compute, (void *)item);
    }
    // wait for them to complete
    for ( i = 0; i < size; i++)
      pthread_join(tid[i], &ret);
    
    printf("The total is %g, it should be equal to %g\n", 
           ret, ((double)N*(N+1))/2);
    
    return 0;
}

