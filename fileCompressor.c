#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "fileCompressor.h"


int main(int argc, char** argv){
	if(argc <3 || argc>5){
		printf("Not enough arguments\n");
		return 0;
	}

	int fd; 
	
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
		if ((strcmp(argv[1], "-r")==0 && strcmp(argv[2], "-b")==0 )|| (strcmp(argv[1], "-b")==0 && strcmp(argv[2], "-r")==0)){
			if(isDir(argv[3])){
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
		if ((strcmp(argv[1], "-r")==0 && strcmp(argv[2], "-c")==0 )|| (strcmp(argv[1], "-c")==0 && strcmp(argv[2], "-r")==0)){
			if(isDir(argv[3]) && isFile(argv[4])){
				listdir(2, argv[3], argv[4]);
			}
			else{
				error(1);
			}

		}
		else if ((strcmp(argv[1], "-r")==0 && strcmp(argv[2], "-d")==0 )|| (strcmp(argv[1], "-d")==0 && strcmp(argv[2], "-r")==0)){
			if(isDir(argv[3]) && isFile(argv[4]) ){
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
