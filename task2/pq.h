/*
pq.h

Visible structs and functions for priority queues.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
/* The priority queue. */
struct pq;

/* Get a new empty priority queue that stores integers. */
struct pq *newPQ();

/* Add an item (integer) to the priority queue. */
void enqueue(struct pq *pq, int item, int priority);

/* Take the smallest item from the priority queue */
int deleteMin(struct pq *pq);

/* Update priority of an existing item */
void updatePQ(struct pq *pq, int item, int newPriority);

/* Returns 1 if empty, 0 otherwise. */
int emptyPQ(struct pq *pq);

/* Returns if an element is in the priority queue, 0 otherwise. */
int inPQ(struct pq *pq, int item);

/* Remove all items from priority queue (doesn't free) and free the queue. */
void freePQ(struct pq *pq);
