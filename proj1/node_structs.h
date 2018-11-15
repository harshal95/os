typedef struct procNode{
	char *procName;
	struct procNode *next;
}procNode;

typedef struct procOutput{
	int pid;
	char* state;
	char* utime;
	char* stime;
	char* vmem;
	char* cmdline;
}procOutput;

typedef struct optionNode{
	int pid;
	int state;
	int utime;
	int stime;
	int vmem;
	int cmdline;
}optionNode;

