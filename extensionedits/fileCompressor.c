
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>


struct HashNode{
	char* token;
	int key;
	int frequency;
	struct HashNode* next;
};
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};
struct LLNode{
	struct HeapNode* Tree;
	struct LLNode* next;
	};



struct HashNode* HashTable[10000];
//main
void error(int err);
build();
void deallocate(struct HashNode* head);
//dirwalk
void listdir(int flag, const char* dirname, const char* codebook);
void openfiles(const char* filename);
int is_directory(const char *dir);
void writefiles(const char* filename);

//compress
char* getNextToken(char* filename, int size, int offset);
int compress(char* tocompress, char* codebook);
char* retcode(char* tofind, char* codebook);
int isFile(char *to_read) ;

//decompress
void decompress(char* codebook, char* decompressed);
char* itoa(int num, char* str, int base) ;

//treefromcbook
struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook);
struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index);

//hashmap.c
struct HashNode* makeHashNode(char* string);
int getKey(char* string);
void addNode(struct HashNode** HashTable,char** string);//*
int seek(struct HashNode** HashTable,char* string);
void deallocate(struct HashNode* head);

//buildhtree
struct HeapNode* makeTree(struct HeapNode* head, struct HeapNode* smaller, struct HeapNode* larger);
struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead, int numToks);
struct LLNode* makeNode(struct LLNode* newNode, struct HeapNode* tree);
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void delete(struct LLNode **head, char* Name);
void insert(struct LLNode **head, struct HeapNode **tree, int freq);
//void freeNodes(struct LLNode *head);
void freeTree(struct HeapNode* headTree);
void printTree(struct HeapNode* node);
void printLL(struct LLNode *head);

//heapsort
void swap(struct HeapNode** arr, int size, int largest);
void heapify(struct HeapNode** arr, int size, int i);
struct HeapNode* hashToArr(struct HashNode** HashTable, int numToks);





int main(int argc, char** argv){
	if(argc <3 || argc>5){
		printf("Not enough arguments\n");
		return 0;
	}

	//int fd; 
	
	if(argc == 3){
		if(strcmp(argv[1], "-b")==0 && isFile(argv[2]) ==1){
			//call build codebook
		int i=0;
		for(i=0; i<10000; i++){
			HashTable[i]=NULL;
		}
		int* numToks =0;
			build(0, argv[2]);

		}
		else {
			error(1);
			return 0;
		}

	}
	// possibilities: brp, rbp, cfc, dfc
	if(argc == 4){
		// build recurse, recurse build
		if ((strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-b")==0 )|| (strcmp(argv[1], "-b")==0 && strcmp(argv[2], "-R")==0)){
			if(is_directory(argv[3])){
				listdir(1, argv[3], NULL);
			}
			else{
				error(1);
			}

		}

		// compress 
		else if(isFile(argv[2]) && isFile(argv[3])){
			if(strcmp(argv[1], "-c")==0){
				int n = compress(argv[2], argv[3]);
				if (n == 0){
					error(3);
				}
			}
			// decompress
			else if (strcmp(argv[1], "-d")==0){
				decompress(argv[2], argv[3]);
			}
		}
		else{
			error(1);
		}
	}
	// crpc, rcpc, drpc, rdpc
	if(argc == 5){
		if ((strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-c")==0 )|| (strcmp(argv[1], "-c")==0 && strcmp(argv[2], "-R")==0)){
			if(is_directory(argv[3]) && isFile(argv[4])){
				listdir(2, argv[3], argv[4]);
			}
			else{
				error(1);
			}

		}
		else if ((strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-d")==0 )|| (strcmp(argv[1], "-d")==0 && strcmp(argv[2], "-R")==0)){
			if(is_directory(argv[3]) && isFile(argv[4]) ){
				listdir(3, argv[3], argv[4]);
			}
			else{
				error(1);
			}

		}

	}
	int z=0;
	for(z=0; z<10000; z++){
			struct HashNode* temp=HashTable[z];
			deallocate(temp);
		}

return 0;
}

void error(int err){
	if(err ==1){
		printf("Improper input\n" );
		return;
	}
	
	else if (err ==3)	{
		printf("Cannot compress file \n" );
		return;
	}
}

void deallocate(struct HashNode* head){
	if(head==NULL){
		return;
	}
	else{
		deallocate(head->next);
	}
	free(head);
	return;
}

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
int compress(char* tocompress, char* codebook){
   
    
    if (isFile(tocompress) == 1){
         int fd= open(tocompress, O_RDONLY);
     	if(fd == -1){
     	printf("Error: Could not open file to compress.\n");
     	return 3;
     	}
        //create the output file
        int clen = strlen(tocompress)+ strlen(".hcz");
        char* filename = (char*) malloc(clen*sizeof(char));
        if(filename == NULL){
        printf("Error mallocing space for filename in Compress\n");
        return 3;}
        int le = strlen(tocompress);
       // int comp = strlen(tocompress);
       // comp = comp - 4;
        strcpy(filename, tocompress);
        //strcat(filename, "1.txt");
        strcat(filename, ".hcz");
        int towrite = open(filename, O_CREAT | O_RDWR | O_APPEND, 00644);
        int size=lseek(fd, 0, SEEK_END);//find the size of the string
        int l= lseek(fd, 0, SEEK_SET);
        char* buffer = (char*) malloc(size);
    read(fd, buffer,size); 
    
    int t= lseek(fd, 0, SEEK_SET);
        int totaltokcounter=0;
        int tokenlength=0;
        //char* token;
        char* delim;
        //char* delimcode;
        //char* tokencode;

        while(totaltokcounter < size){
            if(totaltokcounter+1  == size){
                close(fd);
                free(delim);
                return 1;
            }
            char* token = getNextToken(tocompress, size - totaltokcounter, totaltokcounter);
            if(strcmp(token, "3")==0){
            	printf("Error: Could not get next token.\n");
            	remove(filename);
            	return 3;
            }
            tokenlength = strlen(token);
            totaltokcounter = totaltokcounter + tokenlength;
            //printf("C token:[%s] COUNTER:%d\n\n", token, totaltokcounter);
            delim = (char*) malloc(2 * sizeof(char));
			if(delim == NULL){
			printf("Error: Malloc returned NULL\n");
			remove(filename);
			return 3;
			}
           
            if(tokenlength == 1){
                delim[0] = token[0];
                if(iscntrl(token[tokenlength-1]) > 0 || token[tokenlength-1] == ' '){
                token[tokenlength] = '\0';
               
                }
              
                delim[1] = '\0';
            }

            if(tokenlength >1){
                if(iscntrl(token[tokenlength-1]) >0||token[tokenlength-1] == ' ' ){
                    delim[0] = token[tokenlength-1];
                    delim[1] = '\0';
                    token[tokenlength-1] = '\0';
                }

            }
          
            if(strcmp(token, "3") !=0){
                if(tokenlength > 1){
                    char* tokencode = retcode(token, codebook);
                    if(strcmp(tokencode,"3") !=0 ){
                      
                        write(towrite, tokencode, strlen(tokencode));
                    }
                    else if(strcmp(tokencode,"3") ==0){
                	printf("Error: Token %s doesn't exist in codebook\n", token);
                	remove(filename);
                	return 0;
                	}
                }
        }
     

            if(strcmp(delim, "3") !=0 && (iscntrl(delim[0])>0 || delim[0]==' ') ){
                char* delimcode = retcode(delim, codebook);
                if(strcmp(delimcode,"3") !=0){

                    write(towrite, delimcode, strlen(delimcode));
                }
                else if(strcmp(delimcode,"3") ==0){

                	remove(filename);
                	return 0;
                	}
            }

            if(totaltokcounter+1  == size){
     
                free(delim);
                
                return 1;
            }
        }
       
  //close(cb);
    close(fd);
    }

 else{
        printf("Not a file\n");
        return 3;
    }

    return 0;
}


char* retcode(char* tofind, char* codebook){

//if (isFile(codebook) == 1){
    int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer = (char*) malloc(size);
    read(cb, buffer,size);
    int t= lseek(cb, 0, SEEK_SET);
    char* code;  // holds huffman code
    char* token; //holds word
    int codelength=0;
    int tokenlength=0;
    int codebookread=0;
    int tofindlen = strlen(tofind); //length of the word looking for
    

    // error check to make sure the target's delimiters are removed
    //token is a delimiter len ==1 --> no letters. null [len]/[1] & cntrlcode [len-1] /[0]
    //contains at least 1 letter/non delimiter | delimiter[len-1] null [len]

    //tofind is not a delimiter but has a delimiter at 
    if(strlen(tofind)>1){
    if( (iscntrl(tofind[tofindlen-1]) > 0 || tofind[tofindlen] == ' ') && tofind[tofindlen-1]!= '\0' && tofindlen>1){
        tofind[tofindlen-1] = '\0';
    }
}
char* key = (char*) malloc(3*sizeof(char));
if(key == NULL){
printf("could not malloc space for escape sequence key\n");
return "3";
}
key = getNextToken(codebook, size - codebookread , codebookread);
codebookread = codebookread + (strlen(key));
int cntrl=0;
if(iscntrl(tofind[0]) >0 || tofind[0] == ' '){
	cntrl = 1;
}
     while(codebookread < size){
        code = getNextToken(codebook, size - codebookread , codebookread);
        codelength = strlen(code);
        codebookread = codebookread + codelength;
        //printf("RC code:[%s]\n",code);

        if(strcmp(code, "3")==0){
            printf("Error: Could not retrieve code in HuffmanCodebook at offset:%d \n", codebookread);
            
            return "3";
        }
       
        token = getNextToken( codebook,  size - codebookread, codebookread);    
        tokenlength = strlen(token);
        codebookread = codebookread+tokenlength;
         if(strcmp(token, "3")==0){
            printf("Error: Could not retrieve token in HuffmanCodebook at offset:%d \n", codebookread);
            return "3";
        }
       
        // tokens and code exist. code and token are guaranteed to have one delimiter at end, [len-1]

        if(tokenlength > 1){
            token[tokenlength-1] = '\0' ; // delete delimiter
        }
        else if (tokenlength == 1){
            token[1] = '\0';
        }
        if(codelength > 1){
            code[codelength-1] = '\0' ;
        }
        else if(codelength == 1){
            code[1] = '\0';
        }
          if(cntrl==1){
            if (token[0] == '^' && token[1]=='%'){
            	//char ctok = token[2];
            	char itf = tofind[0];
            	int ctf = itf;
            	
            	if((ctf== 32 && token[2] == 'w')||(ctf== 9 && token[2] == 't')||(ctf== 10 && token[2] == 'n')||(ctf== 0 && token[2] == '0')||(ctf== 11 && token[2] == 'v') ||(ctf== 12 && token[2] == 'f')||ctf== 13 && token[2] == 'r'||ctf== 7 && token[2] == 'a'||ctf== 8 && token[2] == 'b'||ctf== 27 && token[2] == 'e'){
            		return code;
            	}
            		
            	}
            }
        if(strcmp(tofind, token) ==0 ){
            return code;
        }
        if(codebookread+1 == size && strcmp(tofind, token) !=0){
     
            return "3";
        }
        else{
            continue;
        }


        }
        printf("ReturnCode Errors: Code not found\n");
        return "3";

}
void build(int flag, char* filename){ //FOR TESTING PURPOSES ONLY
	// get size of file, getnexttoken, send into hashmap 
	if(isFile(filename) == 1){
		int fd = open(filename, O_RDONLY);
		if(fd == -1){
			printf("error\n");
			return;
		}
		int size = lseek(fd, 0, SEEK_END);
		int l = lseek(fd, 0, SEEK_SET);
		int readbytes =0;
		int tokenlength =0;
		int absent = 0;
		char* delim;
		numToks =0;
		while(readbytes<size){
		char* token = getNextToken(filename, size - readbytes, readbytes);
		if(strcmp(token, "3")==0){
			printf("error\n");
			return;
		}
		tokenlength = strlen(token);
		readbytes = readbytes + tokenlength;
		

		delim = (char*) malloc(2 * sizeof(char));
		 if(tokenlength == 1){
                delim[0] = token[0];
                if(iscntrl(token[tokenlength-1]) > 0 || token[tokenlength-1] == ' '){
                	token[tokenlength-1] = '\0';
                }
                delim[1] = '\0';
            }

            if(tokenlength >1){
                if(iscntrl(token[tokenlength-1]) >0||token[tokenlength-1] == ' ' ){
                    delim[0] = token[tokenlength-1];
                    delim[1] = '\0';
                    token[tokenlength-1] = '\0';
                }
            }
            absent =seek(token);
            if(absent == 1){
            	addNode(&token);
            	numToks = numToks+1; //increments by size of pointer 
            
            }
            

            absent =seek(delim);
            if(absent == 1){
            	addNode(&delim);
            	numToks = numToks+1;
            	
            } 
         if(readbytes == size){
         break;
         } 
        } 
        if(flag==1){
        // finished reading the file and creating the hash table. Create heap
        struct HeapNode* sortedHeapHead=NULL;
		sortedHeapHead= hashToArr();
		 
		//heap done. create tree
		
		struct HeapNode *treehead = buildhTree(sortedHeapHead, treehead);

		
		buildCBook(treehead);

		//deallocate(sortedHeapHead); 
		}
	}	
	else{
		printf("error not a file \n");
		return;
	}
freeTree(treehead);

}

int getKey(char* string){
	int key=0;
	int sum=0;
	int i;
	for(i=0; i<strlen(string);i++){
		sum=sum+string[i];
		}
	key=sum%10000;
	return key;
}

struct HashNode* makeHashNode(char* string){
	struct HashNode* newNode=(struct HashNode*) malloc (1*sizeof(struct HashNode));
	newNode->token=string;
	newNode->key=getKey(string);
	newNode->frequency=1;
	newNode->next=NULL;
	return newNode;
}

void addNode(char** string){
	int isDup = seek( *string);//syntax??
	if(isDup==0){
		return;	
	}
	else {		
			struct HashNode* HNode=makeHashNode(*string);
		
			if(HashTable[HNode->key]==NULL){
				HashTable[HNode->key]=HNode;
			}
			else{
				struct HashNode* ptr=HashTable[HNode->key];	
					while(ptr->next!=NULL){
						ptr=ptr->next;
				}

				ptr->next=HNode;
			}
				return;
		}

}

int seek(char* string){
	
	int absent=1;
	int key=getKey(string);	
	
	struct HashNode *temp= HashTable[key];
		while(temp!=NULL){
			if(strncmp(temp->token, string, strlen(string+1))==0){
			temp->frequency++;
			absent=0;
			}
		temp=temp->next;
		}
		
	//free(temp);
	return absent;
}

struct HeapNode* hashToArr(){
	struct HeapNode* heapArr=(struct HeapNode*)malloc(numToks*sizeof(struct HeapNode));
	int i=0;//keeps track of hashtable index
	int j=0;//keeps track of array index
	for(i=0; i<10000; i++){//visit each index of the Hashtable
		if(HashTable[i]!=NULL){
			struct HashNode* ptr=HashTable[i];
			while(ptr!=NULL){//fill in array values
				heapArr[j].name=(ptr)->token;
				
				heapArr[j].frequency=ptr->frequency;
				heapArr[j].left=NULL;
				heapArr[j].right=NULL;
				j++;
				ptr=ptr->next;
			}
		}
	}

	for (i = (numToks / 2) - 1; i >= 0; i--){ 
        	heapify(&heapArr, numToks, i); 
		}
    // One by one extract an element from heap 
    for (i=numToks-1; i>=0; i--) 
    { 
	
        // Move current root to end 
        swap(&heapArr, numToks, i); //(struct HeapNode* arr, int largest)
	
        // call max heapify on the reduced heap 
        heapify(&heapArr, i, 0); 
	//***************************************************************************************************
    } 

	return heapArr;
}

void heapify(struct HeapNode** arr, int size, int i) { 
	
	int sizeArr=(sizeof((**arr))/sizeof((*arr[0])));
	
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
	
    // If left child is larger than root 
    if (l < size && ((*arr)[l].frequency > (*arr)[largest].frequency)){
	
        largest = l; 
  	}
    // If right child is larger than largest so far 
    if (r < size && (*arr)[r].frequency > (*arr)[largest].frequency) {
	
        largest = r; 
  }
    // If largest is not root 
    if (largest != i) { 
        swap(arr, size, largest); 
  
        // Recursively heapify the affected sub-tree 
        heapify(arr, size, largest); 
    } 
}
void swap(struct HeapNode** arr, int size, int largest){
	int tempfreq=(*arr)[0].frequency;
	char* tempname=(*arr)[0].name;
	(*arr)[0].frequency=(*arr)[largest].frequency;
	(*arr)[0].name=(*arr)[largest].name;
	(*arr)[largest].frequency=tempfreq;
	(*arr)[largest].name=tempname;

	//int size=sizeof(arr)/sizeof(arr[0]);

	return;
	}
	
struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead){
	int size=numToks;
	struct LLNode* LLptr=NULL;
	
	if(numToks==1){
		struct HeapNode* holdNode=makeHeapNode(holdNode, sortedArr[0].frequency, sortedArr[0].name);
		struct HeapNode* head=makeHeapNode(head, sortedArr[0].frequency, NULL);
		head->right=holdNode;
		return head;
	}

	struct LLNode* head=NULL;//setting up LL to have one node, the first tree in it
	struct HeapNode* first=makeHeapNode(first, sortedArr[0].frequency, sortedArr[0].name);
	struct HeapNode* second=makeHeapNode(second,sortedArr[1].frequency, sortedArr[1].name);
	//struct HeapNode*
	int index=2; //took first two vals in arr, now at index 2
	heapHead=makeTree(heapHead, first, second);
	insert(&head, &heapHead, heapHead->frequency);
	LLptr=head;
	while(index<=(numToks-1) || LLptr->next!=NULL){
		
		struct HeapNode* newfirst=NULL;
		struct HeapNode* newsecond=NULL;
		struct HeapNode* heapHead=NULL;
		int boolean;
		if(index>=size){
			boolean=3;
		}
		else{
		
			if((sortedArr[index].frequency) <= (LLptr->Tree->frequency)){//if value in arr is smaller than val in LL
			
				if((index<size)){
					
					if(index+1>=size){
						boolean=0;
					}
					else {//protect her from index+1
						if(LLptr->Tree->frequency <= sortedArr[index+1].frequency){
						boolean=1;
						}
						else if(index+1<size && sortedArr[index+1].frequency <= LLptr->Tree->frequency){
						boolean=2;
							}
						}
					}
				}
			else if((LLptr->Tree->frequency)<= (sortedArr[index].frequency)){//if value in LL is smaller than value in arr
				
					if(LLptr->next==NULL){
						boolean=5;
					}
					else {//protect her from index
						if(LLptr->next->Tree->frequency <= sortedArr[index].frequency){
						
						boolean=4;
						}
						else if(index<size && sortedArr[index].frequency <= LLptr->next->Tree->frequency){
						boolean=6;
						}
					}
					}
			
				}
				switch(boolean){
				case 0:
				case 1:
					
					newfirst=makeHeapNode(newfirst, sortedArr[index].frequency,  sortedArr[index].name);
					//newsecond=makeTree(newsecond, LLptr->Tree->left, LLptr->Tree->right);//wronk
					heapHead=makeTree(heapHead, newfirst, LLptr->Tree);
					insert(&LLptr, &heapHead, heapHead->frequency);
				
					//printLL(LLptr);
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					
					//printLL(LLptr);
					index++;//imcrement through the minHeap
					break;
				case 2:
					newfirst=makeHeapNode(newfirst, sortedArr[index].frequency, sortedArr[index].name);
					newsecond=makeHeapNode(newsecond,sortedArr[index+1].frequency,sortedArr[index+1].name);
					heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);	
				
					//printLL(LLptr);
					index=index+2;//increment through the minHeap
					break;
				case 3:
						if(LLptr->next==NULL){
							break;
						}
				case 4:
					
					//newfirst=makeTree(newfirst, LLptr->Tree->left, LLptr->Tree->right);//newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
					//newsecond=makeHeapNode(newsecond, LLptr->next->Tree->left, LLptr->next->Tree->right);//newsecond,LLptr->next->Tree->frequency,LLptr->next->Tree->name);
					heapHead=makeTree(heapHead, LLptr->Tree, LLptr->next->Tree);
					insert(&LLptr, &heapHead, heapHead->frequency);
					
					//printLL(LLptr);
					delete(&LLptr->next, (LLptr->next->Tree->name));//delete LLNode used
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
				
					//printLL(LLptr);
					break;
				case 5:
				case 6:
				//newfirst=makeHeapNode(newsecond, LLptr->Tree->left, LLptr->Tree->right);//newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
				newsecond=makeHeapNode(newsecond, sortedArr[index].frequency, sortedArr[index].name);
				heapHead=makeTree(heapHead, LLptr->Tree, newsecond);
				insert(&LLptr, &heapHead, heapHead->frequency);
				index++;

				//printLL(LLptr);
				delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
				
				//printLL(LLptr);	
			
				break;
				


}
				
	}
	//printTree(LLptr->Tree);
	return LLptr->Tree;
freeLL(LLptr);
}
void printTree(struct HeapNode* node){
	 if (node == NULL) 
          return; 
   
     printTree(node->left); 
 
  
     /* now recur on right child */
     printTree(node->right); 
}


struct HeapNode* makeTree(struct HeapNode* head, struct HeapNode* smaller, struct HeapNode* larger){
		
		head=(struct HeapNode*)malloc(1*sizeof(struct HeapNode));
		head->frequency= (smaller->frequency)+(larger->frequency);
		int sumNames=strlen(smaller->name)+strlen(larger->name);

		char* headName= (char*)malloc(sizeof(char)*(sumNames+1));
		headName[0]='\0';
		strcat(headName,larger->name);
		strcat(headName,smaller->name);
		
		head->name=headName;
		head->left=smaller;
		printTree(smaller);
		head->right=larger;
		if(larger->height>smaller->height){
			head->height=(larger->height)++;
		}
		else{
			head->height=(smaller->height)++;
		}
		printTree(larger);
		return head;
		}

struct LLNode* makeNode(struct LLNode* newNode, struct HeapNode* tree){
	newNode=(struct LLNode*)malloc(1*sizeof(struct LLNode));
	newNode->Tree=tree;
	newNode->next=NULL;
	return newNode;
	}

void insert(struct LLNode **head, struct HeapNode** tree, int freq){
struct LLNode *temp1=(*head);
struct LLNode *node= NULL;
	node=makeNode(node, (*tree));
	//printf("check\n");	
	if(temp1==NULL){
	//printf("fill head\n");
	(*head)=node;
	//printf("head filled\n");
	}
		else if(temp1->next==NULL){
			
				//printf("more than head\n");
				if((*head)->Tree->frequency<=freq){//if head is only one in LL and node goes after it
					node->next= NULL;
					(*head)->next=node;
				}
				else if((*head)->Tree->frequency>freq){//head is greater than val to be inserted
					node->next=(*head);
					(*head)=node;
				}		
			}
			
			else if(temp1->next!=NULL){
				//printf("temp1->next isn't null\n");
				struct LLNode* temp2=(*head)->next;
				while(temp1!=NULL){//while temp1 hasn't iterated off of the linked list
				
					if((*head)->Tree->frequency>=freq){//head is greater than val to be inserted
						
						node->next=(*head);
						(*head)=node;
						break;
					}
					else if(temp1->Tree->frequency<=freq && temp2==NULL){
						
						node->next=NULL;
						temp1->next=node;
							break;
					}
					else if(temp1->Tree->frequency>=freq && temp2==NULL){//if you get to the end of the list
							
							free(node);
							break;
					}
					else if(temp1->Tree->frequency<=freq && temp2->Tree->frequency>=freq){
						
						//if node belongs between head & head->next
							if(temp1==(*head)){//hopefully this is the right way to deal w inserting a new head->next
								temp1->next=node;
								node->next=temp2;
								(*head)->next=node;
								break;	
							}
							else{//middle of array, not dealing with head
								node->next=temp2;
								temp1->next=node;
								break;
							}
					
						}
					//iterate to next
					temp1= temp1->next;
					temp2=temp2->next;
					}
				}	
				
	return;	//implicit that you do nothing if temp1==data	
	}
//to delete a node in the LL
void delete(struct LLNode **head, char* Name){
	struct LLNode *temp1=(*head);
		if((*head)==NULL){
			//printf("hello 2!\n");
			return;
		}
		else if((*head)->next==NULL && strcmp((*head)->Tree->name,Name)==0){
				(*head)=(*head)->next;
				free(temp1);
			}
			else if((*head)->next!=NULL){
				struct LLNode *temp2=(*head)->next;
				while(temp1!=NULL){
					if(strcmp((*head)->Tree->name, Name)==0){
						temp1=(*head)->next;
						free(*head);
						(*head)=temp1;
						
						return;
						}
					else if(temp2==NULL){
						break;
					}
					else if(strcmp(temp2->Tree->name,Name)==0){
						//printf("second node to be deleted\n");
						//hopefully this also deletes duplicates w/in the LL
						if(temp2==(*head)->next){
							//printf("delete head->next\n");
							temp1->next=temp2->next;//deleting the node after head
							(*head)->next=temp1->next;
							temp2->next=temp1->next;
						}
						else{
							//printf("deleted\n");
							temp1->next=temp2->next;// all other cases
							free(temp2);
							return;
						}
		
					}
					if(temp1->next!=NULL){
					temp1=temp1->next;
					}
					else{
						return;
					}
					
					if(temp2->next!=NULL){
						temp2=temp2->next;
						}
					
				}
				//deal w/ the case where the value doesnt appear by doing nothing?
			}
			
	 //else if(temp1->value==*data){//might be redundant
		//(*head)==NULL;
	//}
	return;
}

void printLL(struct LLNode *head){
int counter=0;
int data;
//printf("%s\n", head->Tree->name);
struct LLNode *temp1=head;
	if(temp1==NULL){
	//	printf("0");
		return;
	}
	
	while(temp1!=NULL){
		counter++;
		temp1=temp1->next;
	}
	temp1=head;
	while(temp1!=NULL){
		data=temp1->Tree->frequency;
			if(temp1->next==NULL){
				return;	
			}
			else{ 	
				temp1=temp1->next;
			}
	}
	return;	
	
}

void freeNodes(struct LLNode *head){
	if(head==NULL){
		return;	
	}
	else{
	freeNodes(head->next);
	}
	//call method that frees HeapNode trees for head->Tree********************
	free(head);
	return;
}


void buildCBook(struct HeapNode* hufftree){//ADD:  const char* pathname
		int fd=open("./HuffmanCodebook", O_WRONLY | O_CREAT, 00644);
		//traverse through Huffman tree (inorder), printing each time a leaf is found(i.e. leftchild/rightchild ==null or ig if name != NULL), print on each traversal downward.
		if(write(fd, "^%\n", 3)!=3){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			} 
		int pathArrSize=hufftree->height;
		char paths[pathArrSize];
		traverseTree(hufftree,paths,0, fd, -2000);
		int success=close(fd);
		if(success!=0){
		printf("problem closing file descriptor\n");
		}
		return;
	}

void traverseTree(struct HeapNode* root,char path[], int index, int filedesc, int dir){
		if(root==NULL){
			printf("returning from NULL\n");
			return;
		}
		
		if(dir==0 || dir==1){
		if(dir==0){
			path[index]='0';
			/*/print 0 to file
			if(write(filedesc, "0", 1)!=1){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			}*/
		}
		else if(dir==1){
			
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
				
			}
			//when you get to the leaf node, print a space and then the name contained in that node
			write(filedesc, path, index);
			write(filedesc, "\t",1);
			
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
			write(filedesc, "\n", 1);//write newline
		}
		else{
		traverseTree(root->left,path,index,filedesc, 0);
		traverseTree(root->right,path,index,filedesc, 1);
		
		//return;
	}
	}

void listdir(int flag, const char* dirname, const char* codebook){
	DIR* currdir = opendir(dirname);
    struct dirent *dir_info;
    if(!currdir){
        printf("Error: Cannot open directory\n");
        return;
    }
    // create file paths
    else{
        while((dir_info = readdir(currdir))){

            int dirname_len = strlen(dirname);
            int currdir_len = strlen(dir_info->d_name);
            char* currdir_name = (char*) malloc(currdir_len);
             if(currdir_name == NULL){
            printf("malloc failed in directory walk \n");
            return;
            }
            currdir_name = dir_info->d_name;
            // + '/' + '\0'
            char* path = (char*) malloc(dirname_len+currdir_len+2);
             if(path == NULL){
            printf("malloc failed in directory walk \n");
            return;
            }
            memcpy(path, dirname, dirname_len);
            memcpy(path+dirname_len , "/", 1);
            memcpy(path+dirname_len+1 , currdir_name, currdir_len+1);
            //path now contains the current directory path

            // "."
            if(strncmp(dir_info->d_name, ".", sizeof(char))==0){
                continue;
            }
            //is directory
            else if(dir_info->d_type == DT_DIR){
                listdir(flag, path, codebook);
            }
            //is file
            else if(dir_info->d_type == DT_REG){
                if(flag ==1){
                	build(1, path);
                }
                else if(flag == 2){
			compress(path, codebook);
		}
		else if(flag == 3){
			decompress(path, codebook);
		}
            }
            //error check
            else if(dir_info->d_type == DT_UNKNOWN){
                printf("Error: Directory type is unknown\n");
                return;
            }

        }
	if (flag == 1){
		struct HeapNode* sortedHeapHead=NULL;
		sortedHeapHead= hashToArr();
		struct HeapNode *treehead = buildhTree(sortedHeapHead, treehead);
		buildCBook(treehead);  
	}
    closedir(currdir);
    }

}

int is_directory(const char *directory) {
    DIR *direc = opendir(directory);
    if (direc) {
        closedir(direc);
        return 1;
    }
    else {
        return 0;
    }
}



