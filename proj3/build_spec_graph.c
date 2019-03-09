#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include "build_spec_repr.h"
#include "proc_creation_prog_exe.h"

extern const int BUFSIZE;
/**
 * C file for creating,traversing and evaluating graph
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

/* Function that creates links in the graph */
void createGraphLinks(Node** head, Node** tail, Node* trav){
	if(trav-> marked == 1){
		return;
	}
	trav-> marked = 1;
//	printf("entering dfs for %s\n", trav-> name);
	for(int i = 0; i < trav-> size; i++){
//		printf("searching for %s\n", trav-> depNames[i]);	
		Node* searchedNode = searchNodeSpec(head, trav->depNames[i]);
//		printf("finished searching for %s\n", trav-> depNames[i]);
		if(searchedNode!= NULL){
			trav-> dependencies[i] = searchedNode;
//			printf("dfs for %s\n", searchedNode-> name);
			createGraphLinks(head, tail, trav-> dependencies[i]);
				
		}else{
			//char* newbuf = (char*)calloc(BUFSIZE, sizeof(char));
			//newbuf[0] = '\0';
			//strncpy(newbuf, trav-> depNames[i], strlen(trav-> depNames[i]));
			char* newbuf = strdup(trav-> depNames[i]);
			Node* createdNode = createNodeSpec(newbuf);
			trav-> dependencies[i] = createdNode;
			createdNode-> marked = 1;
			createdNode-> next = NULL;
			if(*tail){
				(*tail)-> next = createdNode;
				*tail = createdNode;
			}
		}
	}

}
/* Function that creates the graph */
Node* createGraphs(Node** head, Node** tail){
	Node* graphHead = createNodeSpec(NULL);
	int i = 0;
	Node* trav = *head;
	while(trav){
		if(trav-> marked == 0){
			createGraphLinks(head, tail, trav);
			graphHead-> dependencies[i] = trav;
			char* newbuf = strdup(trav-> name);
			graphHead-> depNames[i] = newbuf;
			graphHead-> size++;
			i++;
		}
		trav = trav-> next;
	}
	return graphHead;
}

/* Function that evaluate the current node */
void evaluateGraph(Node* curNode){
    if(curNode->cmdList == NULL){
	return;
    }	
    FILE* fp = fopen(curNode -> name, "r");
    FILE* fp_dep;
    time_t root_mod_time;
    time_t dep_mod_time;

    //buffer to store info from fstat
    struct stat buf;
    
    //flag to check if need to run commands
    int is_dep_mod = 0;
    int fd;
    
    //If file doesnt exist or not a file
    if (fp == NULL) {
	//to make sure target runs
        root_mod_time = 0;
	is_dep_mod = 1;
    }

    if(fp!= NULL){
	//Getting file descriptor by doing fstat
    	fd = fileno(fp);
       	fstat(fd,&buf);
   	root_mod_time = buf.st_mtime;
    }

     //looping through dependencies checking if any dependency is older than the root
    for(int i = 0; i < curNode -> size; i++){
        //printf("dep : %s ", curNode -> dependencies[i] -> name);
	fp_dep = NULL;
	if (curNode -> dependencies[i] != NULL) {
     		fp_dep = fopen(curNode -> dependencies[i] -> name,"r");
	}	
        //If dependencies is not a file
	if (fp_dep == NULL) {
		is_dep_mod = 1;
		break;
        }
        else {
                fd = fileno(fp_dep);	
    		fstat(fd,&buf);
    		dep_mod_time = buf.st_mtime;
		if (dep_mod_time > root_mod_time) {
			is_dep_mod = 1;
			break;
		}
        }
     }

     CommandNode* trav = curNode -> cmdList;

     //Traverse and execute all commands
     if (is_dep_mod) {
	printf("537make: trying to make target %s\n", curNode-> name);
     	while(trav){
		executeCommand(trav -> commands, trav-> lineNo);	
		trav = trav-> next;
	}
	printf("537make: succeeded for target %s\n", curNode-> name);
     }else{
     	printf("537make: nothing to update for target %s\n", curNode-> name);
     }
	
}
/* Function that calls evaluateGraph post order */
void postOrderEvaluate(Node* curNode) {
	if(curNode == NULL){
		return;
	}
	
	for(int i = 0; i < curNode -> size; i++){
		postOrderEvaluate(curNode -> dependencies[i]);
	}
	evaluateGraph(curNode);
}
/* Function that checks for cyclic dependencies */
int traverseGraph(Node* curNode){
	if(curNode-> visited == 0){
		curNode-> visited = 1;
		curNode-> backEdge = 1;
		for(int i = 0; i < curNode-> size; i++){
			Node* depNode = curNode-> dependencies[i];
			if(depNode->visited == 0 && traverseGraph(depNode) == 1){
				printf("Cyclic dependency detected between %s and %s\n",curNode ->name, depNode -> name);
				exit(1);
			}else if(depNode->backEdge == 1){
				printf("Cyclic dependency detected between %s and %s\n",curNode ->name, depNode -> name);
				exit(1);
			}
		}
			
	}

	curNode-> backEdge = 0;
	return 0;
}

void freeGraph(Node* curNode){
	if(curNode == NULL){
		return;
	}
	for(int i = 0; i < curNode-> size; i++){
		freeGraph(curNode-> dependencies[i]);
		curNode-> dependencies[i] = NULL;
	}
}

