/* Solution to the Multiple Producer/Single Consumer problem using
   semaphores. This example uses a circular buffer to put and get the
   data (a bounded-buffer).
   Source: UNIX Network Programming, Volume 2 by W. Richard Stevens

   To compile: gcc -O -Wall -o <filename> <filename>.c -lpthread 
*/

/* include globals */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>     /* for POSIX threads */
#include <semaphore.h>   /* for POSIX semaphores */

#define  min(a, b)  (((a) < (b)) ? (a) : (b))

#define  NBUFF           10
#define  MAXNTHREADS     100

int      nitems, nproducers;                          /* read-only */

struct {                 /* data shared by producers and consumers */
  int       buff[NBUFF];
  int       nput;                     /* item number: 0, 1, 2, ... */
  int       nputval;                  /* value to store in buff[] */
  sem_t     mutex, nempty, nstored;   /* semaphores, not pointers */
} shared;

void       *producer(void *), *consumer(void *);

/* end globals */

/* main program */
int main(int argc, char **argv)
{
  int             i, prodcount[MAXNTHREADS];
  pthread_t       tid_producer[MAXNTHREADS], tid_consumer;

  if (argc != 3) {
    printf("Usage: %s <#items> <#producers>\n", argv[0]);
    exit(-1);
  }

  nitems = atoi(argv[1]);
  nproducers = min(atoi(argv[2]), MAXNTHREADS);

  /* initialize three semaphores */
  sem_init(&shared.mutex, 0, 1);
  sem_init(&shared.nempty, 0, NBUFF);
  sem_init(&shared.nstored, 0, 0);

  /* create all producers and one consumer */
  for (i = 0; i < nproducers; i++) {
    prodcount[i] = 0;
    pthread_create(&tid_producer[i], NULL, producer, &prodcount[i]);
  }
  pthread_create(&tid_consumer, NULL, consumer, NULL);

  /* wait for all producers and the consumer */
  for (i = 0; i < nproducers; i++) {
    pthread_join(tid_producer[i], NULL);
    printf("producer count[%d] = %d\n", i, prodcount[i]);       
  }
  pthread_join(tid_consumer, NULL);

  sem_destroy(&shared.mutex);
  sem_destroy(&shared.nempty);
  sem_destroy(&shared.nstored);

  return 0;
}
/* end main */

/* producer function */
void *producer(void *arg)
{
  for ( ; ; ) {
    sem_wait(&shared.nempty);       /* wait for at least 1 empty slot */
    sem_wait(&shared.mutex);

    if (shared.nput >= nitems) {
      sem_post(&shared.nempty);
      sem_post(&shared.mutex);
      return(NULL);                 /* all done */
    }

    shared.buff[shared.nput % NBUFF] = shared.nputval;
#ifdef DEBUG
    printf("wrote %d to buffer at location %d\n", 
           shared.nputval, shared.nput % NBUFF);
#endif
    shared.nput++;
    shared.nputval++;

    sem_post(&shared.mutex);
    sem_post(&shared.nstored);      /* 1 more stored item */
    *((int *) arg) += 1;
  }
}
/* end producer */

/* consumer function */
void *consumer(void *arg)
{
  int i;

  for (i = 0; i < nitems; i++) {
    sem_wait(&shared.nstored);       /* wait for at least 1 stored item */
    sem_wait(&shared.mutex);

    if (shared.buff[i % NBUFF] != i)
      printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
#ifdef DEBUG
    printf("read %d from buffer at location %d\n",
           shared.buff[i % NBUFF], i % NBUFF);
#endif

    sem_post(&shared.mutex);
    sem_post(&shared.nempty);        /* 1 more empty slot */
  }

  return (NULL);
}
/* end consumer */
