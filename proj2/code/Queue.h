#include <semaphore.h>

/**
 * h File that handles synchronized queue
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */
typedef struct Queue{
	char** arr;
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
	int size;
	sem_t empty;
	sem_t full;
}Queue;

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);
