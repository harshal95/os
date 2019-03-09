#include "build_spec_repr.h"
void createGraphLinks(Node** head, Node** tail, Node* trav);
void traverseGraph(Node* curNode);
Node* createGraphs(Node** head, Node** tail);
void evaluateGraph(Node* curNode);
void postOrderEvaluate(Node* curNode);
void freeGraph(Node* curNode);
