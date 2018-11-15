/**
 * h file for reading the /proc directory
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */
#include <dirent.h>
DIR* openProcDirectory();
procNode* insertProcNode(procNode* tail, char* processName);
void accessProcessFile(procNode* tail, int pid);
int isUserProcess(char* filePath);
