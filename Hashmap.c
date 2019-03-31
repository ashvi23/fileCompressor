#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//keep track of the number of words added to hahstable accross all files
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

/*int main(int argc, char** argv){ //FOR TESTING PURPOSES ONLY
	FILE *ip;
	int i=0;
	char* val[11];
	for(i=0; i<10;i++){
	val[i]=(char*)malloc(15);
	}
	struct HashNode* HashTable[10000];
		for(i=0; i<10000; i++){
			HashTable[i]=NULL;
		}
	if(argc<2){
	printf("error");
	return 0;
	}
	ip=fopen(argv[1], "r");
		if(ip==NULL){
		printf("error");
		fclose(ip);
		return 0;
		}
		i=0;
		int z;
		for(z=0; z<11; z++){
			fscanf(ip, "%s", val[i]);
				printf("Hashing %s\n", val[i]);
				addNode(HashTable, &val[i]);
				printf("henlo?\n");
		i++;
		}
	printf("done\n");
	fclose(ip);
	//deallocate hashtable
	for(i=0; i<10000; i++){
		struct HashNode* temp=HashTable[i];
		deallocate(temp);
	}
	return 0;
}*/

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

void addNode(struct HashNode** HashTable,char** string){
	int isDup = seek(HashTable, *string);//syntax??
	if(isDup==0){
		return;	
	}
	else {		
			struct HashNode* HNode=makeHashNode(*string);
			printf("HNode->key: %d\n", HNode->key);
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
			printf("inserted\n");
				return;
		}

}

int seek(struct HashNode** HashTable,char* string){
	
	int absent=1;
	int key=getKey(string);	
	printf("key: %d\n", key);
	struct HashNode *temp=HashTable[key];
		while(temp!=NULL){
			//printf("temp->token: %s\n", temp->token);
			if(strncmp(temp->token, string, strlen(string+1))==0){
			temp->frequency++;
			//printf("duplicate found: %s, frequency: %d\n", temp->token, temp->frequency);
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
