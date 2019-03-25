#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
void parseFile(char* pathName)

void parseFile(char* pathName){
	int fd= open(pathName, O_RDONLY);
	int size=lseek(fd, 0, SEEK_END);//find the size of the file
	char* buffer= NULL;
	buffer=(char*) malloc(sizeof(char)*size);
	int bufIndex=0;
	while(read(fd, buffer,1)){
		//check for error
		if(buffer[bufIndex]=='\n' || buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v' || buffer[bufIndex]==' '){
			buffer[bufIndex]='\0';
			char* currNodeName= (char*) malloc(sizeof(char)*bufIndex+1);
			strncpy(currNodeName, buffer, buffIndex+1);
			int i=0;
			int sum=0;
			for(i=0; i<buffIndex+1; i++){
				sum+=currNodeName[];
			}
			struct AVLNode* newNode=makeAVLNode(, char* string)
			//copy val to binary tree node
			//call fxn to add node to tree
			//add delimeter to tree as well
		}
		bufIndex++;
	}
	
	return;
}
