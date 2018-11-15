#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>	
#include <fcntl.h>
#include "node_structs.h"

/**
 * C file for parsing files of processes and displaying output
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

const int STATE = 3;
const int UTIME = 14;
const int STIME = 15;
const int VMEM = 1;
extern int BUF_SIZE;

extern char* procFolder;
extern char* statFile;
extern char* statmFile;
extern char* cmdFile;


/**
 * Function for reading file
 */
char* parseFile(char* filePath, int pos){
	//printf("trying to parse file %s\n", filePath);
	FILE *fp = fopen(filePath, "r");
	char *res = malloc(1024);
	
	if(fp != NULL){
		for(int i = 0; i < pos; i++){
			if(i == 1){
				//special case for reading first field as it is enclosed in paranthesis
				while(1){
					if(fgetc(fp) == ')'){
						if(fgetc(fp) ==' '){
							break;
						}else{
							fseek(fp, -1L, SEEK_CUR);
						}
					}
				}
			}else{
				fscanf(fp, "%s", res);
			}
		}
	}
	fclose(fp);
	return res;
}


/**
 * Function for reading cmdline file inside /proc
 */
char* parseCmdLine(char* filePath){
	FILE *fp = fopen(filePath, "r");
	char* buf = malloc(1000);

	int ch, i = 0;
	
	while((ch = fgetc(fp)) != EOF){
		if(ch != '\0'){
			buf[i++] = ch;
		}else{
			buf[i++] = ' ';
		}
	}
	buf[i] = '\0';
	fclose(fp);	
	return buf;

}

 

/**
 * Function for parsing and storing process information
 */
procOutput* parseInfo(optionNode *optionFlags, char* procPath, char* procName){	
	char* statmPath = (char*) malloc(BUF_SIZE);
	statmPath[0] = '\0';
	char *cmdPath = (char*) malloc(BUF_SIZE);
	cmdPath[0] = '\0';
	strcpy(statmPath, procPath);
	strcpy(cmdPath, procPath);
	strcat(procPath, statFile);
	procOutput* outputNode = (procOutput*)malloc(sizeof(procOutput));
	outputNode -> pid = atoi(procName);
	if(optionFlags -> state == 1){
		char* res = parseFile(procPath, STATE);
		outputNode -> state = res;
	}
	if(optionFlags -> utime == 1){
		char* res = parseFile(procPath, UTIME);
		outputNode -> utime = res;
	}
	if(optionFlags -> stime == 1){
		char* res = parseFile(procPath, STIME);	
		outputNode -> stime = res;
	}
	if(optionFlags -> vmem == 1){
		strcat(statmPath, statmFile);
		char* res = parseFile(statmPath, VMEM);	
		outputNode -> vmem = res;
	}
	if(optionFlags -> cmdline == 1){
		strcat(cmdPath, cmdFile);
		char* res = parseCmdLine(cmdPath);	
		outputNode -> cmdline = res;
	}
	free(statmPath);
	return outputNode;

}


/**
 * Function for printing output for each process
 */
void printOutputNode(procOutput* trav, optionNode* optionFlags){
	
	procOutput* outputNode = trav;
	printf("%d: ", outputNode -> pid);
	if(optionFlags -> state != 0){
		printf("state = %s ", outputNode -> state);
	}
	if(optionFlags -> utime != 0){
		printf("utime = %s ", outputNode -> utime);	
	}
	if(optionFlags -> stime != 0){
		printf("stime = %s ", outputNode -> stime);
	}
	if(optionFlags -> vmem != 0){
		printf("vmem = %s ", outputNode -> vmem);
	}
	if(optionFlags -> cmdline != 0){
		printf("cmdline = [ %s]", outputNode -> cmdline);
	}
	printf("\n");	
	
}


/**
 * Caller function for parsing and storing process information
 */
procOutput* callParseFunc(optionNode *optionFlags, char* procName){
	char* procPath = (char*) malloc(BUF_SIZE);
	procPath[0] = '\0';
	strcat(procPath, procFolder);
	strcat(procPath,"/");
	strcat(procPath,procName);
	procOutput* outputNode = parseInfo(optionFlags, procPath, procName);
	free(procPath);
	return outputNode;
}


