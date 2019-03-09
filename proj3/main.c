///////////////////////////////////////////////////////////////////////////////
//                   
// Title:            Make
// Files:            text_parsing.c build_spec_graph.c build_spec_repr.c proc_creation_prog_exe.c
// Main File:  	     main.c 
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
 * C File with main function.
 * 1: Calls text parsing to parse Makefile
 * 2: Builds graph conataining targets and dependencies  
 * 3: Evaluates the graph post order
 *
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "text_parsing.h"
#include "build_spec_graph.h"

const int BUFSIZE = 1024;

int main(int argc, char* argv[]){

	char* buildTarget = NULL;
	//if target specified
	if(argc == 2){
		buildTarget = argv[1];
	}
	//Search for makefile/Makefile
	char* filePath = strdup("makefile");
	
	FILE* fp = openFile(filePath);
	if(fp == NULL){
		strcpy(filePath,"Makefile");
		fp = openFile(filePath);

	}
	
	if(fp == NULL){
		fprintf(stderr, "makefile not found in current directory\n");
		exit(1);
	}
	Node* head = NULL;
	Node* tail = NULL;
	read_lines(&head, &tail, fp);
	fclose(fp);
	Node* graphHead = createGraphs(&head, &tail);
	//If no target specified take first one as default
	Node* buildNode = graphHead-> dependencies[0];
	//Check if specfied target exists
	if(buildTarget!= NULL){
		buildNode = searchNodeSpec(&head, buildTarget);
		if(buildNode == NULL){
			fprintf(stderr,"make failed as target: %s not found\n", buildTarget);
			exit(1);
		}
	}
	if(buildNode!= NULL){
		//Check for cycles
		for (int i=0;i < graphHead -> size;i++) {
			traverseGraph(graphHead -> dependencies[i]);
		}
		//Evaluate the buildNode
		postOrderEvaluate(buildNode);
	}else{
		//If target not found
		fprintf(stderr, "graph returned empty: No target found to make\n");
		exit(1);
	}
	
	free(filePath);
	freeGraph(graphHead);	
	freeBuildSpec(head);
	freeNode(graphHead);
	return 0;
}
