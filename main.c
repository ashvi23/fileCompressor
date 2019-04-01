#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>



int main(int argc, char** argv){
	if(argc <3 || argc>5){
		printf("Not enough arguments\n");
		return 0;
	}

	int fd; 
	
	if(argc == 3){
		if(strcmp(argv[1], "-b")==0 && isFile(argv[2]) ==1){
			//call build codebook
			build(argv[2]);

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
				recurse(1, path, codebook);
			}
			else{
				error(1);
			}

		}
		// compress 
		if(isFile(argv[2]) && isFile(argv[3])){
			else if(strcmp(argv[1], "-c")==0){
				int n = compress(argv[2], argv[3]);
				if (n == 0){
					error(3);
				}
			}
			// decompress
			else if (strcmp(argv[1], "-d")==0)
			{
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
				recurse(2, path, codebook);
			}
			else{
				error(1);
			}

		}
		else if ((strcmp(argv[1], "-r")==0 && strcmp(argv[2], "-d")==0 )|| (strcmp(argv[1], "-d")==0 && strcmp(argv[2], "-r")==0)){
			if(isDir(argv[3]) && isFile(argv[4]) ){
				recurse(3, path, codebook);
			}
			else{
				error(1);
			}

		}

	}

return 0;
}

//int error=0;
	/*
	1. Improper arg input
	2. 
	*/
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
