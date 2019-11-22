/* Solution to the Multiple Producer/Multiple Consumer problem using
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

int      nitems, nproducers, nconsumers;              /* read-only */

struct {                 /* data shared by producers and consumers */
  int       buff[NBUFF];
  int       nput;                     /* item number: 0, 1, 2, ... */
  int       nputval;                  /* value to store in buff[] */
  int       nget;                     /* item number: 0, 1, 2, ... */
  int       ngetval;                  /* value fetched from buff[] */
  sem_t     mutex, nempty, nstored;   /* semaphores, not pointers */
} shared;

void       *producer(void *), *consumer(void *);

/* end globals */

/* main program */
int main(int argc, char **argv)
{
  int             i, prodcount[MAXNTHREADS], conscount[MAXNTHREADS];
  pthread_t       tid_producer[MAXNTHREADS], tid_consumer[MAXNTHREADS];

  if (argc != 4) {
    printf("Usage: %s <#items> <#producers> <#consumers>\n", argv[0]);
    exit(-1);
  }

  nitems = atoi(argv[1]);
  nproducers = min(atoi(argv[2]), MAXNTHREADS);
  nconsumers = min(atoi(argv[3]), MAXNTHREADS);

  /* initialize three semaphores */
  sem_init(&shared.mutex, 0, 1);
  sem_init(&shared.nempty, 0, NBUFF);
  sem_init(&shared.nstored, 0, 0);

  /* create all producers and all consumers */
  for (i = 0; i < nproducers; i++) {
    prodcount[i] = 0;
    pthread_create(&tid_producer[i], NULL, producer, &prodcount[i]);
  }
  for (i = 0; i < nconsumers; i++) {
    conscount[i] = 0;
    pthread_create(&tid_consumer[i], NULL, consumer, &conscount[i]);
  }

  /* wait for all producers and all consumers */
  for (i = 0; i < nproducers; i++) {
    pthread_join(tid_producer[i], NULL);
    printf("producer count[%d] = %d\n", i, prodcount[i]);       
  }
  for (i = 0; i < nconsumers; i++) {
    pthread_join(tid_consumer[i], NULL);
    printf("consumer count[%d] = %d\n", i, conscount[i]);       
  }

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
      sem_post(&shared.nstored);    /* let consumers terminate */
      sem_post(&shared.nempty);
      sem_post(&shared.mutex);
      return(NULL);                 /* all done */
    }

    shared.buff[shared.nput % NBUFF] = shared.nputval;
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

  for ( ; ; ) {
    sem_wait(&shared.nstored);       /* wait for at least 1 stored item */
    sem_wait(&shared.mutex);

    if (shared.nget >= nitems) {
      sem_post(&shared.nstored);
      sem_post(&shared.mutex);
      return(NULL);                  /* all done */
    }

    i = shared.nget % NBUFF;
    if (shared.buff[i] != shared.ngetval)
      printf("error: buff[%d] = %d\n", i, shared.buff[i]);
    shared.nget++;
    shared.ngetval++;

    sem_post(&shared.mutex);
    sem_post(&shared.nempty);        /* 1 more empty slot */
    *((int *) arg) += 1;
  }
}
/* end consumer */
