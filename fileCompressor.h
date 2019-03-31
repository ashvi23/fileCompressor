#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

//dirwalk
void listdir(const char* dirname);
void openfiles(const char* filename);
int is_directory(const char *dir);
void writefiles(const char* filename);

//compress
char* getNextToken(char* filename, int size, int offset);
int compress(char* tocompress, char* codebook);
char* retcode(char* tofind, char* codebook);
int isFile(char *to_read) ;

//decompress
void decompress(char* codebook, char* decompressed)
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
void freeNodes(struct LLNode *head);
void printTree(struct HeapNode* node);
void printLL(struct LLNode *head);

//heapsort
void swap(struct HeapNode** arr, int size, int largest);
void heapify(struct HeapNode** arr, int size, int i);
struct HeapNode* hashToArr(struct HashNode** HashTable, int numToks);