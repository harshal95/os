#include "build_spec_repr.h"
int flushStr(FILE* fp);
int read_line(FILE* fp, char* s, int line_no);
FILE* openFile(char* filePath);
int parseTarget(char* line, int lineno, char** line_words);
void parseLine(char* line, int i, int lineno, int wordStart, char** line_words);
void print_words(char** line_words);
void read_lines(Node** head, Node** tail, FILE* fp);

