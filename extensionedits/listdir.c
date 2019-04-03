#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include "fileCompressor.h"

int numToks;
struct HashNode* HashTable[10000];


int main (int argc, char**argv){
int i=0;
	for(i=0; i<10000; i++){
			HashTable[i]=NULL;
	}
	int* numToks =0;
 	int flag = argv[1][0] -48;
 	 if(isFile(argv[2])){
       // printf("warning: file sent in\n");
        listdir(flag , argv[2], argv[3]);
    }
  else  if (is_directory(argv[2])) {
        printf("yes\n");
     
      	
        listdir(flag, argv[2], argv[3]);
    }
   
    else{
      printf("no\n"); 
      }
    return 0; 
    }

void listdir(int flag, const char* dirname, const char* codebook){
//printf("");
if(isFile(dirname)){
printf("warning: a file is sent to recurse\n");
      if(flag ==1){
      printf("build\n");
                	//build(1, argv[2]);
                }
                else if(flag == 2){
              //    printf("compre build\n");
			compress(dirname, codebook);
		}
		else if(flag == 3){
		 // printf("decompr\n");
			//decompress(argv[2], argv[3]);
		}
		return;
}
else {
//printf("in here!\n");
	DIR* currdir = opendir(dirname);
    struct dirent *dir_info;
    if(!currdir){
        printf("Error: Cannot open directory\n");
        return;
    }
    // create file paths
    else{
        while((dir_info = readdir(currdir))){

            int dirname_len = strlen(dirname);
            int currdir_len = strlen(dir_info->d_name);
            char* currdir_name = (char*) malloc(currdir_len);
             if(currdir_name == NULL){
            printf("malloc failed in directory walk \n");
            return;
            }
            currdir_name = dir_info->d_name;
            // + '/' + '\0'
            char* path = (char*) malloc(dirname_len+currdir_len+2);
             if(path == NULL){
            printf("malloc failed in directory walk \n");
            return;
            }
            memcpy(path, dirname, dirname_len);
            memcpy(path+dirname_len , "/", 1);
            memcpy(path+dirname_len+1 , currdir_name, currdir_len+1);
            //printf("path %s\n", path);
            //path now contains the current directory path

            // "."
            if(strncmp(dir_info->d_name, ".", sizeof(char))==0){
                continue;
            }
            //is directory
            else if(dir_info->d_type == DT_DIR){
            
                listdir(flag, path, codebook);
            }
            //is file
            else if(dir_info->d_type == DT_REG){
            
				   if(flag ==1){
			  printf("BUILD: %s\n",path );
				        //	build(1, path);
				        }
				        else if(flag == 2){
				          printf("compre build\n");
					compress(path, codebook);
				}
				else if(flag == 3){
				  printf("decompr\n");
					//decompress(path, codebook);
				}
            }
            //error check
            else if(dir_info->d_type == DT_UNKNOWN){
                printf("Error: Directory type is unknown\n");
                return;
            }

        }
	if (flag == 1){
		//struct HeapNode* sortedHeapHead=NULL;
		//sortedHeapHead= hashToArr();
		//struct HeapNode *treehead = buildhTree(sortedHeapHead, treehead);
		//buildCBook(treehead);  
	}
    closedir(currdir);
    }
}
return;
}

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

int isFile(const char *to_read) {

  struct stat s;
  if(stat(to_read, &s) == 0) {

    if(s.st_mode & S_IFDIR) {
      return 0;
    } else if(s.st_mode & S_IFREG) {
      return 1;
    } else {
      printf("Error, not found\n");
      return 0;
    }

  } else {


    return 0;
  }
}
char* getNextToken(const char* filename, int size, int offset){
    if(size ==0){
        return "3";
    }
    int fd= open(filename, O_RDONLY);
    int l= lseek(fd, offset, SEEK_SET);
    char* token= NULL;
    token = (char*)malloc(10*sizeof(char));
    if(token == NULL){
    printf("Malloc failed to get next token\n");
    return "3";
    }
    char* tempchar = (char*) malloc(2 * sizeof(char));
    if(tempchar == NULL){
    printf("Malloc failed in get next token\n");
    return "3";
    }
    int tokcounter =0;
    token[0] = '\0';
    int tokensize = sizeof(token);
    while(read(fd, tempchar,1 )){
        if(sizeof(token)-strlen(token) >=2){
        strcat(token, tempchar);
       }
       else{
       	tokensize = sizeof(token);
        //printf("size of token:%d\n", tokensize);
        char* temp = (char*) realloc(token , (tokensize+10) * sizeof(char) );
        if(temp ==NULL){
            printf("Error: Cannot malloc space for token\n");
            return "3";
        }
        else{
            token = temp;
            strcat(token, tempchar);
        }
        
       }
        
int tempcharint = tempchar[0];
        if( tempchar[0]==' '|| iscntrl( tempchar[0])>0) {
            close(fd);
            return token; 
            break;   
        }
        if(tokcounter+1 == size ){
           free(tempchar);
            close(fd);
            return token;
        }
        tokcounter++;
    }
free(token);
return "3";

}
int compress(char* tocompress, char* codebook){
   
    
    if (isFile(tocompress) == 1){
         int fd= open(tocompress, O_RDONLY);
     	if(fd == -1){
     	printf("Error: Could not open file to compress.\n");
     	return 3;
     	}
        //create the output file
        int clen = strlen(tocompress)+ strlen(".hcz");
        char* filename = (char*) malloc(clen*sizeof(char));
        if(filename == NULL){
        printf("Error mallocing space for filename in Compress\n");
        return 3;}
        int le = strlen(tocompress);
       // int comp = strlen(tocompress);
       // comp = comp - 4;
        strcpy(filename, tocompress);
        //strcat(filename, "1.txt");
        strcat(filename, ".hcz");
        int towrite = open(filename, O_CREAT | O_RDWR | O_APPEND, 00644);
        int size=lseek(fd, 0, SEEK_END);//find the size of the string
        int l= lseek(fd, 0, SEEK_SET);
        char* buffer = (char*) malloc(size);
    read(fd, buffer,size); 
    
    int t= lseek(fd, 0, SEEK_SET);
        int totaltokcounter=0;
        int tokenlength=0;
        //char* token;
        char* delim;
        //char* delimcode;
        //char* tokencode;

        while(totaltokcounter < size){
            if(totaltokcounter+1  == size){
                close(fd);
                free(delim);
                return 1;
            }
            char* token = getNextToken(tocompress, size - totaltokcounter, totaltokcounter);
            if(strcmp(token, "3")==0){
            	printf("Error: Could not get next token.\n");
            	remove(filename);
            	return 3;
            }
            tokenlength = strlen(token);
            totaltokcounter = totaltokcounter + tokenlength;
            //printf("C token:[%s] COUNTER:%d\n\n", token, totaltokcounter);
            delim = (char*) malloc(2 * sizeof(char));
			if(delim == NULL){
			printf("Error: Malloc returned NULL\n");
			remove(filename);
			return 3;
			}
           
            if(tokenlength == 1){
                delim[0] = token[0];
                if(iscntrl(token[tokenlength-1]) > 0 || token[tokenlength-1] == ' '){
                token[tokenlength] = '\0';
               
                }
              
                delim[1] = '\0';
            }

            if(tokenlength >1){
                if(iscntrl(token[tokenlength-1]) >0||token[tokenlength-1] == ' ' ){
                    delim[0] = token[tokenlength-1];
                    delim[1] = '\0';
                    token[tokenlength-1] = '\0';
                }

            }
          
            if(strcmp(token, "3") !=0){
                if(tokenlength > 1){
                    char* tokencode = retcode(token, codebook);
                    if(strcmp(tokencode,"3") !=0 ){
                      
                        write(towrite, tokencode, strlen(tokencode));
                    }
                    else if(strcmp(tokencode,"3") ==0){
                	printf("Error: Token %s doesn't exist in codebook\n", token);
                	remove(filename);
                	return 0;
                	}
                }
        }
     

            if(strcmp(delim, "3") !=0 && (iscntrl(delim[0])>0 || delim[0]==' ') ){
                char* delimcode = retcode(delim, codebook);
                if(strcmp(delimcode,"3") !=0){

                    write(towrite, delimcode, strlen(delimcode));
                }
                else if(strcmp(delimcode,"3") ==0){

                	remove(filename);
                	return 0;
                	}
            }

            if(totaltokcounter+1  == size){
     
                free(delim);
                
                return 1;
            }
        }
       
  //close(cb);
    close(fd);
    }

 else{
        printf("Not a file\n");
        return 3;
    }

    return 0;
}


char* retcode(char* tofind, char* codebook){

//if (isFile(codebook) == 1){
    int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer = (char*) malloc(size);
    read(cb, buffer,size);
    int t= lseek(cb, 0, SEEK_SET);
    char* code;  // holds huffman code
    char* token; //holds word
    int codelength=0;
    int tokenlength=0;
    int codebookread=0;
    int tofindlen = strlen(tofind); //length of the word looking for
    

    // error check to make sure the target's delimiters are removed
    //token is a delimiter len ==1 --> no letters. null [len]/[1] & cntrlcode [len-1] /[0]
    //contains at least 1 letter/non delimiter | delimiter[len-1] null [len]

    //tofind is not a delimiter but has a delimiter at 
    if(strlen(tofind)>1){
    if( (iscntrl(tofind[tofindlen-1]) > 0 || tofind[tofindlen] == ' ') && tofind[tofindlen-1]!= '\0' && tofindlen>1){
        tofind[tofindlen-1] = '\0';
    }
}
char* key = (char*) malloc(3*sizeof(char));
if(key == NULL){
printf("could not malloc space for escape sequence key\n");
return "3";
}
key = getNextToken(codebook, size - codebookread , codebookread);
codebookread = codebookread + (strlen(key));
int cntrl=0;
if(iscntrl(tofind[0]) >0 || tofind[0] == ' '){
	cntrl = 1;
}
     while(codebookread < size){
        code = getNextToken(codebook, size - codebookread , codebookread);
        codelength = strlen(code);
        codebookread = codebookread + codelength;
        //printf("RC code:[%s]\n",code);

        if(strcmp(code, "3")==0){
            printf("Error: Could not retrieve code in HuffmanCodebook at offset:%d \n", codebookread);
            
            return "3";
        }
       
        token = getNextToken( codebook,  size - codebookread, codebookread);    
        tokenlength = strlen(token);
        codebookread = codebookread+tokenlength;
         if(strcmp(token, "3")==0){
            printf("Error: Could not retrieve token in HuffmanCodebook at offset:%d \n", codebookread);
            return "3";
        }
       
        // tokens and code exist. code and token are guaranteed to have one delimiter at end, [len-1]

        if(tokenlength > 1){
            token[tokenlength-1] = '\0' ; // delete delimiter
        }
        else if (tokenlength == 1){
            token[1] = '\0';
        }
        if(codelength > 1){
            code[codelength-1] = '\0' ;
        }
        else if(codelength == 1){
            code[1] = '\0';
        }
          if(cntrl==1){
            if (token[0] == '^' && token[1]=='%'){
            	//char ctok = token[2];
            	char itf = tofind[0];
            	int ctf = itf;
            	
            	if((ctf== 32 && token[2] == 'w')||(ctf== 9 && token[2] == 't')||(ctf== 10 && token[2] == 'n')||(ctf== 0 && token[2] == '0')||(ctf== 11 && token[2] == 'v') ||(ctf== 12 && token[2] == 'f')||ctf== 13 && token[2] == 'r'||ctf== 7 && token[2] == 'a'||ctf== 8 && token[2] == 'b'||ctf== 27 && token[2] == 'e'){
            		return code;
            	}
            		
            	}
            }
        if(strcmp(tofind, token) ==0 ){
            return code;
        }
        if(codebookread+1 == size && strcmp(tofind, token) !=0){
     
            return "3";
        }
        else{
            continue;
        }


        }
        printf("ReturnCode Errors: Code not found\n");
        return "3";

}
//end of compress, retcode, getnexttoken, is_directory, isFile, listdir
//*********************************************************************************************************************************************************


//end of build codebook methods

//*****************************************************************************************************************************************************************************************************************




