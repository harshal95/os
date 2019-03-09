#ifndef TREE_H
#define TREE_H
#include <stdlib.h>
typedef struct Node{
    void* addr;
    size_t len;
    int height;
    int alloc;
    struct Node* left;
    struct Node* right;    
}Node;

typedef struct LLNode{
    void* addr;
    size_t len;
    struct LLNode* next;
}LLNode;

Node* createNode(void* p, size_t len,int flag);
void printNode(Node* currentNode);
void inorderTraversal(Node* currentNode);
Node* insertNode(Node* currentNode, void* p, size_t len,int flag);
int max(int a, int b);
int getHeight(Node* currentNode);
int getBalanceFactor(Node* currentNode);
Node* rotateRight(Node* parent);
Node* rotateLeft(Node* parent);
Node* deleteNode (Node* currentNode,void* p);
void copyData(Node* Node1, Node* Node2);
Node* getInorderNode(Node* node);

LLNode* createLLNode(void* ptr, size_t len);
void appendList(void* ptr, size_t len, LLNode** head);
void createDeleteList(Node* treeNode, void* ptr, size_t len, LLNode** head);
LLNode* searchOverlap(Node* currentNode, void* ptr, size_t len);
void printDeleteList(Node* currentNode, void* ptr, size_t len);
#endif
