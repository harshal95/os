#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include "Queue.h"

/**
 * C File that reads strings from stdin and enqueues it to the first queue
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

extern const int BUFSIZE;

/* Function for flushing excess characters in a string */
int flushStr(){
	int c = 10;

	//flushing remainder of string until newline or EOF
	while((c = getc(stdin))!= '\n' && c!= EOF){
	}
	
	return c;
}

/* Function for performing the thread action of reading strings from stdin */
void initReader(void* data){
	Queue* read2Munch1 = data; 
	int c;
        while((c = getc(stdin))!= EOF){
		
		// buffer for reading string
                char* s = (char*)malloc((BUFSIZE )*sizeof(char));

		// null check for malloc
		if(s == NULL){
			fprintf(stderr, "Failed to allocate memory for reader string..try reducing BUFSIZE\n");
			exit(1);
		}

		// handle empty string
		if(c == '\n'){
			s[0] = '\0';
			EnqueueString(read2Munch1, s);
			continue;
		}
		// store first char
                int i = 0;
                s[i++] = c;

		// variable for indicating overflow
                int bufFlush = 0;

		// last char seen while flushing
		int lastCh = 10;

		// perform reading of chars in a single line
                while((c = getc(stdin))!= '\n' && c!=EOF){

			// handle overflow
                        if(i >= BUFSIZE - 1){
                                fprintf(stderr, "Buffer limit exceeded and so flushing out string\n");
                                lastCh = flushStr();
                                bufFlush = 1;
                                free(s);
                                break;
                        }else{
                                s[i++] = c;
                        }

                }

		// incase of no overflow, enqueue string else don't enqueue and check if end of file is reached to exit
                if(bufFlush == 0){
                        s[i] = '\0';
			EnqueueString(read2Munch1, s);

                }else if(c == EOF || lastCh == EOF){
			break;
		}
        }

	// handle termination of queue and thread
	EnqueueString(read2Munch1, NULL);

	pthread_exit(data);

}

