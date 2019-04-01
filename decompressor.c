#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <math.h>

//CHECK IF FILE CONTAINS HCZ (return if it doesn't)
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void reverse(char*, int length) ;
char* itoa(int num, char* str, int base) ;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
void traverseTree(struct HeapNode* root,char path[], int index, int dir);
char* getNextToken(char* filename, int size, int offset);
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
printf("CODEBOOK IN TREEFROMC: %s\n", codebook);
	char* dictEntry[2];//stores bitcode, token
	dictEntry[0]=NULL;//bitcode
	dictEntry[1]=NULL;//token
	int fd=open(codebook, O_RDONLY);//open codebook

	if(fd == -1){
     	printf("Error: Could not open codebook.\n");
     	return 0;
}

	int size=lseek(fd, 0, SEEK_END);//find the size of the file (need for getToken)
	printf("filesize: %d\n", size);
	int l= lseek(fd, 0, SEEK_SET);//sets back to beginning of file
	char* buffer= NULL;
	buffer = (char*)malloc(size*sizeof(char));
	//int bufIndex=0;
	//read(fd, buffer,size);
	int count=2;//allows you to assign unique names to heads based on the ranking of their insertion
	int longestpath=0;
	int bytesRead=0;


	while(bytesRead<size){
		buffer=getNextToken(codebook, size-bytesRead, bytesRead);

		printf("\nFINISHED GETTING NEXT TOKEN\n");
		 if(strcmp(buffer, "3")==0){
		    	printf("Error: Could not get next token.\n");
		    	//remove(filename);//???? idk if this should be here
		    	return 0;
		}
		char* delim=(char*) malloc(sizeof(char)*2);//where to hold delims for comparison
		    if(delim == NULL){
		    printf("malloc failed in decompress\n");
		    return;
		    }

		int buffLen=strlen(buffer);
		bytesRead=bytesRead+buffLen;

	    if(buffLen == 1){//just a delimeter
                delim[0] = buffer[0];
		// if is char or if is cntrl token[0]
                if(iscntrl(buffer[buffLen-1]) > 0 || buffer[buffLen-1] == ' '){
                	buffer[buffLen] = '\0';
                //printf("COMPRESS TOKEN:[%s]\n", token);
                }
                //token[tokenlength-1] = '\0';
                delim[1] = '\0';
	}
		
	if(buffLen >1){//if not just a delimiter (must append NULL terminator)
		//printf("NOT JUST A DELIM\n");
                if(iscntrl(buffer[buffLen-1]) >0 || buffer[buffLen-1] == ' ' ){//if not a control character?
                    delim[0] = buffer[buffLen-1];
                    delim[1] = '\0';
                    buffer[buffLen-1] = '\0';
                    //printf("C Token:[%s] \n", buffer);
		}
	}
	//******************done splitting up token/delimiter************************
	if(strcmp(buffer, "^%")!=0){
	if(delim[0]!='\n' && buffLen!=1){
				//printf("binary string set to contents of buffer\n");
				dictEntry[0]=buffer;
					//printf("binary string set to contents of buffer\n");	
			}
			else if(delim[0]=='\n' && buffLen!=1){
				//printf("EXCUSE ME\n");
				dictEntry[1]=buffer;
				int length=strlen(dictEntry[0]);
				//**********************************
				if(longestpath<length){
				longestpath=length;
				}
				//printf("binary in dict: %s\n", dictEntry[0]);
				//printf("length: %d\n", length);
				printf("Dictionaru entries: 0:%s    1:%s\n\n", dictEntry[0],dictEntry[1]);
				head=insertEntry(head, dictEntry[0], dictEntry[1], 0, count);//call fxn
				count = count + length;
				printf("count : %d\n\n", count);
				free(dictEntry[0]);
				free(dictEntry[1]);
				}
		}
		printf("\nBYTES READ: %d, SIZE: %d\n", bytesRead, size);
	}


















    /*int totalcount =0; // total number of nodes that have been read 
    int startingindex=0;
    int toklen=0;



	while( bufIndex < size){
		printf("buffer at index %d: %c\n", bufIndex, buffer[bufIndex]);
		 if(buffer[bufIndex]=='\n' || buffer[bufIndex]==' '|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v'){//UPDATE ME TO BE MORE THOROUGH
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
	}*/



	/*create head of tree
	open code book, read from it line by line, tokenizing the first thing after each newline, for each line take binary token and parse it, but also tokenize value, 
	char by char, if char==0, give current node a left node, else give a right node, once binary token has been parsed (having checked each time if any more vals in binary token), fill final node with name of token*/
	char path[longestpath];
	printf("*************PRIIIIIIIINNNNTTTTTTTTIIIIIIINNNNNNNG**********");
	traverseTree(head, path, 0, -2000  );
	//printTree(head);
	return head;
}

void traverseTree(struct HeapNode* root,char path[], int index, int dir){
		if(root==NULL){
			printf("returning from NULL\n");
			return;
		}
		printf("root->name, root->frequency: %s, %d\n", root->name, root->frequency);
		if(dir==0 || dir==1){
		if(dir==0){
			printf("0\n");
			path[index]='0';
			/*/print 0 to file
			if(write(filedesc, "0", 1)!=1){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			}*/
		}
		else if(dir==1){
			printf("1\n");
			path[index]='1';
			/*/print 1 to file
			if(write(filedesc, "1", 1)!=1){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			}*/
		}
		index++;
		}
		if(root->left == NULL && root->right==NULL){
			int i=0;
			for(i=0; i<index; i++){
				printf("index %d: %c\n", i, path[i]);
			}
			//when you get to the leaf node, print a space and then the name contained in that node
			/*write(filedesc, path, index);
			write(filedesc, "\t",1);
			printf("root->name, %s", );
			int currTokLen=strlen(root->name);
			if(root->name[0]=='\n'){//writing escape characters to file
				if(write(filedesc, "^%n", currTokLen+2)!=(currTokLen+2)){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]== '\t'){
				if(write(filedesc, "^%t", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			} 
			else if(root->name[0]=='\r'){
				if(write(filedesc, "^%r", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\v'){
				if(write(filedesc, "^%v", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\0'){
				if(write(filedesc, "^%0", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\f'){
				if(write(filedesc, "^%f", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\a'){
				if(write(filedesc, "^%a", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\b'){
				if(write(filedesc, "^%b", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\e'){
				if(write(filedesc, "^%b", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]=='\\'){
				if(write(filedesc, "^%\\", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(root->name[0]==' '){
				if(write(filedesc, "^%w", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else{
				int currTokLen=strlen(root->name);
				write(filedesc, root->name, currTokLen);
				}
			write(filedesc, "\n", 1);//write newline*/
		}
		else{
		traverseTree(root->left,path,index, 0);
		traverseTree(root->right,path,index, 1);
		printf("returning\n");}
		//return;
	}




struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index, int count){	
printf("Insert Entry DIRECTIONS: %s\n", directions);
int size=strlen(token);
printf("strlen(token): %d", size);
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
			int toklen= strlen(token);
			node->name=token;
			node->frequency=toklen;//to be able to send strlen to decompress (impossible otherwise)
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
void decompress(const char* compressed,const char* codebook){
  if (isFile(compressed) == 1){
  printf("Compressed: %s\n",compressed);
  printf("Codebook: %s\n",codebook);
	//WOULD REALISTICALLY HAVE TO SEARCH FOR CODEBOOK IN DIRECTORY, NEED ALGO FOR THAT
	
	struct HeapNode* head= NULL;
	head=treeFromCBook( head, codebook); // ????needs head
	struct HeapNode* ptr=NULL;
	ptr=head;
	int td=open(compressed, O_RDWR);//opening compressed file (binary, .hcz)
	int clen = strlen(compressed);
	const char *last_four = &compressed[clen-4];//setting a pointer to the last four letters of the pathname
	if(strcmp(last_four, ".hcz")!=0 || clen<=4){
		printf("Error: Wrong Filetype for Compression\n");
		}
	if(td == -1){
	printf("Error: Cannot open Compressed file\n");
	}
int sc = lseek(td, 0 , SEEK_END);//DELETE ME
int r = lseek(td, 0, SEEK_SET);
char* cbuf[sc];
read(td, cbuf, sc);
printf("cbuf:%s\n", cbuf);
int q = lseek(td, 0, SEEK_SET);
	
	clen = clen - 4; // subtract ".hcz" HAVE TO CHECK IF .HCZ
	char* decompressed = (char*) malloc(clen*sizeof(char));
	if(decompressed == NULL){
	printf("malloc error in decompress\n");
	return;
	}
	decompressed[0] = '\0';
	
	strncpy(decompressed, compressed, clen);
	int fd=open(codebook, O_RDONLY);
	if(fd == -1){
	printf("Error: Cannot open Codebook\n");
	}
	printf("decompressed: %s\n", decompressed);
	int id=open(decompressed, O_CREAT | O_RDWR| O_APPEND);
	if(id == -1){
	printf("Error: Cannot create decompress\n");
	}
	
	char* buffer=NULL;
	buffer = (char*) malloc(2*sizeof(char));
	if(buffer == NULL){
		printf("Malloc failed in decompress\n");
		return;
	}
printf("hello\n");




	while(read(td, buffer,1)){
		if(ptr==NULL){
			printf("invalid data in compressed file\n");
			ptr=head;//reset
			//return; // ??????
		}
		if(ptr->left!=NULL || ptr->right!=NULL){
printf("ptrname:[%s]  buffer[0]:[%c]\n", ptr->name, buffer[0]);
			if(buffer[0]=='0' ){
				printf("left\n");
				ptr=ptr->left;
				
			}
			else if(buffer[0]=='1' ){
				printf("right\n");
				
				ptr=ptr->right;
				
			}
			else if(buffer[0]!='1' && buffer[0]!='0'){
				printf("Error in Decompress:Compressed file contained a non 1 or 0 number \n");
			return;
			}
		}
		if(ptr->left==NULL && ptr->right==NULL){
			
			//printf("ptr->name: %s\n", ptr->name);
			//printf("(*ptr).name: %s\n", (*ptr).name);
			int currSize=ptr->frequency;
			char* currToken= NULL;
			currToken= (char*)malloc(currSize*sizeof(char));
			if(currToken == NULL){
				printf("Malloc failed in decompress\n");
				return;
			}
			memcpy(currToken,(*ptr).name, currSize);//
			currToken=memcpy(currToken, (*ptr).name, currSize);
			printf("currToken: %s\n", currToken);

			int written = write(id, currToken, currSize);
//printf("written: %d", written);
			if(written <1){
				printf("Error Write to file failed in Decompress\n");
				return;
			}
			ptr=head;//reset ptr to top of stack
		}
	}
printf("hey\n");
//int bd= open(decompressed, O_RDONLY);
int m = lseek(id, 0, SEEK_SET);
    int s= lseek(id, 0, SEEK_END);
	int t = lseek(id, 0, SEEK_SET);

    char* buf2= NULL;
buf2= (char*)malloc(s);
read(id, buf2, s);
printf("decompressed file:[%s]\n", buf2);
	return;
	//call buildtree or whatev its called
	//read from file char by char
	//on a 0, go left in tree, on a 1 go right, at each node check to see if name is null
	//if name isn't null, write that to file
	//if directions steer u to null node, print error message
	close(td);
	close(fd);
	close(id);
	}
	else{
	printf("decompress not given file error\n");
	return;
	}
	}
//*************************************************************DUPLICATE
char* getNextToken(char* filename, int size, int offset){
    if(size ==0){
        return "3";
    }
    int fd= open(filename, O_RDONLY);
    int l= lseek(fd, offset, SEEK_SET);
    char* token= NULL;
    token = (char*)malloc(2*sizeof(char));
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
	printf("GNT Tempchar:_%s_\n", tempchar);//comment me out
        strcat(token, tempchar);
        printf("GNT: token before realloc:[%s]\n", token);//comment me out
        tokensize = sizeof(token);
	//printf("tokensize: %d\n", tokensize);
        char* temp = (char*) realloc(token , tokensize+1* sizeof(char) );
        if(temp ==NULL){
            printf("Error: Cannot malloc space for token\n");
            return "3";
        }
        else{
            token = temp;
           // printf("GNT: token after realloc:[%s]\n", token);
        }
int tempcharint = tempchar[0];
        if( tempchar[0]==' '|| iscntrl( tempchar[0])>0) {
            printf("GNT Token:[%s] ",token );
            printf("GNT Delim:_%s_\n", tempchar);
          
            //free(tempchar);
            //free(temp);
            close(fd);
            return token; 
            break;   
        }
        if(tokcounter+1 == size ){
            //free(temp);
           free(tempchar);
            close(fd);
            return token;
        }
        tokcounter++;
    }
free(token);
return "3";

}
//*********************************************************DUPLICATE
int main(int argc, char** argv){
	if(argc<2){
		printf("error");
		return 0;
	}
	struct HeapNode* head=NULL;
	//head=treeFromCBook(head, argv[2]);
	decompress(argv[1], argv[2]);
	return 0;
}



