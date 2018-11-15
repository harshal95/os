///////////////////////////////////////////////////////////////////////////////
//                   
// Title:            ps command
// Files:            option_processor(.c/.h), read_options(.c/.h), get_proc(.c/.h), info_parser(.c/.h)
// Main File:  		 option_processor.c
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
 * C File with main function. Calls 3 modules:
 * 1: Getting options from user
 * 2: Getting list of processes
 * 3: Reads information inside /proc directory and prints output
 *
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

#include "node_structs.h"
#include "read_options.h"
#include "get_proc.h"
#include "info_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Function creates dummy node for maintaining linked list of process names
 * @return processNode
 */
procNode* createDummyNode(){
	procNode* dummy_head = (procNode*)malloc(sizeof(procNode));
	dummy_head -> next = NULL;
	return dummy_head;
}

int main(int argc, char* argv[]){
	// calls method for reading options from user	
	optionNode* optionFlags = setFlags(argc, argv);
	
	procNode* node_head = createDummyNode();
	procNode* tail = node_head;

	//calls method for appending process nodes in linked list
	accessProcessFile(tail, optionFlags -> pid);
	procNode* trav = node_head -> next;
	
	//calls method for outputting process information
	while(trav != NULL){
		procOutput* outputNode = callParseFunc(optionFlags, trav -> procName);
		printOutputNode(outputNode, optionFlags);
		procNode* freeNode = trav;
		trav = trav -> next;
		free(freeNode);
	}
	return 0;
}

		
