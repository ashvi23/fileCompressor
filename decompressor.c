#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <math.h>


struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void reverse(char*, int length) ;
char* itoa(int num, char* str, int base) ;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 

struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook);
struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index, int count);
void printTree(struct HeapNode* head);
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void decompress( char* compressed, char* codebook);
//remember to free everything everwhere
char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    int isNegative = 0; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
 
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    //reverse(str, i); 
  printf("STR :%s ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n", str);
    return str; 
} 
/*void reverse(char* str, int length) 
{ 
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
        swap(*(str+start), *(str+end)); 
        
        start++; 
        end--; 
    } 
} */

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& (duplicate from other file)
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};
int isFile(char *to_read) ;


int isFile(char *to_read) {

  struct stat s;
  if(stat(to_read, &s) == 0) {

    if(s.st_mode & S_IFDIR) {
      return 0;
    } else if(s.st_mode & S_IFREG) {
      return 1;
    } else {
      printf("Error, not found\n");
      return -7;
    }

  } else {


    return -6;
  }
}
//struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 

//struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook);

struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook){
printf("CODEBOOK IN TREEFROMC: %s", codebook);
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
    int count =0; // total number of nodes that 
    int startingindex=0;
    int toklen=0;
	while( bufIndex < size){
		printf("buffer at index %d: %c\n", bufIndex, buffer[bufIndex]);
		 if(buffer[bufIndex]=='\n' || buffer[bufIndex]==' '|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v'){
		    char* delim=(char*) malloc(sizeof(char)*2);
		    if(delim == NULL){
		    printf("malloc failed in decompress\n");
		    return;
		    }
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
				printf("Dictionaru entries: 0:%s    1:%s\n\n", dictEntry[0],dictEntry[1]);
				head=insertEntry(head, dictEntry[0], dictEntry[1], 0, count);//call fxn
				count = count + length;
				printf("count : %d\n\n", count);
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

struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index, int count){	
printf("Insert Entry DIRECTIONS: %s\n", directions);
	if(head==NULL){
		int count2 = count;
		printf("hello\n");
		int digits = 0; 
		do { 
		count2 /= 10; 
		digits++; 
		} 
		while (count2 != 0);
		printf("NEW COUNT %d AND DIGITS %d", count, digits);
		char* cstr = (char*) malloc(digits+1 * sizeof(char));
		cstr = itoa(count, cstr, 10);
		printf("cstr %s\n", cstr);
		struct HeapNode* node=makeHeapNode(node, 0, cstr);
		printf("PRINT TREE for dir:%s\n", directions);
		printTree(node);
		printf("PRINTED TREE\n\n");
	
		//printf("the name: %s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", directions);
		count++;
		
		head=node;
		printf("IE: inserted %d\n", node->height);
		if(index==strlen(directions)){
			printf("IE: index: %d\n", index);
			node->name=token;
			printf("nnnode name :%s \n", node->name);
			return node;
		}
	//return node;
	}
	if(directions[index]=='1'){//if(directions[0]='1')
	printf("going right\n");
	count++;
	index++;
	printf("count in right %d\n", count);
	head->right=insertEntry(head->right,directions, token, index, count);//syntax
	}
	if(directions[index]=='0'){//if(directions[0]='1')
	printf("going left\n");
	count++;
	printf("count in left %d\n", count);
	index++;
	head->left=insertEntry(head->left,directions, token, index, count);
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
		printf("Name: %s\n", head->name);
			}
			printf("going left: ");
	printTree(head->left);
	printf("going right: ");
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
void decompress( char* compressed, char* codebook){
  if (isFile(compressed) == 1){
  printf("Compressed: %s\n",compressed);
  printf("Codebook: %s\n",codebook);
	//WOULD REALISTICALLY HAVE TO SEARCH FOR CODEBOOK IN DIRECTORY, NEED ALGO FOR THAT
	struct HeapNode* head= treeFromCBook( head, codebook); // ????needs head
	struct HeapNode* ptr=NULL;
	ptr=head;
	int td=open(compressed, O_RDONLY);
	if(td == -1){
	printf("Error: Cannot open Compressed file\n");
	}
	
	int clen = strlen(compressed);
	clen = clen - 4; // subtract ".hcz"
	char* decompressed = (char*) malloc(clen*sizeof(char));
	decompressed[0] = '\0';
	
	strncpy(decompressed, compressed, clen);
	int fd=open(codebook, O_RDONLY);
	if(fd == -1){
	printf("Error: Cannot open Codebook\n");
	}
	
	int id=open(decompressed, O_CREAT | O_WRONLY| O_APPEND);
	if(id == -1){
	printf("Error: Cannote open Codebook\n");
	}
	
	char* buffer=NULL;
	buffer = (char*) malloc(2*sizeof(char));
	if(buffer == NULL){
		printf("Malloc failed in decompress\n");
		return;
	}

	while(read(fd, buffer,1)){
		if(ptr==NULL){
			printf("invalid data in compressed file\n");
			ptr=head;//reset
			return; // ??????
		}
		if(ptr->name==NULL){
			if(buffer[0]=='0'){
				ptr=ptr->left;
			}
			else if(buffer[0]=='1'){
				ptr=ptr->right;
			}
			else{
			printf("Error in Decompress:Compressed file contained a non 1 or 0 number \n");
			return;
			}
		}
		else{
			int currSize=strlen(ptr->name);
			char* currToken= NULL;
			currToken=(char*)malloc(currSize*sizeof(char));
			if(currToken == NULL){
				printf("Malloc failed in decompress\n");
				return;
			}
			currToken=ptr->name;
			int written  = write(id, currToken, strlen(currToken));
			if(written <1){
				printf("Error Write to file failed in Decompress\n");
				return;
			}
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
	else{
	printf("decompress not given file error\n");
	return;
	}
	}

int main(int argc, char** argv){
	if(argc<2){
		printf("error");
		return 0;
	}
	struct HeapNode* head=NULL;
	head=treeFromCBook(head, argv[2]);
	decompress(argv[1], argv[2]);
	return 0;
}


