#include <stdlib.h>
#include <stdio.h>
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct AVLNode* left;
	struct AVLNode* right;
};

struct HeapNode* hashToArr(struct HashNode* HashTable, int numToks){
	struct HeapNode* heapArr=(struct HeapNode*)malloc(numToks*sizeof(struct HeapNode));
	int i=0;//keeps track of hashtable index
	int j=0;//keeps track of array index
	for(i=0; i<10000; i++){//visit each index of the Hashtable
		if(hashmap[i]!=NULL){
			struct HashNode* ptr=HashTable[i];
			while(ptr!=NULL){//fill in array values
				heapArr[j]->name=HashTable->token;
				heapArr[j]->frequency=HashTable->frequency;
				heapArr[j]->left=NULL;
				heapArr[j]->right=NULL;
				j++;
				ptr=ptr->next;
			}
		}
	}
	//make actual tree-heap
	struct HeapNode* root=NULL;
	root=makeHeap(root, heapArr, numToks, 0);
	//root=makeHeapNode(root, heapArr[0]->frequency, heapArr[0]->name);
	//***************************************************************************************************
	for (i = numToks / 2 - 1; i >= 0; i--) 
        heapify(heapArr, numToks, i); 
  
    // One by one extract an element from heap 
    for (i=numToks-1; i>=0; i--) 
    { 
        // Move current root to end 
        swap(heapArr[0], heapArr[i]); 
  
        // call max heapify on the reduced heap 
        heapify(heapArr, i, 0); 
	//***************************************************************************************************
    } 
	
}
//*******************************************************************************
void heapify(struct HeapNode* arr, int size, int i) 
{ 
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
  
    // If left child is larger than root 
    if (l < size && arr[l] > arr[largest]) 
        largest = l; 
  
    // If right child is larger than largest so far 
    if (r < size && arr[r] > arr[largest]) 
        largest = r; 
  
    // If largest is not root 
    if (largest != i) 
    { 
        swap(arr[i], arr[largest]); 
  
        // Recursively heapify the affected sub-tree 
        heapify(arr, size, largest); 
    } 
}
//******************************************************************************** 
struct HeapNode* makeHeap(struct HeapNode* root, struct HeapNode* arr, int size, int index){
	int i;
	if(index<size){
		root=makeHeapNode(root, arr[i]->frequency, arr[i]->name);
		root->left=makeHeap(root->left, arr, size, (2i+1));
		root->right=(root->right, arr, size, (2i+2));
	}
	return root;
}

struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token){
	node=(struct HeapNode*)malloc(sizeof(struct HeapNode)*1);
	node->name=token;
	node->frequency=freq;
	node->left=NULL;
	node->right=NULL;
	return node;
}
