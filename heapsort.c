#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Brought in from HashMap
struct HashNode{
	char* token;
	int key;
	int frequency;
	struct HashNode* next;
};

struct HashNode* makeHashNode(char* string);
int getKey(char* string);
void addNode(struct HashNode** HashTable,char** string);//*
int seek(struct HashNode** HashTable,char* string);
void deallocate(struct HashNode* head);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


void swap(struct HeapNode** arr, int size, int largest);
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void heapify(struct HeapNode** arr, int size, int i);
struct HeapNode* hashToArr(struct HashNode** HashTable, int numToks);

int main(int argc, char** argv){
	int i=0;
	struct HashNode** HashTable=(struct HashNode**)malloc(10*sizeof(struct HashNode*));
		for(i=0; i<10; i++){
			HashTable[i]=NULL;
		}
	char* keeper= (char*)malloc(sizeof(char)*13);
	keeper="beluga";
	addNode(HashTable, &keeper);
	keeper="beluga";
	addNode(HashTable, &keeper);
	keeper="beluga";
	addNode(HashTable, &keeper);
	keeper="rabblerouser";
	//keeper[12]='\0';
	addNode(HashTable, &keeper);
	keeper="rhythm";
	//keeper[6]='\0';
	addNode(HashTable, &keeper);
	keeper="tsigele";
	//keeper[7]='\0';
	addNode(HashTable, &keeper);
	keeper="hintl";
	//keeper[5]='\0';
	addNode(HashTable, &keeper);
	keeper="verve";
	//keeper[5]='\0';
	addNode(HashTable, &keeper);
	keeper="gentle";
	//keeper[6]='\0';
	addNode(HashTable, &keeper);
	keeper="gentle";
	//keeper[6]='\0';
	addNode(HashTable, &keeper);
	keeper="yop";
	//keeper[3]='\0';
	addNode(HashTable, &keeper);
	keeper="poy";
	//keeper[3]='\0';
	addNode(HashTable, &keeper);
	keeper="pyo";
	//keeper[3]='\0';
	addNode(HashTable, &keeper);
	
	struct HeapNode* head=NULL;
	head=hashToArr(HashTable, 10);
	for(i=0; i<10; i++){
		printf("index: %d, %s\n", i, (head++)->name);
	}
	return 0;
}

struct HeapNode* hashToArr(struct HashNode** HashTable, int numToks){
	struct HeapNode* heapArr=(struct HeapNode*)malloc(numToks*sizeof(struct HeapNode));
	int i=0;//keeps track of hashtable index
	int j=0;//keeps track of array index
	for(i=0; i<10; i++){//visit each index of the Hashtable
		if(HashTable[i]!=NULL){
			struct HashNode* ptr=HashTable[i];
			while(ptr!=NULL){//fill in array values
				heapArr[j].name=(ptr)->token;
				printf("adding %s to heapArr at index %d\n", heapArr[j].name, j);
				heapArr[j].frequency=ptr->frequency;
				printf("frequency at %d: %d\n", j, heapArr[j].frequency);
				heapArr[j].left=NULL;
				heapArr[j].right=NULL;
				j++;
				ptr=ptr->next;
			}
		}
	}
	/*make actual tree-heap
	struct HeapNode* root=NULL;
	root=makeHeap(root, heapArr, numToks, 0);
	//root=makeHeapNode(root, heapArr[0]->frequency, heapArr[0]->name);*/
	//***************************************************************************************************
	printf("******************************************************\nBefore First Heapify:\n");
	for(i=0; i<numToks;i++){
		printf("%s at index %d\n", heapArr[i].name, i);
	}
	for (i = (numToks / 2) - 1; i >= 0; i--){ 
        	heapify(&heapArr, numToks, i); 
		}
	printf("******************************************************\nAfter First Heapify:\n");
	for(i=0; i<numToks;i++){
		printf("%s at index %d\n", heapArr[i].name, i);
	}
  
    // One by one extract an element from heap 
    for (i=numToks-1; i>=0; i--) 
    { 
	printf("*********************************************\nBefore Swap:\n");
	int z=0;
	for(z=0; z<numToks;z++){
		printf("%s at index %d\n", heapArr[z].name, z);
	}
        // Move current root to end 
        swap(&heapArr, numToks, i); //(struct HeapNode* arr, int largest)
  	printf("************************************************\nAfter Swap:\n");
	for(z=0; z<numToks;z++){
		printf("%s at index %d\n", heapArr[z].name, z);
	}
        // call max heapify on the reduced heap 
        heapify(&heapArr, i, 0); 
	//***************************************************************************************************
    } 
	return heapArr;
}
//*******************************************************************************
void heapify(struct HeapNode** arr, int size, int i) { 
	printf("SIZE: %d\nI:%d\n", size,  i);
	int sizeArr=(sizeof((**arr))/sizeof((*arr[0])));
	printf("SIZE OF ARR: %d \n", sizeArr);
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
	printf("L:%d\n", l);
    // If left child is larger than root 
    if (l < size && ((*arr)[l].frequency > (*arr)[largest].frequency)){
	printf("L IS LARGEST: %s\n", (*arr)[l].name);
        largest = l; 
  	}
    // If right child is larger than largest so far 
    if (r < size && (*arr)[r].frequency > (*arr)[largest].frequency) {
	printf("R IS LARGEST: %s\n", (*arr)[r].name);
        largest = r; 
  }
    // If largest is not root 
    if (largest != i) { 
        swap(arr, size, largest); 
  
        // Recursively heapify the affected sub-tree 
        heapify(arr, size, largest); 
    } 
}
//******************************************************************************** 
/*struct HeapNode* makeHeap(struct HeapNode* root, struct HeapNode* arr, int size, int index){
	int i;
	if(index<size){
		root=makeHeapNode(root, arr[i]->frequency, arr[i]->name);
		root->left=makeHeap(root->left, arr, size, (2i+1));
		root->right=(root->right, arr, size, (2i+2));
	}
	return root;
}*/
void swap(struct HeapNode** arr, int size, int largest){
	int tempfreq=(*arr)[0].frequency;
	char* tempname=(*arr)[0].name;
	(*arr)[0].frequency=(*arr)[largest].frequency;
	(*arr)[0].name=(*arr)[largest].name;
	(*arr)[largest].frequency=tempfreq;
	(*arr)[largest].name=tempname;
	int z=0;
	//int size=sizeof(arr)/sizeof(arr[0]);
	printf("size: %d\n", size);
	for(z=0; z<size; z++){
		printf("at index %d: %s\n", z, (*arr)[z].name);
	}
	return;
	}

struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token){
	node=(struct HeapNode*)malloc(sizeof(struct HeapNode)*1);
	node->name=token;
	node->frequency=freq;
	node->left=NULL;
	node->right=NULL;
	return node;
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& From HashMap
int getKey(char* string){
	int key=0;
	int sum=0;
	int i;
	for(i=0; i<strlen(string);i++){
		sum=sum+string[i];
		}
	printf("From getKey:ascii sum of %s=%d\n", string, sum);
	key=sum%10;//change me back
	return key;
}

struct HashNode* makeHashNode(char* string){
	struct HashNode* newNode=(struct HashNode*) malloc (1*sizeof(struct HashNode));
	newNode->token=string;
	printf("From makeHashNode: ");
	newNode->key=getKey(string);
	newNode->frequency=1;
	newNode->next=NULL;
	return newNode;
}

void addNode(struct HashNode** HashTable,char** string){
	int isDup = seek(HashTable, *string);//syntax??
	if(isDup==0){
		printf("ISDUP IS ZERO");
		return;	
	}
	else {		
			struct HashNode* HNode=makeHashNode(*string);
			//printf("HNode->key: %d\n", HNode->key);
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
			printf("INSERTED\n");
				return;
		}

}

int seek(struct HashNode** HashTable,char* string){
	
	int absent=1;
	printf("From Seek: ");
	int key=getKey(string);	
	printf("key: %d\n", key);
	struct HashNode *temp=HashTable[key];
		while(temp!=NULL){
			//printf("temp->token: %s\n", temp->token);
			if(strncmp(temp->token, string, strlen(string+1))==0){
			temp->frequency++;
			printf("************************duplicate found: %s, at index: %d, frequency: %d\n", temp->token, key, temp->frequency);
			absent=0;
			}
		temp=temp->next;
		}
	free(temp);
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
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
