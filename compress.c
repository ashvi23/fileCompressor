
#include <stdlib.h>
#include <stdio.h>
//#include <sys/types.h>
#include <string.h>
//#include <errno.h>
/* "readdir" etc. are defined here. */
#include <dirent.h>
void compress(const char* tocompress, const char* codebook);
char* retcode(char* tofind, const char* codebook);
void tokenize(char* pathName);

void compress(const char* tocompress, const char* codebook){
    /*
    open tocompress 
    open codebook

    read the file to compress, tokenize it
    send the token and codebook to a method that finds the string and returns its huffman code
    take this code and append it to the compressedfile
    */
    FILE* tc = fopen(tocompress);


}


char* retcode(char* tofind, const char* codebook){
    
    FILE* cb = fopen(codebook);
    if(cb == NULL){
        printf("error cant open codebook\n");
        return;
    }
    char* code, word;
    else{
        char* retcode;
        int codelen;

        //find the word for loop
        while(fscanf(cb, "%s %s", &code, &word) != EOF){
            //fscanf(cb, "%s %s", &code, &word);
            if(strcmp(tofind, word) == 0){
                //found word
                return code;
            }


        }
       
    }

close(codebook);
}



void parseFile(char* pathName){
    int fd= open(pathName, O_RDONLY);
    int size=lseek(fd, 0, SEEK_END);//find the size of the file
    char* buffer= NULL;
    buffer=(char*) malloc(sizeof(char)*size);
    int bufIndex=0;
    while(read(fd, buffer,1)){
        //check for error
        if(buffer[bufIndex]=='\n' || buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v' || buffer[bufIndex]==' '){
            buffer[bufIndex]='\0';
            char* currNodeName= (char*) malloc(sizeof(char)*bufIndex+1);
            strncpy(currNodeName, buffer, buffIndex+1);
            int i=0;
            int sum=0;
            for(i=0; i<buffIndex+1; i++){
                sum+=currNodeName[];
            }
            struct AVLNode* newNode=makeAVLNode(, char* string)
            //copy val to binary tree node
            //call fxn to add node to tree
            //add delimeter to tree as well
        }
        bufIndex++;
    }
    
    return;
}


