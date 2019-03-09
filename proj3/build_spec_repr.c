#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include "build_spec_repr.h"

/**
 * C file for building specification abstraction. It creates,updates and accesses build specification.
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */
	
extern const int BUFSIZE;
/* Function that creates a new node along with its dependencies */
Node* createNodeSpec(char* name){
	Node* node = (Node*)calloc(1, sizeof(Node));
	//creates node
	if(name!= NULL){
		node -> name = name;        
	}else{
		node-> name = strdup("GRAPH-HEAD-DUMMY");
	}
	node-> dependencies = (Node**)malloc(BUFSIZE*sizeof(Node*));
	node-> depNames = (char**)malloc(BUFSIZE*(sizeof(char*)));
	node-> cmdList = NULL;
	//creates dependencies
	for(int i = 0; i < BUFSIZE; i++){
		node-> dependencies[i] = NULL;
		node-> depNames[i] = NULL;
	}
	node-> size = 0;
	//used for checking cyclic dependencies
	node-> marked = 0;
	node-> visited = 0;
	node-> backEdge = 0;
	return node;

}
void freeCommandList(CommandNode* cmdList){
	CommandNode* trav = cmdList;
	while(trav){
		CommandNode* nextNode = trav-> next;
		for(int i = 0; i < trav->commandNum; i++){
			free(trav-> commands[i]);
		}
		free(trav-> commands);
		free(trav);
		trav = nextNode;
	}
}
void freeNode(Node* node){
//	printf("freeing node %s\n", node-> name);
	for(int i = 0; i < node-> size; i++){
		free(node-> dependencies[i]);
//		printf("freeing depname %s\n", node-> depNames[i]);
		free(node-> depNames[i]);
	}
	free(node-> dependencies);
	free(node-> depNames);
	freeCommandList(node-> cmdList);
	free(node-> name);
	free(node);
}
/* Function that inserts a node into list */
Node* insertNodeSpec(Node** head, Node** tail, char **strings){
	char* targetName = strings[0];
	Node* targetNode = createNodeSpec(targetName);
	//appending node and incrementing tail pointer
	if(*head == NULL){
		*head = targetNode;
		*tail = targetNode;
	}else{
		(*tail)-> next = targetNode;
		*tail = targetNode;
	}
	int i = 1;
	//setting dependencies
	while(strings[i]!= NULL){
		if(strings[i][0] == '\0'){
			free(strings[i]);
		}else{
			targetNode-> depNames[targetNode-> size] = strings[i];
			targetNode-> size++;
		}
		i++;
	}
	return targetNode;
}
/* Function that updates commands in the target node */
Node* updateCommandSpec(Node* curNode, char** strings, int lineNo){
	int i = 0;
	CommandNode* cmds = (CommandNode*)malloc(sizeof(CommandNode));
	cmds-> commandNum = 0;
	cmds-> next = NULL;
	cmds-> lineNo = lineNo;
	cmds-> commands = (char**)malloc(BUFSIZE*(sizeof(char*)));
	for(i = 0; i < BUFSIZE; i++){
		cmds-> commands[i] = NULL;
	}

	i = 0;
	//setting commands from the list of strings
	while(strings[i]!= NULL){
		if(strings[i][0] == '\0'){
			free(strings[i]);
		}else{
			cmds -> commands[cmds-> commandNum] = strings[i];	
			
			cmds -> commandNum++;
		}
		i++;
	}
	
	CommandNode* trav = curNode -> cmdList;
	
	if(trav == NULL){
		curNode -> cmdList = cmds;
	}else{
		while(trav-> next!= NULL){
			trav = trav-> next;
		}
		trav-> next = cmds;
	}

	return curNode;

}
/* Function that prints specification of the node */
void printNodeSpecs(Node* trav){
	while(trav!= NULL){
		printf("%s\n", trav-> name);
		printf("%d\n", trav-> size);
		for(int i = 0; i < trav-> size; i++){
			printf("%s\n", trav->depNames[i]);
		}
		printf("Commands\n");
		CommandNode* cmdtrav = trav-> cmdList;
		while(cmdtrav){
			for(int i = 0; i < cmdtrav-> commandNum; i++){
				printf("%s\n", cmdtrav-> commands[i]);
			}
			cmdtrav = cmdtrav-> next;
		}
		printf("-------------------\n");
		trav = trav-> next;
	}
}
/* Function that searches for a node in the list */
Node* searchNodeSpec(Node** head, char* name){
	Node* trav = *head;
	while(trav){
		if(strcmp(trav->name,name)==0){
			return trav;
		}
		trav = trav-> next;
	}
	return NULL;
}
/* Function that frees all build spec nodes */
void freeBuildSpec(Node* trav2){
	while(trav2!= NULL){
		Node* nextNode = trav2-> next;
		freeNode(trav2);
		trav2 = nextNode;
	}
	
}
