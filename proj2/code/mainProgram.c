///////////////////////////////////////////////////////////////////////////////
//                   
// Title:            Shared Memory Producer/Consumer Program
// Files:            mainProgram(.c), Queue(.c/.h), reader(.c/.h), munch1(.c/.h), munch2(.c/.h), writer(.c/.h)
// Main File:        mainProgram.c
// Semester:         (CS537) Fall 2018
//
// Author:           SRI HARSHAL PARIMI
// Email:            sparimi@wisc.edu
// CS Login:         sparimi@cs.wisc.edu
// Lecturer's Name:  Bart Miller
// 
//
///////////////////////////////////////////////////////////////////////////////
//
// Pair Partner:     NIVEDITHA HARIHARAN
// Email:            nhariharan@wisc.edu
// CS Login:         nhariharan@cs.wisc.edu
// Lecturer's Name:  Bart Miller
//
///////////////////////////////////////////////////////////////////////////////
/**
 * C File with main function. Performs following:
 * 1: Creates 3 queues
 * 2: Creates threads for reader, munch1, munch2, writer
 * 3: Joins threads
 * 4: Prints queue statistics of all queues and exits
 *
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include "Queue.h"
#include "threadInput.h"
#include "reader.h"
#include "munch1.h"
#include "munch2.h"
#include "writer.h"

// constant for handling queue size 
const int QSIZE = 10;

// constant for handling buffer size.. in this case 1024 holds 1023 characters + '\0'
const int BUFSIZE = 1024;
   
int main() {

        int errThread;
        
	//Initialize 3 Queues
        Queue *read2Munch1 = CreateStringQueue(QSIZE);
        Queue *munch1Munch2 = CreateStringQueue(QSIZE);
        Queue *munch2Writer = CreateStringQueue(QSIZE);
        
	//Initialize threads
        pthread_t thread1;
        pthread_t thread2;
	pthread_t thread3;
        pthread_t thread4;

	// package arguments to input to Munch1 Thread
        Thread_input munch1_args;
        munch1_args.q1 = read2Munch1;
        munch1_args.q2 = munch1Munch2;

        errThread = pthread_create(&thread1, NULL, (void*)initReader, read2Munch1);
        
	if(errThread != 0){
                fprintf(stderr, "Thread Reader not created\n");
		exit(1);
	}
	
        errThread = pthread_create(&thread2, NULL, (void*)initMunch1, &munch1_args);
        
	if(errThread != 0){
                fprintf(stderr, "Thread munch1 not created\n");
		exit(1);
	}
	
	//package input arguments to Munch2 Thread
	Thread_input munch2_args;
	munch2_args.q1 = munch1Munch2;
	munch2_args.q2 = munch2Writer;

	errThread = pthread_create(&thread3, NULL, (void*)initMunch2, &munch2_args);

	if(errThread != 0){
		fprintf(stderr, "Thread munch2 not created\n");
		exit(1);
	}

	errThread = pthread_create(&thread4, NULL, (void*)initWriter, munch2Writer);
	
	if(errThread != 0){
		fprintf(stderr, "Thread writer not created\n");
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	
	fprintf(stderr, "Printing statistics for Queue1\n");	
	PrintQueueStats(read2Munch1);
	
	fprintf(stderr, "Printing statistics for Queue2\n");	
	PrintQueueStats(munch1Munch2);

	fprintf(stderr, "Printing statistics for Queue3\n");	
	PrintQueueStats(munch2Writer);

}

