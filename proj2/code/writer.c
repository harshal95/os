#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include "Queue.h"
#include "threadInput.h"

/**
 * C File that dequeues from queue3 and outputs to stdout
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

/* Function for performing dequeue and stdout operation of writer */
void initWriter(void* data){

	Queue* munch2Writer = data;
	//dequeue from queue3 and output to stdout
	while(1){
		char* input = DequeueString(munch2Writer);
	       	if(input == NULL){
			break;
		}
		printf("%s\n", input);
		// freeing input buffer
		free(input);
	}
	//handle thread termination
	pthread_exit(data);
	
}
