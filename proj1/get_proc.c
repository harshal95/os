#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "node_structs.h"
#include "get_proc.h"


/**
 * C File that reads /proc to get list of accessible processes
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

char* procFolder = "/proc";
char* statusFile = "/status";
char* statFile = "/stat";
char* statmFile = "/statm";
char* cmdFile = "/cmdline";
int UID_LINE = 9;
int BUF_SIZE = 10000;


/**
 * Opens file directory
 */
DIR* openProcDirectory(){
	DIR *myDirectory;	
	myDirectory = opendir(procFolder);
	if(myDirectory){
		//printf("proc directory is accessible\n");
	}else{
		if(errno == ENOENT){
			printf("the directory does not exist\n");
		}else if(errno == ENOTDIR){
			printf("this file is not a directory\n");
		}else if(errno == EACCES){		
			printf("you do not have rights to open this folder\n");
		}else{
			printf("new error while opening proc directory, check manual!\n");
		}
		exit(EXIT_FAILURE);
	}
	return myDirectory;
}

/**
 * Inserting process node to the linked-list
 */
procNode* insertProcNode(procNode* tail, char* processName){
	procNode* temp = (procNode*)malloc(sizeof(procNode));
	temp -> procName = processName;
	temp -> next = NULL;
	tail -> next = temp;
	tail = temp;
	return tail;
}


/**
 * Check if user can access the process
 */
int isUserProcess(char* filePath){
	FILE* fp = fopen(filePath, "r");
	char *buf = (char*)malloc(BUF_SIZE);
	char *valbuf = (char*)malloc(BUF_SIZE);
	if(fp != NULL){
		for(int i = 0; i < UID_LINE; i++){
			fgets(buf, BUF_SIZE, fp);
		}
		
		valbuf = strtok(buf, " \t");
		int count = 0;
		while(valbuf != NULL && count <= 1){
			count++;
			valbuf = strtok(NULL, " \t");
		}

		int uid = atoi(valbuf);
		if(uid == getuid()){
			return 1;
		}
	}
	free(buf);
	return 0;
}


/**
 * Reads the file inside a process and adds a valid accessible process to linked-list
 */
void accessProcessFile(procNode* tail, int pid){
	DIR *procDirec = openProcDirectory();
	struct dirent *myFile;
	if(pid != 0){
		char *filePath = (char*) malloc(BUF_SIZE);
		strcpy(filePath, procFolder);
		strcat(filePath, "/");
		char * procName = (char*) malloc(BUF_SIZE);
		snprintf(procName, BUF_SIZE + 1, "%d", pid);
		strcat(filePath, procName);
		strcat(filePath, statusFile);
		
		if(isUserProcess(filePath)){
			tail = insertProcNode(tail, procName);
			
		}else{
			printf("process id %d does not belong to user\n", pid);
		}
		free(filePath);
	}
	else{
		while((myFile = readdir(procDirec))){
			char *processName = myFile -> d_name;			
			if(atoi(processName)!= 0){
				char *filePath = (char*) malloc(BUF_SIZE);	
				strcpy(filePath, procFolder);
				strcat(filePath, "/");
				strcat(filePath, processName);
				strcat(filePath, statusFile);
				
				if(isUserProcess(filePath)){
					tail = insertProcNode(tail, processName);
				}
				free(filePath);
			}
		}
	}
	if(closedir(procDirec) == 0){
		//printf("the directory is closed\n");
	}else{
		printf("the directory cannot be closed\n");
		exit(EXIT_FAILURE);			
	}

}

