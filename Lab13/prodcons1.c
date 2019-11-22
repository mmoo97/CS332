/* Solution to the single Producer/Consumer problem using semaphores. 
   This example uses a circular buffer to put and get the data 
   (a bounded-buffer).
   Source: UNIX Network Programming, Volume 2 by W. Richard Stevens

   To compile: gcc -O -Wall -o <filename> <filename>.c -lpthread 
   To run: ./<filename> <#items>

   To enable printing add -DDEBUG to compile:
   gcc -O -Wall -DDEBUG -o <filename> <filename>.c -lpthread 
*/

/* include globals */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>     /* for POSIX threads */
#include <semaphore.h>   /* for POSIX semaphores */

#define  NBUFF           10

int      nitems;                                      /* read-only */

struct {                   /* data shared by producer and consumer */
  int       buff[NBUFF];
  sem_t     mutex, nempty, nstored;   /* semaphores, not pointers */
} shared;

void       *producer(void *), *consumer(void *);

/* end globals */

/* main program */
int main(int argc, char **argv)
{
  pthread_t       tid_producer, tid_consumer;

  if (argc != 2) {
    printf("Usage: %s <#items>\n", argv[0]);
    exit(-1);
  }

  nitems = atoi(argv[1]);

  /* initialize three semaphores */
  sem_init(&shared.mutex, 0, 1);
  sem_init(&shared.nempty, 0, NBUFF);
  sem_init(&shared.nstored, 0, 0);

  /* create one producer thread and one consumer thread */
  pthread_create(&tid_producer, NULL, producer, NULL);
  pthread_create(&tid_consumer, NULL, consumer, NULL);

  /* wait for producer and consumer threads */
  pthread_join(tid_producer, NULL);
  pthread_join(tid_consumer, NULL);

  /* remove the semaphores */
  sem_destroy(&shared.mutex);
  sem_destroy(&shared.nempty);
  sem_destroy(&shared.nstored);

  return 0;
}
/* end main */

/* producer function */
void *producer(void *arg)
{
  int i;

  for (i = 0; i < nitems; i++) {
    sem_wait(&shared.nempty);       /* wait for at least 1 empty slot */
    sem_wait(&shared.mutex);

    shared.buff[i % NBUFF] = i;     /* store i into circular buffer */
#ifdef DEBUG
    printf("wrote %d to buffer at location %d\n", i, i % NBUFF);
#endif

    sem_post(&shared.mutex);
    sem_post(&shared.nstored);      /* 1 more stored item */
  }
  return (NULL);
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
