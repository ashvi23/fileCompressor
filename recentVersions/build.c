#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
//keep track of the number of words added to hahstable accross all files
int numToks;
struct HashNode* HashTable[10000];
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

int isFile(char *to_read) ;
char* getNextToken(char* filename, int size, int offset);

struct HashNode* makeHashNode(char* string);
int getKey(char* string);
void addNode(char** string);//*
int seek(char* string);
void deallocate(struct HashNode* head);
void swap(struct HeapNode** arr, int size, int largest);
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);

void heapify(struct HeapNode** arr, int size, int i);
struct HeapNode* hashToArr();
struct HeapNode* makeTree(struct HeapNode* head, struct HeapNode* smaller, struct HeapNode* larger);
struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead);
struct LLNode* makeNode(struct LLNode* newNode, struct HeapNode* tree);

void delete(struct LLNode **head, char* Name);
void insert(struct LLNode **head, struct HeapNode **tree, int freq);
void freeNodes(struct LLNode *head);
void printLL(struct LLNode *head);
void buildCBook(struct HeapNode* hufftree);//ADD: const char* pathname
void traverseTree(struct HeapNode* root,char path[], int index, int filedesc,  int dir);//dir is direction
void printTree(struct HeapNode* node);
void build(int flag, char* filename);

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
        // finished reading the file and creating the hash table. Create heap
       if(flag ==0 ){
		struct HeapNode* sortedHeapHead=NULL;
		sortedHeapHead= hashToArr();
		struct HeapNode *treehead = buildhTree(sortedHeapHead, treehead);
		buildCBook(treehead);
       	}
	}	
	else{
		printf("error not a file \n");
		return;
	}


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
      return -7;
    }

  } else {


    return -6;
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
	//printf("seek: key: %d, string:[%s]\n", key,string);
	
	struct HashNode *temp= HashTable[key];
		while(temp!=NULL){
			//printf("temp->token: %s\n", temp->token);
			if(strncmp(temp->token, string, strlen(string+1))==0){
			temp->frequency++;
			//printf("duplicate found: %s, frequency: %d\n", temp->token, temp->frequency);
			absent=0;
			}
		temp=temp->next;
		}
		
	//free(temp);
	return absent;
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
		//LLptr=head;//reset ptr to current head of LL 
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
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					index++;//imcrement through the minHeap
					break;
				case 2:
					newfirst=makeHeapNode(newfirst, sortedArr[index].frequency, sortedArr[index].name);
					newsecond=makeHeapNode(newsecond,sortedArr[index+1].frequency,sortedArr[index+1].name);
					heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);	
					
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
					
					delete(&LLptr->next, (LLptr->next->Tree->name));//delete LLNode used
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					
					break;
				case 5:
				case 6:
				//printf("Arr is smaller\n");
				//	printf("case 6\n");
				//printf("name, frequency in first pos: %d, [%s]\n",(LLptr->Tree->frequency), LLptr->Tree->name );
				//printf("name, frequency in second pos: %d, [%s]\n",(sortedArr[index].frequency), (sortedArr[index].name) );
				//newfirst=makeHeapNode(newsecond, LLptr->Tree->left, LLptr->Tree->right);//newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
				newsecond=makeHeapNode(newsecond, sortedArr[index].frequency, sortedArr[index].name);
				heapHead=makeTree(heapHead, LLptr->Tree, newsecond);
				insert(&LLptr, &heapHead, heapHead->frequency);
				index++;
			
				delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
				

				break;

}
			

	}
	printTree(LLptr->Tree);
	return LLptr->Tree;
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
	if(temp1==NULL){
	(*head)=node;
	}
		else if(temp1->next==NULL){
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
							
							//printf("entered\n");
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
					//printf("about to increment\n");
					if(temp1->next!=NULL){
					temp1=temp1->next;
					}
					else{
						return;
					}
					
					//printf("incremented 1\n");
					if(temp2->next!=NULL){
						temp2=temp2->next;
						}
					//printf("incremented 2\n");
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
		printf("0");
		return;
	}
	
	while(temp1!=NULL){
		counter++;
		temp1=temp1->next;
	}
	temp1=head;
	while(temp1!=NULL){
		data=temp1->Tree->frequency;
		//printf("data:%d\n", data);
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

struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token){
	node=(struct HeapNode*)malloc(sizeof(struct HeapNode)*1);
	node->name=token;
	node->frequency=freq;
	node->left=NULL;
	node->right=NULL;
	node->height=1;
	return node;
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
