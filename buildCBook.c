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
			char* delim=(char*) malloc(sizeof(char)*2);
			delim[0]=buffer[buffIndex];
			delim[1]='\0';
			buffer[bufIndex]='\0';
			char* currNodeName= (char*) malloc(sizeof(char)*bufIndex+1);
			strncpy(currNodeName, buffer, buffIndex+1);
			int i=0;
			int sum=0;
			int delimSum+=delim[0];
			for(i=0; i<buffIndex+1; i++){//getting the accumulated ascii value of the token
				sum+=currNodeName[i];
			}
			struct AVLNode* newNode=makeAVLNode(sum, currNodeName);
			struct AVLNode* delimNode=makeAVLNode(delimSum, delim);
			structAVLNode* tree=insert(newNode, sum);
			tree=insert(delimNode, delimSum);//insert delimeter node
			//copy val to binary tree node
			//call fxn to add node to tree
			//add delimeter to tree as well
		}
		bufIndex++;
	}
	
	return;
}
