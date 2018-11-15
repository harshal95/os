#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>

/**
 * C File that handles synchronized queue
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

typedef struct Queue{
	char **arr;
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
	int size;
	int front;
	int rear;
  	sem_t mutex;
  	sem_t empty;
  	sem_t full;
	sem_t block_mutex;
}Queue;

/* Function that creates a queue of given size */
Queue *CreateStringQueue(int size) {

	//allocating memory for the whole queue
	Queue* q = (Queue*)malloc(sizeof(Queue));
	
	if(q == NULL){
		fprintf(stderr, "Failed to allocate mem for queue node\n");
		exit(1);
	}
	
	int errInit = 0;
	
	//allocating memory for the array in the queue
	q -> arr = (char**)malloc(size*sizeof(char*));

	if(q -> arr == NULL){
		fprintf(stderr, "Failed to allocate mem for queue array of strings\n");
		exit(1);
	}
	
	//initialize other counts
	q -> enqueueCount = 0;
	q -> dequeueCount = 0;
	q -> enqueueBlockCount = 0;
	q -> dequeueBlockCount = 0;
	q -> size = size;
	q -> front = 0;
	q -> rear = 0;
	
	//Initalize semaphores
	errInit = sem_init(&q -> empty , 0 , size);
	if(errInit == -1){
		fprintf(stderr,"Semaphore empty not initialized properly\n");
		exit(1);
	}
	errInit = sem_init(&q -> full , 0 , 0);
	if(errInit == -1){
		fprintf(stderr, "Semaphore full not initialized properly\n");
		exit(1);
    	}
	errInit = sem_init(&q -> mutex, 0 , 1);
	if (errInit == -1){
		fprintf(stderr, "Semaphore mutex not initialized properly");
		exit(1);
	}
	errInit = sem_init(&q -> block_mutex, 0, 1);
	if (errInit == -1){
		fprintf(stderr, "Semaphore block mutex not initialized properly");
		exit(1);
	}
	return q;
}

/* Function for enqueueing a string to queue */
void EnqueueString(Queue *q, char *string) {
	int size = q -> size;
	int front = q -> front;
	int rear = q -> rear;
	// check blocking condition
	sem_wait(&q -> block_mutex);
	if((rear + 1) % size == front){
		q -> enqueueBlockCount++;
	}
	sem_post(&q -> block_mutex);

	// wait if queue is full
	sem_wait(&q -> empty);
	
	//actual enqueue
	sem_wait(&q -> mutex);
	q -> arr[rear] = string;
	rear = (rear+1) % size;
	q -> rear = rear;
	if (string != NULL) {
		q -> enqueueCount++;
	}
	sem_post(&q -> mutex);
	
	sem_post(&q -> full);
}

/* Function for dequeueing a string from queue */
char * DequeueString(Queue *q) {
	char* dequeuedStr;
	int size = q -> size;
	int front = q -> front;
	int rear = q -> rear;
	// check blocking condition
	sem_wait(&q -> block_mutex);
	if(front == rear){
		q -> dequeueBlockCount++;
	}
	sem_post(&q -> block_mutex);

	// wait if queue is full
	sem_wait(&q -> full);
	
	// perform actual dequeue
	sem_wait(&q -> mutex); 
	dequeuedStr = q -> arr[front];
	front = (front + 1) % size;
	q -> front = front;
	if(dequeuedStr != NULL){
	 q -> dequeueCount++;
	}
	sem_post(&q -> mutex);
	
	sem_post(&q -> empty);
	
	return dequeuedStr;
}
   
/* Function for printing queue statistics */
void PrintQueueStats(Queue *q) {

	fprintf(stderr,"\n");
	fprintf(stderr,"EnqueueCount:%20d\n",q -> enqueueCount);
	fprintf(stderr, "DequeueCount:%20d\n",q -> dequeueCount);
	fprintf(stderr, "EnqueueBlockCount:%15d\n",q -> enqueueBlockCount);
	fprintf(stderr, "DequeueBlockCount:%15d\n",q -> dequeueBlockCount);
	fprintf(stderr,"\n");
	// At the end of all operations.. to free queue allocation
	free(q -> arr);
	free(q);
}




