#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char** argv){
	struct HeapNode* sortedArr=(struct HeapNode*) malloc(sizeof(struct HeapNode)*8);
	char string1[7]="wraith";
	string1[6]='\0';
	sortedArr[0].name=string1;
	sortedArr[0].frequency=1;


	char string2[7]="gargle";
	string2[6]='\0';
	sortedArr[1].name=string2;
	sortedArr[1].frequency=3;
	
	char string3[10]="hankering";
	string3[9]='\0';
	sortedArr[2].name=string3;
	sortedArr[2].frequency=12;
	
	char string4[9]="nudniker";
	string4[8]='\0';
	sortedArr[3].name=string4;
	sortedArr[3].frequency=13;
	
	char string5[8]="shmates";
	string5[7]='\0';
	sortedArr[4].name=string5;
	sortedArr[4].frequency=14;

	char string6[10]="editorial";
	string6[9]='\0';
	sortedArr[5].name=string6;
	sortedArr[5].frequency=14;

	char string7[6]="bimbo";
	string7[5]='\0';
	sortedArr[6].name=string7;
	sortedArr[6].frequency=45;
	
	char string8[7]="gabled";
	string8[6]='\0';
	sortedArr[7].name=string8;
	sortedArr[7].frequency=61;

	struct HeapNode* head=NULL;
	head=buildhTree(sortedArr, head, 8);
	return 0;
	
}

struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead, int numToks){
	int size=sizeof(sortedArr);
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
		LLptr=head;//reset ptr to current head of LL 
		printf("\nhead->Tree->name: %s\n", LLptr->Tree->name);
		if((sortedArr[index].frequency) <= (LLptr->Tree->frequency)){//if value in arr is smaller than val in LL
			printf("Array is smaller\n");
			if((index<size)){//if there are at least 2 vals in both arr and LL: && LLptr->next!=NULL 
				if(index+1<size && sortedArr[index+1].frequency <= LLptr->Tree->frequency){//if next in arr is smaller than LL val
					printf("Next in array is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
					printf("name, frequency in second pos: %d, %s\n",(sortedArr[index+1].frequency), (sortedArr[index].name) );
					struct HeapNode* newfirst=makeHeapNode(newfirst, sortedArr[index].frequency, sortedArr[index+1].name);
					struct HeapNode* newsecond=makeHeapNode(newsecond,sortedArr[index+1].frequency,sortedArr[index+1].name);
					struct HeapNode* heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);	
					index=index+2;//increment through the minHeap
				}
				else if(index+1>=size || LLptr->Tree->frequency <= sortedArr[index+1].frequency){
					printf("First in LL is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
					printf("name, frequency in second pos: %d, %s\n",(sortedArr[index+1].frequency), (sortedArr[index].name) );
					struct HeapNode* newfirst=makeHeapNode(newfirst, sortedArr[index].frequency, sortedArr[index+1].name);
					struct HeapNode* newsecond=makeHeapNode(newsecond, LLptr->Tree->frequency, LLptr->Tree->name);
					struct HeapNode* heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					index++;//imcrement through the minHeap
				}
			}
		}
		else{//if value in LL is smaller than value in arr
			if((index+1<size)){//if there are at least 2 vals in both arr and LL 
				printf("LL is smaller\n");
				if(LLptr->next==NULL || sortedArr[index].frequency <= LLptr->next->Tree->frequency){//if next in arr is smaller than LL val	
					 
					printf("Arr is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(LLptr->Tree->frequency), LLptr->Tree->name );
					printf("name, frequency in second pos: %d, %s\n",(sortedArr[index].frequency), (sortedArr[index].name) );
					struct HeapNode* newfirst=makeHeapNode(newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
					struct HeapNode* newsecond=makeHeapNode(newsecond, sortedArr[index].frequency, sortedArr[index].name);
					struct HeapNode* heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);
					index++;
					printf("\nBefore deletions:\n");
					printLL(LLptr);
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					printf("\nAfter deletions:\n");
					printLL(LLptr);	
					printf("\nReturned\n");
				}
				else if((index+1>=size)|| LLptr->next->Tree->frequency <= sortedArr[index].frequency){
					printf("Next LL is smaller\n");
					printf("name, frequency in first pos: %d, %s\n",(LLptr->Tree->frequency), LLptr->Tree->name );
					printf("name, frequency in second pos: %d, %s\n",(LLptr->next->Tree->frequency), LLptr->next->Tree->name  );
					struct HeapNode* newfirst=makeHeapNode(newfirst, LLptr->Tree->frequency, LLptr->Tree->name);
					struct HeapNode* newsecond=makeHeapNode(newsecond,LLptr->next->Tree->frequency,LLptr->next->Tree->name);
					struct HeapNode* heapHead=makeTree(heapHead, newfirst, newsecond);
					insert(&LLptr, &heapHead, heapHead->frequency);
					printf("\nBefore deletions:\n");
					printLL(LLptr);
					delete(&LLptr->next, (LLptr->next->Tree->name));//delete LLNode used
					delete(&LLptr, (LLptr->Tree->name));//delete LLNode used
					printf("\nAfter deletions:\n");
					printLL(LLptr);
				}
			}
		}
		
		/*if(head->next==NULL && index>=size){
			break;//done
		}*/
		printf("\nDONE\n");
	}
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
		head->right=larger;
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
						(*head)=(*head)->next;
						free(temp1);
						
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
printf("\nprinting\n");
printf("%s\n", head->Tree->name);
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
	return node;
}
