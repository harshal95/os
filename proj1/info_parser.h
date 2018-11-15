/**
 * h file for parsing files of processes and displaying output
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */
char* parseFile(char* filePath, int pos);
char* parseCmdLine(char* filePath);
procOutput* parseInfo(optionNode *optionFlags, char* procPath, char* procName);
procOutput* callParseFunc(optionNode *optionFlags, char* procName);
void printOutputNode(procOutput* trav, optionNode* optionFlags);
