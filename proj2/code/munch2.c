#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include "Queue.h"
#include "threadInput.h"

/**
 * C File that dequeues from queue2 and enqueues to queue3
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

/* Function for applying munch2 operation */
char* applyMunch2(char* s){
	int i = 0;
	for(; s[i]!='\0'; i++){
                if(islower(s[i])){
                        s[i] = toupper(s[i]);
                }
        }
	return s;
}

/* Function for performing enqueue and dequeue of munch2 thread */
void initMunch2(void* data){
	// unpack data for queues
	Thread_input* munch2_inp = data;
	Queue* munch1Munch2 = munch2_inp -> q1;
	Queue* munch2Writer = munch2_inp -> q2;
	
	// dequeue from queue2 and enqueue to queue3
	while(1){
		char* input = DequeueString(munch1Munch2);
		if(input == NULL){
			break;
		}
		EnqueueString(munch2Writer, applyMunch2(input));

	}
	// handle termination of queue and thread
	EnqueueString(munch2Writer, NULL);
	pthread_exit(data);
}
