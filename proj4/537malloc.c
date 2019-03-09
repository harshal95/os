#include <stdlib.h>
#include <stdio.h>
#include "537tree.h"

/**
 * C File that does the following
 * 1.malloc537
 * 2.free537
 * 3.memcheck537
 * 4.realloc537
 *
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

//Root of the range tree
Node* root = NULL;

/**
 *Checks and allocates memory and inserts tuple in range tree
 * */
void *malloc537(size_t size){
	void* p = malloc(size);
	if (size == 0) {
		fprintf(stderr,"Warning : Trying to allocate memory of size 0\n");		
	}
	if(p == NULL){
		fprintf(stderr,"malloc537: returned NULL\n");
		exit(-1);
	}
	//check for overlap
	LLNode* headOverlap = searchOverlap(root,p,size);
	//If no overlap
	if (headOverlap-> addr == NULL) {
  		root = insertNode(root, p, size,1);
	}
	//If overlap
	else {
		LLNode* temp = headOverlap;
		//check if partial overlap in the first node
		if (p > temp-> addr) {
			root = insertNode(root, headOverlap-> addr,p - headOverlap-> addr,0);
			temp = temp-> next;			
		}
		//delete all other nodes
		while (temp-> addr != NULL) {			
			root = deleteNode(root ,temp-> addr);
			temp = temp-> next;	
		}
		//Insert the node
		root = insertNode(root, p , size,1);	
	}

	return p;
	
}

/**
 *If appropriate ptr, frees ptr and marks alloc in the node of range tree
 * */
void free537(void* ptr) {
	Node* cur = root;
	if(ptr == NULL){
		fprintf(stderr, "Error: NULL pointer passed to free537\n");
		exit(-1);
	}
	while(cur!= NULL){
		if(ptr == cur-> addr){
			if(cur-> alloc == 0){
				fprintf(stderr, "Error : Freeing memory that was previously freed (double free)\n");
				exit(-1);
			}else{
				if (cur-> len == 0) {
					fprintf(stderr, "Error: Freeing memory of size 0\n");
					exit(-1);
				}
				cur-> alloc = 0;
				//free the ptr
				free(ptr);
				return;
			}
		}
		if(ptr < cur-> addr){
			cur = cur-> left;
		}else if(ptr > (cur-> addr + cur-> len - 1)){
			cur = cur-> right;
		}else{
			fprintf(stderr, "Error : Freeing memory that is not the first byte of the range of memory that was allocated.\n");
			exit(-1);
		}

	}
	fprintf(stderr, "Error: Freeing memory that has not been allocated\n");
	exit(-1);


}
/**
 *Does realloc and removes existing tuple and inserts a newly allocated one in range tree
 * */
void* realloc537(void *ptr,size_t size) {

	if (ptr == NULL) {
		return malloc537(size);
	}
	else if (ptr != NULL && size == 0) {
		fprintf(stderr,"Warning: Trying to call realloc on a size 0 memory\n");
		free537(ptr);
		return NULL;
	}
	else {
		Node* curr = root;
		//find the target node
		while (curr != NULL) {
			if (ptr > curr-> addr) {
				curr = curr-> right;
			}
			else if (ptr < curr-> addr) {
				curr = curr-> left;
			}
			else {
				if (curr -> alloc == 0) {
					fprintf(stderr,"Error : Realloc on a node which was freed by free537\n");
					exit(-1);
				}
				else {
					root = deleteNode(root , curr-> addr);
					break;
				}
			}
		}

		if (curr == NULL) {
			fprintf(stderr,"Error : Realloc on ptr that was never allocated");
			exit(-1);
		}

		//get the newly alloc ptr
		void* new_ptr = realloc(ptr,size);
		//NULL check 
		if (new_ptr == NULL) {
			fprintf(stderr,"Error : Realloc returned NULL");
			exit(-1);		
		}
		//gets the overlap list
		LLNode* headOverlap = searchOverlap(root,new_ptr,size);
		//If no overlap
		if (headOverlap-> addr == NULL) {
  			root = insertNode(root, new_ptr, size,1);
		}
		//If overlap
		else {
			LLNode* temp = headOverlap;
			//check if partial overlap in head
			if (new_ptr > temp-> addr) {
				root = insertNode(root, headOverlap-> addr,new_ptr - headOverlap-> addr,0);
				temp = temp-> next;			
			}
			//delete all other nodes
			while (temp-> addr != NULL) {
				root = deleteNode(root ,temp-> addr);
				temp = temp-> next;	
			}
			//Insert the node
			root = insertNode(root, new_ptr , size,1);	
		}
		return 	new_ptr;
	}
}

/**
 *Checks if complete overlap with one of the existing ranges in the range tree
 * */
void memcheck537(void *ptr, size_t size){
	Node* cur = root;
	if(ptr == NULL){
		fprintf(stderr, "Error : NULL pointer passed to memcheck537\n");
		exit(-1);
	}
	while(cur!= NULL){
		if((ptr >= cur-> addr) && (ptr + size - 1 <= cur-> addr + cur->len - 1)){
			if(cur-> alloc == 1){
				return;
			}else{
				fprintf(stderr, "Error : memcheck537 tried to access memory already freed by free537\n");
				exit(-1);
			}
		}
		if(ptr < cur-> addr){
			cur = cur-> left;
		}else{
			cur = cur-> right;
		}
	}
	fprintf(stderr, "Error: memcheck537 tried to access memory never allocated\n");
	exit(-1);
}


