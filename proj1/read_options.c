#include "node_structs.h"
#include "read_options.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/**
 * C File that reads options from user using getopt
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

const char* max_pid = "2147483647";
extern int BUF_SIZE;

/**
 * Check if the user gave an additional option
 */
int isOption(char* argv[], int optind){
	return (argv[optind] != NULL && argv[optind][0] != '-');
}

/**
 * Check if the user gave a valid argument
 */
int isValidArgument(char *optarg){	
	if((strcmp(optarg,"-") == 0)){
		return 1;
	}
	return 0;	
	
}

/**
 * Print error message for invalid input
 */
void printErrorMessage(){
	printf("Invalid input\n");
	printf("please enter something like 537ps -p 1234 -s -U -c\n");
}

/**
 * Function for parsing options given by user
 */
int parseOptions(int argc, char* argv[], char chopt){
	if(isOption(argv, optind)){
		printErrorMessage();
		exit(EXIT_FAILURE);
	}
	if(optarg){
		if(!isValidArgument(optarg)){
			printErrorMessage();
			exit(EXIT_FAILURE);
		}else if(chopt == 'U' || chopt == 'c' || chopt == 's' || chopt == 'S' || chopt == 'v'){
			return 0;
		}
	}
	return 1;
}

/**
 * Function for parsing pid option
 */
int parsePidOption(int argc, char* argv[]){
	int pid;
	if(isOption(argv, optind)){					
		if(optarg){
			printErrorMessage();
			exit(EXIT_FAILURE);
		}
		char* pidString = argv[optind++];
		pid = atoi(pidString);
		if(pid != 0){
			char* tempPid = (char*) malloc(BUF_SIZE);
			snprintf(tempPid, BUF_SIZE + 1, "%d", pid);
			if(strcmp(tempPid, pidString) != 0){
				printf("Process ID is too large/invalid\n");
				exit(EXIT_FAILURE);	
			}
			
		}else{
			printErrorMessage();
			exit(EXIT_FAILURE);
		}					
	}else{
		printErrorMessage();
		exit(EXIT_FAILURE);
	}
	return pid;
} 

/**
 * Function for setting option states
 */
optionNode* setFlags(int argc, char* argv[]){
	int opt;
	
	optionNode* optionFlags = (optionNode*)malloc(sizeof(optionNode));
	optionFlags -> state = 0;
	optionFlags -> vmem = 0;
	optionFlags -> pid = 0;
	optionFlags -> utime = 1;
	optionFlags -> cmdline = 1;
	
	while((opt = getopt(argc, argv, "s::U::S::v::c::p::"))!= -1){
		switch(opt){
			case 'p':
				optionFlags -> pid = parsePidOption(argc, argv);	
				break;

			case 's':
				optionFlags -> state = parseOptions(argc, argv, opt);
				break;

			case 'U':
				optionFlags -> utime = parseOptions(argc, argv, opt); 
				break;

			case 'S':
				optionFlags -> stime = parseOptions(argc, argv, opt); 
				break;

			case 'v':
				optionFlags -> vmem = parseOptions(argc, argv, opt); 
				break;

			case 'c':
				optionFlags -> cmdline = parseOptions(argc, argv, opt); 
				break;

			default:
				printErrorMessage();
				exit(EXIT_FAILURE);
				break;
		}
			
	}
	return optionFlags;
}

