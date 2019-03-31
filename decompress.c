#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//remember to free everything everwhere
void decompress(char* codebook, char* decompressed){
	//WOULD REALISTICALLY HAVE TO SEARCH FOR CODEBOOK IN DIRECTORY, NEED ALGO FOR THAT
	struct HeapNode* head=treeFromCBook(codebook);
	structHeapNode* ptr=NULL;
	ptr=head;
	int fd=open(codebook, O_RDONLY);
	int id=open(decompressed, O_CREAT | O_WRONLY| O_APPEND);
	char* buffer=NULL;
	(char*) malloc(2*sizeof(char));
	while(read(fd, buffer,1)!=EOF){
		if(ptr==NULL){
			printf("invalid data in compressed file\n");
			ptr=head;//reset
		}
		if(ptr->name==NULL){
			if(buffer[0]=='0'){
				ptr=ptr->left;
			}
			else{
				ptr=ptr->right;
			}
		}
		else{
			int currSize=strlen(ptr->name);
			char* currToken= NULL;
			currToken=(char*)malloc(currSize*sizeof(char));
			currToken=ptr->name;
			write(id, currToken, strlen(currToken));
			ptr=head;//reset ptr to top of stack
		}
	}
	return;
	//call buildtree or whatev its called
	//read from file char by char
	//on a 0, go left in tree, on a 1 go right, at each node check to see if name is null
	//if name isn't null, write that to file
	//if directions steer u to null node, print error message
	}



