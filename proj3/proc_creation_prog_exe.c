#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
/**
 * C file for executing a command in new process and waiting for its completion
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

/* Function that executes commands in a child process */ 
int executeCommand (char* argv[], int lineNo) {
	int status;
	pid_t rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed\n");
		exit(-1);
	}else if(rc == 0){
		//command execution
		 execvp(argv[0], argv);
		 exit(-1);
	}else{		
		waitpid(rc, &status, 0);
		if(WIFEXITED(status)){
			//if process exits with an error
			if(WEXITSTATUS(status)!=0){
				fprintf(stderr, "%d: 537make failed while running command: ", lineNo);
				int i = 0;
				while (argv[i] != NULL) {
					printf("%s ",argv[i]);
					i++;			
			 	}
				printf("\n");
				exit(1);
			}else{
				printf("Command: ");
				int i = 0;
				while (argv[i] != NULL) {
					printf("%s ",argv[i]);
					i++;			
			 	}
				printf("\n");
			}
		}
		
	}
	return 1;
}



