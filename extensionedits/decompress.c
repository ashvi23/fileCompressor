#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include "fileCompressor.h"


int isFile(char *to_read) {
  struct stat s;
  if(stat(to_read, &s) == 0) {

    if(s.st_mode & S_IFDIR) {
      return 0;
    } else if(s.st_mode & S_IFREG) {
      return 1;
    } else {
      printf("Error, not found\n");
      return 3;
    }

  } else {


    return 3;
  }
}

char* getNextToken(char* filename, int size, int offset){
    if(size ==0){
        return "3";
    }
    int fd= open(filename, O_RDONLY);
    int l= lseek(fd, offset, SEEK_SET);
    char* token= NULL;
    token = (char*)malloc(10*sizeof(char));
    if(token == NULL){
    printf("Malloc failed to get next token\n");
    return "3";
    }
    char* tempchar = (char*) malloc(2 * sizeof(char));
    if(tempchar == NULL){
    printf("Malloc failed in get next token\n");
    return "3";
    }
    int tokcounter =0;
    token[0] = '\0';
    int tokensize = sizeof(token);
    while(read(fd, tempchar,1 )){
        if(sizeof(token)-strlen(token) >=2){
        strcat(token, tempchar);
       }
       else{
       	tokensize = sizeof(token);
        char* temp = (char*) realloc(token , (tokensize+10) * sizeof(char) );
        if(temp ==NULL){
            printf("Error: Cannot malloc space for token\n");
            return "3";
        }
        else{
            token = temp;
            strcat(token, tempchar);
        }
        
       }
        
int tempcharint = tempchar[0];
        if( tempchar[0]==' '|| iscntrl( tempchar[0])>0) {
            close(fd);
            return token; 
            break;   
        }
        if(tokcounter+1 == size ){
           free(tempchar);
            close(fd);
            return token;
        }
        tokcounter++;
    }
free(token);
return "3";

}
//struct node
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};

//new node
struct HeapNode* makeHeapNode(struct HeapNode* node , int freq, char* token){
	node=(struct HeapNode*)malloc(sizeof(struct HeapNode)*1);
	node->name=token;
	node->frequency=freq;
	node->left=NULL;
	node->right=NULL;
	return node;
}
//insert node
struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index){	
	if(head==NULL){
		struct HeapNode* node=makeHeapNode(node, 0, NULL);
		head=node;
		if(index==strlen(directions)){
			node->name=token;
			
			return node;
		}
	//return node;
	}
	if(directions[index]=='1'){//if(directions[0]='1')
	
	head->right=insertEntry(head->right,directions, token, index+1);//syntax
	}
	else if(directions[index]=='0'){//if(directions[0]='1')
	
	head->left=insertEntry(head->left,directions, token, index+1);
	}
	
	return head;
}
//traverse

struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook){

// is file codebook
	int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer = (char*) malloc(size);
    read(cb, buffer,size);
    free(buffer);
    int t= lseek(cb, 0, SEEK_SET);
    char* code;  // holds huffman code
    char* token; //holds word
    int codelength=0;
    int tokenlength=0;
    int codebookread=0;
   
    
  
	char* key = getNextToken(codebook, size - codebookread , codebookread);
	codebookread = codebookread + (strlen(key));
	
	while(codebookread<size){
		//get bitcode
		code = getNextToken(codebook, size - codebookread , codebookread);
        codelength = strlen(code);
        codebookread = codebookread + codelength;
        
		//get token
		token = getNextToken( codebook,  size - codebookread, codebookread);    
        tokenlength = strlen(token);
        codebookread = codebookread+tokenlength;
		
		//delete delimeter
		if(iscntrl(code[codelength-1]) >0){
			code[codelength-1] = '\0';
		}
		if(iscntrl(token[tokenlength-1]) >0){
			token[tokenlength-1] = '\0';
		}
		
		//insertheapnode
		head = insertEntry(head, code, token, 0);
	}
	return head;
}

void decompress( const char* compressed, char* codebook){
	//is file codebook and compressed
	//create struct head 
	struct HeapNode* head= NULL;
	//call treefromcbook
	//compressed
	head=treeFromCBook( head, codebook);
	struct HeapNode *ptr =NULL;
	ptr=head;
	int td=open(compressed, O_RDWR);//opening compressed file (binary, .hcz)
	int clen = strlen(compressed);
	const char *last_four = &compressed[clen-4];//set a pointer to the last four letters of pathname
	if(strcmp(last_four, ".hcz")!=0 || clen<=4){
		printf("Error: Wrong Filetype for Compression\n");
		}
	if(td == -1){
	printf("Error: Cannot open Compressed file\n");
	}
	int sizec = lseek(td, 0 , SEEK_END);
	int r = lseek(td, 0, SEEK_SET);
	
	clen = clen - 4; // subtract ".hcz" HAVE TO CHECK IF .HCZ
	char* decompressed = (char*) malloc(clen*sizeof(char));
	if(decompressed == NULL){
		printf("malloc error in decompress, couldnt not malloc filepath for decompress\n");
	return;
	}
	decompressed[0] = '\0';
	
	strncpy(decompressed, compressed, clen);
	int fd=open(codebook, O_RDONLY);
	if(fd == -1){
	printf("Error: Cannot open Codebook\n");
	
	}
	//printf("decompressed: %s\n", decompressed);
	int id=open(decompressed, O_CREAT | O_RDWR| O_APPEND, 0644);
	if(id == -1){
	printf("Error: Cannot create decompress\n");
	return;
	}
	
	char* buffer=NULL;
	buffer = (char*) malloc(2*sizeof(char));
	if(buffer == NULL){
		printf("Malloc failed in decompress\n");
		return;
	}
	//while(read(cp, buffer, 1))
	int bytesread=0;
	while(read(td, buffer,1)){
		bytesread= bytesread+1;
		if(ptr==NULL){
			printf("invalid data in compressed file\n");
			ptr=head;//reset
			//return; // ??????
		}
		if(ptr->left!=NULL || ptr->right!=NULL){
		//int val = atoi(ptr->name);
			//printf("ptrname:[%s]  buffer[0]:[%c]\n", ptr->name, buffer[0]);
			if(buffer[0]=='0' ){
				//printf("left\n");
				ptr=ptr->left;
				
			}
			else if(buffer[0]=='1' ){
				//printf("right\n");
				
				ptr=ptr->right;
				
			}
			if(buffer[0]!='1' && buffer[0]!='0' && bytesread == sizec-1){
			break;
			}
			else if (buffer[0]!='1' && buffer[0]!='0' && bytesread<sizec){
				printf("BUFFER: %c\n", buffer[0]);
				printf("Error in Decompress:Compressed file contained a non 1 or 0 number \n");
			return;
			}
		}
		if(ptr->left==NULL && ptr->right==NULL){
		
		
			
			//printf("ptr->name: %s", ptr->name);
			int toklen = strlen(ptr->name);
			int written = write(id, ptr->name, toklen);
			//printf("written:%d", written);
//printf("written: %d", written);
			if(written <1){
				printf("Error Write to file failed in Decompress\n");
				return;
			}
			ptr=head;//reset ptr to top of stack
		}
	}
	freeTree(ptr);
	return;
	close(td);
	close(fd);
	close(id);

}

void freeTree(struct HeapNode* headTree){
	if(head==NULL){
		return;
	}
	freeTree(head->left);
	freeTree(head->right);
	free(head->name);
	free(head);
}

