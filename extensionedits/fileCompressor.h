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
void deallocate(struct HashNode* head);
//dirwalk
void listdir(int flag, const char* dirname, const char* codebook, const char* parentdir);
void openfiles(char* filename);
int is_directory(const char *dir);
void writefiles(char* filename);

//compress
char* getNextToken( char* filename, int size, int offset);
int compress(char* tocompress, char* codebook);
char* retcode(char* tofind, char* codebook);
int isFile(const char *to_read) ;

//decompress
void decompress(char* codebook, char* decompressed);
char* itoa(int num, char* str) ;

//treefromcbook
struct HeapNode* treeFromCBook(struct HeapNode* head, char* codebook);
struct HeapNode* insertEntry(struct HeapNode* head, char* directions, char* token, int index);

//hashmap.c
struct HashNode* makeHashNode(char* string);
int getKey(char* string);
void addNode(char** string);//*
int seek(char* string);
void deallocate(struct HashNode* head);

//buildhtree
struct HeapNode* makeTree(struct HeapNode* head, struct HeapNode* smaller, struct HeapNode* larger, int* count);
struct HeapNode* buildhTree(struct HeapNode* sortedArr, struct HeapNode* heapHead);
struct LLNode* makeNode(struct LLNode* newNode, struct HeapNode* tree);
struct HeapNode* makeHeapNode(struct HeapNode* node, int freq, char* token);
void deleteLL(struct LLNode **head, char* Name);
void insert(struct LLNode **head, struct HeapNode **tree, int freq);
//void freeNodes(struct LLNode *head);
void freeTree(struct HeapNode* headTree);
void printTree(struct HeapNode* node);
void printLL(struct LLNode *head);
void traverseTree(struct HeapNode* root,char path[], int index, int filedesc, int dir);
void buildCBook(struct HeapNode* hufftree, char* filename, char* parentdir);


//heapsort
void swap(struct HeapNode** arr, int size, int largest);
void heapify(struct HeapNode** arr, int size, int i);
struct HeapNode* hashToArr();
void build(int isrecurse , const char* filename, char* parentdir);

