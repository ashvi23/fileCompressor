#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "fileCompressor.h"
//edited so that whenever recursion is one of the flags, check to see if input is FILE as well as directory
//NEEDS TO ACCOUNT FR HUFFMAN CODEOOK ONLY BEING NAMED THAT 
//NEED ERROR FOR IF YOU GIVE -b A HUFFMAN CODEBOOK
int main(int argc, char** argv){
	printf("please do this");
	if(argc <3 || argc>5){
		printf("Not enough arguments\n");
		return 0;
	}
	
	if(argc == 3){
		if(strcmp(argv[1], "-b")==0 && isFile(argv[2])){
		int i=0;
		for(i=0; i<10000; i++){
			HashTable[i]=NULL;
		}
		int* numToks =0;
			build(0, argv[2]);

		}
		else {
			printf("error on -b\n");
			error(1);
			return 0;
		}

	}

	// possibilities: brp, rbp, cfc, dfc
	if(argc == 4){
		// build recurse, recurse build
		if(strcmp(argv[1], "-b")==0 && (strcmp(argv[1], "-R")!=0)){
			printf("Error: Incorrect number of inputs for -b\n");
			return 0;
		}
		if ((strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-b")==0 )|| (strcmp(argv[1], "-b")==0 && strcmp(argv[2], "-R")==0)){
			if(is_directory(argv[3]) || isFile(argv[3]) ){
				listdir(1, argv[3], NULL, argv[3]);
			}
			else{
				error(1);
			}

		} else if(strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-b")!=0 ){//either calling -R without a command, or calling -R with -c or -d and no codebook/file
			printf("Error: Incorrect arguments to -R\n");
			return 0;
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
		if(strcmp(argv[1], "-b")==0 || strcmp(argv[2], "-b")==0 ){
			printf("Error: Too many parameters to -b\n");
			return 0;
		}
		if ((strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-c")==0 )|| (strcmp(argv[1], "-c")==0 && strcmp(argv[2], "-R")==0)){
			if((is_directory(argv[3]) || isFile(argv[3])) &&  isFile(argv[4])){
				if(strcmp(argv[4], "HuffmanCodeBook")!=0){
					printf("Error: Must include a Huffman Codebook\n");
					return 0;
					}
				listdir(2, argv[3], argv[4], argv[3]);
			}
			else{
				error(1);
			}

		}
		else if ((strcmp(argv[1], "-R")==0 && strcmp(argv[2], "-d")==0 )|| (strcmp(argv[1], "-d")==0 && strcmp(argv[2], "-R")==0)){
			if((is_directory(argv[3])||isFile(argv[3])) && isFile(argv[4])){
					if(strcmp(argv[4], "HuffmanCodeBook")!=0){
					printf("Error: Must include a Huffman Codebook\n");
					return 0;
					}
				listdir(3, argv[3], argv[4],argv[3]);
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


/*int isFile(char *to_read) {

	
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
}*/

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

