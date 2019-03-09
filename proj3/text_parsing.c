#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "build_spec_repr.h"
/**
 * C file for parsing the makefile and creating graphNodes
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */
extern const int BUFSIZE;

/* Function for flushing string exceeding buffer size*/
int flushStr(FILE* fp){
	int c;
	
	//flushing remainder of string until newline or EOF
	while((c = getc(fp))!= '\n' && c!= EOF){
	
	}

	return c;
	
}

/* Function for reading a single line from file*/
int read_line(FILE* fp, char* s, int line_no){
	
	int c = 0;
	int flushLine = 1;
	int firstCharFlag = 0;
	int i = 0;
	while((c = getc(fp))!= '\n' && c!=EOF){
		if(!firstCharFlag){
			if(c == '\t' || isalnum(c)){
				flushLine = 0;
			}else{
				fprintf(stderr, "%d: Non-valid start of line %s\n", line_no, s);
				flushLine = 1;
				c = -1;
				break;
			}
			firstCharFlag = 1;
		}
		if(i >= BUFSIZE - 1){
			fprintf(stderr, "Buffer limit exceeded and so flushing out string\n");
			exit(1);
		}else{
			s[i++] = c;
		}

	}

	if(flushLine == 0){
		s[i] = '\0';
	}

	return c;
}

/* Function for opening a file */
FILE* openFile(char* filePath){
	FILE* fp = fopen(filePath, "r");
	return fp;
}

/* Function for parsing the target object followed by a ':' Ex- 537ps: */
int parseTarget(char* line, int lineno, char** line_words){
	int i = 0;
	int w_ind = 0;
	char* temp_buf = (char*)malloc(BUFSIZE*sizeof(char));
	while(line[i]!=':' && line[i]!='\0'){
		if(line[i] == ' ' || line[i] == '\t'){
			i++;
			continue;
		}
		temp_buf[w_ind++] = line[i++];
	}
	if(line[i] == '\0'){
		fprintf(stderr, "%d: Invalid line: %s\n", lineno, line);
		exit(1);
	}
	i++;
	temp_buf[w_ind] = '\0';
	
	if(strlen(temp_buf)==0){
		fprintf(stderr,"target not defined properly(empty string)");
		exit(1);
	}

	char* res_buf = (char*)malloc((w_ind+1)*sizeof(char));
	strncpy(res_buf, temp_buf, w_ind + 1);

	line_words[0] = res_buf;
	free(temp_buf);
	temp_buf = NULL;
	return i;

}

/* Function for parsing either the line containing dependencies or the line containing commands */
void parseLine(char* line, int i, int wordStart, char** line_words){
	
	int w_ind = 0;
	char* temp_buf = (char*)malloc(BUFSIZE*(sizeof(char)));
	
	while(line[i]!='\0'){
		if(line[i] == ' '|| line[i] == '\t'){
			temp_buf[w_ind] = '\0';
			if(strlen(temp_buf) > 0){
				char* res_buf = (char*)malloc((w_ind+1)*sizeof(char));
				strncpy(res_buf, temp_buf, w_ind + 1);
				line_words[wordStart] = res_buf;
				free(temp_buf);
				temp_buf = (char*)malloc(BUFSIZE*(sizeof(char)));
				wordStart++;
				w_ind = 0;
			}
		}else{
			temp_buf[w_ind++] = line[i];
		}
		i++;
	}
	temp_buf[w_ind] = '\0';
	char* res_buf = (char*)malloc((w_ind+1)*sizeof(char));
	strncpy(res_buf, temp_buf, w_ind + 1);
	line_words[wordStart] = res_buf;
	free(temp_buf);
	temp_buf = NULL;
	wordStart++;
	line_words[wordStart] = NULL;
		
}

/* Function for printing words in a line */
void print_words(char** line_words){
	for(int i = 0; line_words[i]!= NULL; i++){
		printf("%s", line_words[i]);
	}
	
}

/* Function for reading lines in a file */
void read_lines(Node** head,Node** tail, FILE* fp){
	int c;
	int line_no = 1;
	Node* prevNode = NULL;
	while((c = getc(fp))!= EOF){
		
		fseek(fp, -1, SEEK_CUR);		
		char* s = (char*)malloc((BUFSIZE)*sizeof(char));
		s[0] = '\0';
		int lastCh = read_line(fp, s, line_no);
		if(lastCh == EOF || lastCh == -1){
			free(s);
			break;
		}
	
		char** line_words = (char**)malloc(BUFSIZE*(sizeof(char*)));
		if(s!= NULL && strlen(s) > 0){
			int specFlag = 0;
			//char** line_words = (char**)malloc(BUFSIZE*(sizeof(char*)));
			int depStart = 0;
			int wordStart = 0;
			//Parsed words in the line are
			if(s[0]!='\t'){
				depStart = parseTarget(s, line_no, line_words);
				specFlag = 1;
				wordStart = 1;
			}
			parseLine(s, depStart, wordStart, line_words);
			if(specFlag){
				if (searchNodeSpec(head,line_words[0])!=NULL) {
					fprintf(stderr,"%d:Duplicate build spec: %s\n",line_no,s);
					exit(1);				
				}
				prevNode = insertNodeSpec(head, tail, line_words);

			}else{
			//	printf("update command\n");
				if(prevNode!= NULL){
					updateCommandSpec(prevNode, line_words, line_no);
				}else{
					fprintf(stderr,"%d: No build spec available\n", line_no);
					exit(1);  				
				}
			}	
		}
		for(int i = 0; i < BUFSIZE; i++){
			line_words[i] = NULL;
		}
		free(line_words);
		free(s);
		s = NULL;
		line_no++;
	}
}

