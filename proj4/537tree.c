#include <stdio.h>
#include <stdlib.h>
#include "537tree.h"

/**
 * C File that has operations(insert,delete,create,searchoverlap) in range tree
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */

/**
 * Creates a node to be inserted in Range tree
 */
Node* createNode(void* p, size_t len,int flag){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode-> addr = p;
    newNode-> len = len;
    newNode-> height = 1;
    newNode-> left = newNode-> right = NULL;
    newNode-> alloc = flag;
    return newNode;
}

/**
 * Prints contents of Node
 */
void printNode(Node* currentNode){
    printf("Node-addr: %p\n", currentNode-> addr);
    printf("Node-len: %ld\n", currentNode-> len);
    printf("Node alloc: %d\n", currentNode-> alloc);
    printf("------------------------------------\n");
}

/**
 * Inorder Traversal of Range tree
 */
void inorderTraversal(Node* currentNode){
    if(currentNode == NULL){
        return;
    }
    inorderTraversal(currentNode-> left);
    printNode(currentNode);
    inorderTraversal(currentNode-> right);
}

/**
 * Returns Max of a,b
 */
int max(int a, int b){
    return (a > b)? a: b;
}

/**
 * Returns height of the Node
 */
int getHeight(Node* currentNode){
    if(currentNode == NULL){
        return 0;
    }
    return currentNode-> height;
}
/**
 * Gets Balance Factor of the input Node
 */
int getBalanceFactor(Node* currentNode){
    if(currentNode == NULL){
        return 0;
    }
    return getHeight(currentNode-> left) - getHeight(currentNode-> right);
}
/** 
 * Right Rotation
 * */
Node* rotateRight(Node* parent){
    Node* lchild = parent-> left;
    Node* rgchild = lchild-> right;

    lchild-> right = parent;
    parent-> left = rgchild;

    parent-> height = max(getHeight(parent-> left), getHeight(parent-> right)) + 1;
    lchild-> height = max(getHeight(lchild-> left), getHeight(lchild-> right)) + 1;

    return lchild;
}

/** 
 * Left Rotation
 * */
Node* rotateLeft(Node* parent){
    Node* rchild = parent-> right;
    Node* lgchild = rchild-> left;

    rchild-> left = parent;
    parent-> right = lgchild;

    parent-> height = max(getHeight(parent-> left), getHeight(parent-> right)) + 1;
    rchild-> height = max(getHeight(rchild-> left), getHeight(rchild-> right)) + 1;

    return rchild;
}
/**
 *Inserting a Tuple as node in the range tree
 * */
Node* insertNode(Node* currentNode, void* p, size_t len,int flag){
    if(currentNode == NULL){
        return createNode(p, len,flag);
    }
    if(p < currentNode->addr){
        currentNode->left = insertNode(currentNode-> left, p, len,flag);
    }else if(p > currentNode-> addr) {
        currentNode-> right = insertNode(currentNode-> right, p, len,flag);
    }else{
        fprintf(stderr,"Error: Malloc returned same address as previously allocated\n");
        exit(-1);
    }
    currentNode-> height = 1 + max(getHeight(currentNode-> left), getHeight(currentNode-> right));
    int balanceFactor = getBalanceFactor(currentNode);
    
    // left left rotate
    if(balanceFactor > 1 && p < currentNode-> left-> addr){
        return rotateRight(currentNode);
    }
    
    // right right rotate
    if(balanceFactor < -1 && p > currentNode-> right-> addr){
        return rotateLeft(currentNode);
    }
    
    //left right rotate
    if(balanceFactor > 1 && p > currentNode-> left-> addr){
        currentNode-> left = rotateLeft(currentNode-> left);
        return rotateRight(currentNode);
    }

    //right left rotate
    if(balanceFactor < -1 && p < currentNode-> right-> addr){
        currentNode-> right = rotateRight(currentNode-> right);
        return rotateLeft(currentNode);
    }

    return currentNode;
}
/** 
 *Getting the Inorder Successor of the Node
 * */
Node* getInorderNode(Node* node) {
    Node* curr = node;

    while (curr-> left != NULL) {
        curr = curr-> left;
    }
    return curr;
}



/**
 *
 * Copying contents from Node2 to Node1 */
void copyData(Node* Node1, Node* Node2) {
    Node1-> addr = Node2-> addr;
    Node1-> len = Node2-> len;
    Node1-> alloc = Node2-> alloc;

}

/*
 * Deleting a Node from the range tree
 * */
Node* deleteNode (Node* currentNode,void* p) {
    //BST delete
    if (currentNode == NULL) {
        return currentNode;
    }
    // if p addr is > currentNode addr
    if (p > currentNode-> addr) {
        currentNode-> right = deleteNode(currentNode-> right,p);
    }
    // if p addr is < currentNode addr
    else if (p < currentNode-> addr) {
        currentNode-> left = deleteNode(currentNode-> left,p);
    }
    //if addr match
    else {
        
        //node with only one child or no child
        if ( (currentNode->left == NULL) || (currentNode->right == NULL)) {
            Node *temp = currentNode->left ? currentNode->left:currentNode->right;
            
            //No child case
            if (temp == NULL) {

               temp = currentNode;
               currentNode = NULL;
            }
            else {
		//Copy contents from the child
            	copyData(currentNode,temp);
		currentNode->left = temp->left;
		currentNode->right = temp->right;
		currentNode->height = temp->height;
	    }
            free(temp);
            //node with 2 children
        }
        else {
            //Get inorder successor
            Node *temp = getInorderNode(currentNode->right);
            //copy contents
            copyData(currentNode,temp);
            //Delete the inorder successor
            currentNode-> right = deleteNode(currentNode-> right,temp -> addr);
        }
    }

    //if tree had one Root
    if (currentNode == NULL) {
        return currentNode;
    }
   // update hieght
    currentNode-> height = 1 + max(getHeight(currentNode-> left),getHeight(currentNode-> right));
    int balanceFactor = getBalanceFactor(currentNode);
    //Rotations to balance trees
    //Left Left
    if (balanceFactor > 1 && getBalanceFactor(currentNode-> left) >= 0 ) {
        return rotateRight(currentNode);
    }
    //Left Right
    if (balanceFactor > 1 && getBalanceFactor(currentNode-> left) < 0 ) {
        currentNode -> left = rotateLeft(currentNode-> left);
        return rotateRight(currentNode);
    }
    //Right Right
    if (balanceFactor < -1 && getBalanceFactor(currentNode-> right) <= 0 ) {
        return rotateLeft(currentNode);
    }
    //Right Left
    if (balanceFactor < -1 && getBalanceFactor(currentNode-> right) > 0) {
        currentNode -> right = rotateRight(currentNode-> right);
        return rotateLeft(currentNode);
    }
    return currentNode;
}

/** 
 *Creating Node for List of items to delete  due to overlap
 * */
LLNode* createLLNode(void* ptr, size_t len){
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    node-> addr = ptr;
    node-> len = len;
    node-> next = NULL;
    return node;
}

/** 
 *Adding a node to the linked list of items for deletion
 * */
void appendList(void* ptr, size_t len, LLNode** head){
    LLNode* temp = createLLNode(ptr, len);
    temp-> next = *head;
    *head = temp;
}
/** 
 *Creating List of Items to delete
 * */
void createDeleteList(Node* treeNode, void* ptr, size_t len, LLNode** head){
    if(treeNode == NULL){
        return;
    }
    createDeleteList(treeNode-> right, ptr, len, head);

    if(treeNode-> addr < ptr && treeNode->addr + treeNode-> len - 1 >= ptr){
        appendList(treeNode-> addr, treeNode-> len, head);
        
    }else if(treeNode-> addr >= ptr && treeNode->addr + treeNode->len - 1 <= ptr + len - 1){
        appendList(treeNode-> addr, treeNode-> len, head);
    }else if(treeNode-> addr <= ptr + len - 1 && treeNode-> addr + treeNode-> len - 1 > ptr + len - 1){
        appendList(treeNode-> addr, treeNode-> len, head);
    }

    createDeleteList(treeNode-> left, ptr, len, head);
}

/**
 *Search for overlap among free Nodes
 * */
LLNode* searchOverlap(Node* currentNode, void* ptr, size_t len){
    LLNode* head = createLLNode(NULL, 0);    
    while(currentNode!= NULL){
        if(ptr + len - 1 < currentNode-> addr){
            currentNode = currentNode-> left;
        }else if(ptr > currentNode-> addr + currentNode->len - 1){
            currentNode = currentNode-> right;
        }else{
            printf("overlap detected at %p\n", currentNode-> addr);
            createDeleteList(currentNode, ptr, len, &head);
            break;            
        }
    }
    return head;
}
