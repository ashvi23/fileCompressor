#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& (duplicate from other file)
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};

struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 

struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook);
struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index);

int main(int argc, char** argv){
	if(argc<2){
		printf("error");
		return 0;
	}
	struct HeapNode* head=NULL;
	head=treeFromCBook(head, argv[1]);
	return 0;
}


struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook){
	char* dictEntry[2];
	dictEntry[0]=NULL;
	dictEntry[1]=NULL;
	int fd=open(codebook, O_RDONLY);
	int size=lseek(fd, 0, SEEK_END);//find the size of the file
	printf("filesize: %d\n", size);
	int l= lseek(fd, 0, SEEK_SET);
	char* buffer= NULL;
	buffer = (char*)malloc(size*sizeof(char));
	int bufIndex=0;
	read(fd, buffer,size);
    
    int startingindex=0;
    int toklen=0;
	while( bufIndex < size){
		printf("buffer at index %d: %c\n", bufIndex, buffer[bufIndex]);
		 if(buffer[bufIndex]=='\n' || buffer[bufIndex]==' '|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v'){
		    char* delim=(char*) malloc(sizeof(char)*2);
		    delim[0]=buffer[bufIndex];
		printf("_%c_\n", delim[0]);
		    delim[1]='\0';
		    buffer[bufIndex]='\0';//make token complete string
	  
		    toklen = bufIndex-startingindex;
			printf("toklen: %d\n", toklen);
		    char* currNodeName= (char*) malloc(sizeof(char)*toklen+1);
		    strncpy(currNodeName, buffer+startingindex, toklen);
		    currNodeName[toklen] = '\0';
		    startingindex = bufIndex+1; //move up starting index
			if(delim[0]!='\n' && toklen!=0){
				dictEntry[0]=currNodeName;	
			}
			else if(delim[0]=='\n' && toklen!=0){
				printf("EXCUSE ME\n");
				dictEntry[1]=currNodeName;
				int length=strlen(dictEntry[0]);
				printf("binary in dict: %s\n", dictEntry[0]);
				printf("length: %d\n", length);
				head=insertEntry(head, dictEntry[0], dictEntry[1], 0);//call fxn
				
				free(dictEntry[0]);
				free(dictEntry[1]);
				}
		}
		bufIndex++;
	}
	/*create head of tree
	open code book, read from it line by line, tokenizing the first thing after each newline, for each line take binary token and parse it, but also tokenize value, 
	char by char, if char==0, give current node a left node, else give a right node, once binary token has been parsed (having checked each time if any more vals in binary token), fill final node with name of token*/
	
	printTree(head);
	return head;
}

struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index){	
	if(head==NULL){
	printf("hello\n");
	struct HeapNode* node=makeHeapNode(node, 0, NULL);
	head=node;
	printf("inserted %d\n", node->height);
	if(index==strlen(directions)){
		printf("index: %d\n", index);
		node->name=token;
		return node;
	}
	//return node;
	}
	if(directions[index]=='1'){//if(directions[0]='1')
	printf("going right\n");
	index++;
	head->right=insertEntry(head->right,directions, token, index);//syntax
	}
	if(directions[index]=='0'){//if(directions[0]='1')
	printf("going left\n");
	index++;
	head->left=insertEntry(head->left,directions, token, index);
	printf("returned\n");
	}
	printf("index: %d, returning\n", index);
	return head;
}

void printTree(struct HeapNode* head){
	if(head==NULL){
		return;
		}
	if(head->name==NULL){
		printf("NULL\n");
		}
		else{
		printf("%s\n", head->name);
			}
	printTree(head->left);
	printTree(head->right);
	}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token){
	node=(struct HeapNode*)malloc(sizeof(struct HeapNode)*1);
	node->name=token;
	node->frequency=freq;
	node->left=NULL;
	node->right=NULL;
	return node;
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
