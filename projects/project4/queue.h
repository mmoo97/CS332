/* Header file for the simple circular queue example */
#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct _queue {
	int size;    /* maximum size of the queue */
	int *buffer; /* queue buffer */
	int start;   /* index to the start of the queue */
	int end;     /* index to the end of the queue */
	int count;   /* no. of elements in the queue */
} queue;

extern queue *queue_init(int n);
extern int queue_insert(queue *q, int item);
extern int queue_delete(queue *q);
extern void queue_display(queue *q);
extern void queue_destroy(queue *q);

#endif
