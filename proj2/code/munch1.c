#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include "Queue.h"
#include "threadInput.h"

/**
 * C File that dequeues from queue1 and enqueues to queue2
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

/* Function for applying munch1 operation */
char* applyMunch1(char* s){
	int i = 0;
	for(; s[i]!='\0'; i++){
		int n = s[i];
                if(n == 32){
                        s[i] = '*';
                }
        }
	return s;
	
}

/* Function of performing dequeue and enqueue operations of munch1 thread*/
void initMunch1(void* data){
	Thread_input* munch1_inp = data;
	// unpack data to get queues
	Queue* read2Munch1 = munch1_inp -> q1;
	Queue* munch1Munch2 = munch1_inp -> q2;

	// dequeue from queue1 and enqueue to queue2
	while(1){
		char* input = DequeueString(read2Munch1);
		if(input == NULL){
			break;
		}
		EnqueueString(munch1Munch2, applyMunch1(input));


	}
	// handle termination of queue and thread
	EnqueueString(munch1Munch2, NULL);
	pthread_exit(data);
}

