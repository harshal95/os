#ifndef BUILDSPEC_H
#define BUILDSPEC_H
typedef struct CommandNode{
	char** commands;
	int commandNum;
	int lineNo;
	struct CommandNode* next;
}CommandNode;

typedef struct Node{
        char* name;
	char** depNames;
        struct Node** dependencies;
        //char** commands;
        //int commandNum;
	CommandNode* cmdList;
        int size;
	struct Node* next;
	int marked;
	int visited;
	int backEdge;
	//int modTime;
}Node;

Node* createNodeSpec(char* name);
Node* insertNodeSpec(Node** head, Node** tail, char **strings);
Node* updateCommandSpec(Node* curNode, char** strings, int lineNo);
void printNodeSpecs(Node* trav);
Node* searchNodeSpec(Node** head, char* name);
void freeBuildSpec(Node* trav);
void freeCommandList(CommandNode* cmdList);
void freeNode(Node* node);
#endif
