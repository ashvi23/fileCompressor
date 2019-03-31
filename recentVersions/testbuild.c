#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>


struct LLNode{
	struct HeapNode* Tree;
	struct LLNode* next;
	};

struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};

struct HeapNode* makeTree(struct HeapNode* head, struct HeapNode* smaller, struct HeapNode* larger);
struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead, int numToks);
struct LLNode* makeNode(struct LLNode* newNode, struct HeapNode* tree);
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void delete(struct LLNode **head, char* Name);
void insert(struct LLNode **head, struct HeapNode **tree, int freq);
void freeNodes(struct LLNode *head);
void printLL(struct LLNode *head);

 

void buildCBook(struct HeapNode* hufftree);//ADD: const char* pathname
void traverseTree(struct HeapNode* root,char path[], int index, int filedesc,  int dir);//dir is direction
void printTree(struct HeapNode* node);

int main(int argc, char** argv){
	struct HeapNode* sortedArr=(struct HeapNode*) malloc(sizeof(struct HeapNode)*6);
	char string1[3];
	string1[0]='\0';
	strcat(string1, "\n");
	//string1[1]='\0';
	sortedArr[0].name=string1;
	sortedArr[0].frequency=11;


	char string2[3];
	string2[0]='\0';
	strcat(string2, "\r");
	//string2[1]='\0';
	sortedArr[1].name=string2;
	sortedArr[1].frequency=12;
	
	char string3[3];
	string3[0]	='\0';
	strcat(string3, "\0");
	//string3[1]='\0';
	sortedArr[2].name=string3;
	sortedArr[2].frequency=13;
	
	char string4[3];
	string4[0]	='\0';
	strcat(string4, "\t");
	//string4[1]='\0';
	sortedArr[3].name=string4;
	sortedArr[3].frequency=14;
	
	char string5[3];
	string5[0]	='\0';
	strcat(string5, " ");
	//string5[1]='\0';
	sortedArr[4].name=string5;
	sortedArr[4].frequency=24;

	char string6[3];
	strcat(string6, "\v");
	//string6[1]='\0';
	sortedArr[5].name=string6;
	sortedArr[5].frequency=26;

	/*char string7[6]="bimbo";
	string7[5]='\0';
	sortedArr[6].name=string7;
	sortedArr[6].frequency=45;
	
	char string8[7]="gabled";
	string8[6]='\0';
	sortedArr[7].name=string8;
	sortedArr[7].frequency=61;*/

	struct HeapNode* head=NULL;
	head=buildhTree(sortedArr, head, 6);
	printTree(head);
	buildCBook(head);
	return 0;
	
}

void printTree(struct HeapNode* node){
	 if (node == NULL) 
          return; 
  
     /* first recur on left child */
     printTree(node->left); 
  
     /* then print the data of node */
     printf("print: _%s_  _%d_ \n", node->name, node->frequency);   
  
     /* now recur on right child */
     printTree(node->right); 
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
			write(filedesc, path, index);
			write(filedesc, "\t",1);
			printf("root->name, %s",root->name );
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
		printf("returning\n");}
		//return;
	}
//***************************************************************************************************************

struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead, int numToks){
	int size=numToks;
	printf("SIZE OF ARR: %d\n", size);
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
	printf("index: %d, numToks:%d \n", index, numToks);
	while(index<=(numToks-1) || LLptr->next!=NULL){
		printf("\n********************index: %d*************************\n", index);
		printf("TREE:\n");
		printTree(LLptr->Tree);
		struct HeapNode* newfirst=NULL;
		struct HeapNode* newsecond=NULL;
		struct HeapNode* heapHead=NULL;
		//LLptr=head;//reset ptr to current head of LL 
		printf("\nhead->Tree->name: %s\n", LLptr->Tree->name);
		int boolean;
		if(index>=size){
			boolean=3;
		}
		else{
			if((sortedArr[index].frequency) <= (LLptr->Tree->frequency)){//if value in arr is smaller than val in LL
				printf("Array is smaller\n");
				if((index<size)){//if there are at least 2 vals in both arr and LL: && LLptr->next!=NULL 
					
					printf("size: %d\n", size);
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
					printf("size: %d\n", size);
					/*if(index>=size){
						printf("LIVING LIKE I'M\n");
						boolean=3;
					}
					else*/ if(LLptr->next==NULL){
						boolean=5;
					}
					else {//protect her from index
						if(LLptr->next->Tree->frequency <= sortedArr[index].frequency){
						printf("DYING\n");
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
					printf("First in LL is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
					printf("name, frequency in second pos: %d, %s\n",( LLptr->Tree->frequency), (LLptr->Tree->name) );
					newfirst=makeHeapNode(newfirst, sortedArr[index].frequency,  sortedArr[index].name);
					//newsecond=makeTree(newsecond, LLptr->Tree->left, LLptr->Tree->right);//wronk
					heapHead=makeTree(heapHead, newfirst, LLptr->Tree);
					insert(&LLptr, &heapHead, heapHead->frequency);
					printf("Before deletion:\n");
					printLL(LLptr);
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					printf("After deletion:\n");
					printLL(LLptr);
					index++;//imcrement through the minHeap
					break;
				case 2:
					printf("Next in array is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
					printf("name, frequency in second pos: %d, %s\n",(sortedArr[index+1].frequency), (sortedArr[index+1].name) );
					newfirst=makeHeapNode(newfirst, sortedArr[index].frequency, sortedArr[index].name);
					newsecond=makeHeapNode(newsecond,sortedArr[index+1].frequency,sortedArr[index+1].name);
					heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);	
					printf("After insertion, no deletion:\n");
					printLL(LLptr);
					index=index+2;//increment through the minHeap
					break;
				case 3:
						if(LLptr->next==NULL){
							break;
						}
				case 4:
					printf("Next LL is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(LLptr->Tree->frequency), LLptr->Tree->name );
					printf("name, frequency in second pos: %d, %s\n",(LLptr->next->Tree->frequency), LLptr->next->Tree->name  );
					
					//newfirst=makeTree(newfirst, LLptr->Tree->left, LLptr->Tree->right);//newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
					//newsecond=makeHeapNode(newsecond, LLptr->next->Tree->left, LLptr->next->Tree->right);//newsecond,LLptr->next->Tree->frequency,LLptr->next->Tree->name);
					heapHead=makeTree(heapHead, LLptr->Tree, LLptr->next->Tree);
					insert(&LLptr, &heapHead, heapHead->frequency);
					printf("\nBefore deletions:\n");
					printLL(LLptr);
					delete(&LLptr->next, (LLptr->next->Tree->name));//delete LLNode used
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					printf("\nAfter deletions:\n");
					printLL(LLptr);
					break;
				case 5:
				case 6:
				printf("Arr is smaller\n");
				printf("name, frequency in first pos: %d, %s\n",(LLptr->Tree->frequency), LLptr->Tree->name );
				printf("name, frequency in second pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
				//newfirst=makeHeapNode(newsecond, LLptr->Tree->left, LLptr->Tree->right);//newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
				newsecond=makeHeapNode(newsecond, sortedArr[index].frequency, sortedArr[index].name);
				heapHead=makeTree(heapHead, LLptr->Tree, newsecond);
				insert(&LLptr, &heapHead, heapHead->frequency);
				index++;
				printf("\nBefore deletion:\n");
				printLL(LLptr);
				delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
				printf("\nAfter deletion:\n");
				printLL(LLptr);	
				printf("\nReturned\n");
				break;


}
			

			/*if((index<size)){//if there are at least 2 vals in both arr and LL 
				printf("LL is smaller\n");
				if(LLptr->next==NULL || sortedArr[index].frequency <= LLptr->next->Tree->frequency){//if next in arr is smaller than LL val	
					 
					printf("Arr is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(LLptr->Tree->frequency), LLptr->Tree->name );
					printf("name, frequency in second pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
					newfirst=makeHeapNode(newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
					newsecond=makeHeapNode(newsecond, sortedArr[index].frequency, sortedArr[index].name);
					heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);
					index++;
					printf("\nBefore deletion:\n");
					printLL(LLptr);
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					printf("\nAfter deletion:\n");
					printLL(LLptr);	
					printf("\nReturned\n");
				}
				else if((index+1>=size)|| LLptr->next->Tree->frequency <= sortedArr[index].frequency){
					printf("Next LL is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(LLptr->Tree->frequency), LLptr->Tree->name );
					printf("name, frequency in second pos: %d, %s\n",(LLptr->next->Tree->frequency), LLptr->next->Tree->name  );
					newfirst=makeHeapNode(newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
					newsecond=makeHeapNode(newsecond,LLptr->next->Tree->frequency,LLptr->next->Tree->name);
					heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);
					printf("\nBefore deletions:\n");
					printLL(LLptr);
					delete(&LLptr->next, (LLptr->next->Tree->name));//delete LLNode used
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					printf("\nAfter deletions:\n");
					printLL(LLptr);
				}
			}*/
		
		
		printf("\nDONE\n");
	}
	printTree(LLptr->Tree);
	return LLptr->Tree;
	
	/*while we arent at the end of sortedArr, or there is more than one node in the LL (i.e. head->next!=null)
	  *Need at least 2 pointers/ index holders. 
			If indexholder1<ptr1,
					if indexholder2 & ptr1->next exist 
					compare indexholder2 and ptr1->next
						if indexholder2 is smaller than ptr1->next, make pair of indexholder1&2
						else
						make pair indexholder1 & ptr1->next
					else if indexholder 2 exists, ptr1->next doesnt
						pair index1, 2
					else if ptr1->next exists, index2 doesn't
							index1, ptr1->next
					else
						index1, ptr combine
			else
				same, but ptr instead of indexholder1
		increment i


	COMBINE:
		use makenode from heapsort file
		create entirely new node for array? but use same node as contained in LL node? can possibly reuse both
		make smaller value left val, larger right val
		make new head node, no name, just frequency, frequency=frequency1+frequency2
		append to end of LL (or use insertion sort if absolutely necessary)*/
}


struct HeapNode* makeTree(struct HeapNode* head, struct HeapNode* smaller, struct HeapNode* larger){
		printf("IN MAKETREE\n\n");
		head=(struct HeapNode*)malloc(1*sizeof(struct HeapNode));
		head->frequency= (smaller->frequency)+(larger->frequency);
		int sumNames=strlen(smaller->name)+strlen(larger->name);
		printf("sumNames:%d\n\n", sumNames);
		char* headName= (char*)malloc(sizeof(char)*(sumNames+1));
		headName[0]='\0';
		strcat(headName,larger->name);
		strcat(headName,smaller->name);
		printf("HELLO\n\n");
		head->name=headName;
		head->left=smaller;
		printf("INSERTING SMALLER\n");
		printTree(smaller);
		head->right=larger;
		if(larger->height>smaller->height){
			head->height=(larger->height)++;
		}
		else{
			head->height=(smaller->height)++;
		}
		printf("INSERTING LARGER\n");
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
				if((*head)->Tree->frequency<freq){//if head is only one in LL and node goes after it
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

					if((*head)->Tree->frequency>freq){//head is greater than val to be inserted
						node->next=(*head);
						(*head)=node;
						break;
					}
					else if(temp1->Tree->frequency<freq && temp2==NULL){
						node->next=NULL;
						temp1->next=node;
							break;
					}
					else if(temp1->Tree->frequency>=freq && temp2==NULL){//if you get to the end of the list
							//printf("entered\n");
							free(node);
							break;
					}
					else if(temp1->Tree->frequency<freq && temp2->Tree->frequency>freq){
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
				//printf("hello hello!\n");
				struct LLNode *temp2=(*head)->next;
				while(temp1!=NULL){
					printf("\nName: %s\n", Name);
					if(strcmp((*head)->Tree->name, Name)==0){
						printf("head to be deleted\n");
						temp1=(*head)->next;
						free(*head);
						(*head)=temp1;
						
						printf("name of head->next:%s\n\n\n", (*head)->Tree->name);
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
					printf("temp1: %s\n", temp1->Tree->name);
					
					//printf("incremented 1\n");
					if(temp2->next!=NULL){
						temp2=temp2->next;
						}
					printf("temp2: %s\n", temp2->Tree->name);
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
	printf("counter: %d\n",counter);//syntax?
	temp1=head;
	while(temp1!=NULL){
		data=temp1->Tree->frequency;
		//printf("data:%d\n", data);
			if(temp1->next==NULL){
				printf("%s, %d\n",temp1->Tree->name, data);
				return;	
			}
			else{ 	
				printf("%s, %d    \t\n",temp1->Tree->name, data);
				temp1=temp1->next;
			}
	}
	printf("\n");
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
	printf("token address: %X\n", token);
	printf("node name address: %X\n", node->name);
	printf("node name: %s\n", node->name);
	node->frequency=freq;
	node->left=NULL;
	node->right=NULL;
	node->height=1;
	return node;
}
