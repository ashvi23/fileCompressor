#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>



void parseFile(const char* pathName);

void parseFile(const char* pathName){
    int fd= open(pathName, O_RDONLY);
    int size=lseek(fd, 0, SEEK_END);//find the size of the file
    int l= lseek(fd, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char));
    int bufIndex=0;
    //printf("size %d, l %d \n",size,l );
   // openfiles(pathName);
    read(fd, buffer,size);
    //printf("attempt %s\n", buffer);
    int startingindex=0;
    int toklen=0;
    while( bufIndex < size){
        //check for error
        //printf("hello while loop\n");
        //printf("BUFFER:_%s_%d_ \n", buffer, bufIndex);

        //|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v' || 
        //printf("bufind %c\n",buffer[bufIndex] );
        if(buffer[bufIndex]=='\n' || buffer[bufIndex]==' '|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v'){
            //printf("in if statement\n");
            
            char* delim=(char*) malloc(sizeof(char)*2);
            delim[0]=buffer[bufIndex];
            delim[1]='\0';
            buffer[bufIndex]='\0';
            
            //printf("before currNodeName\n");
           
            toklen = bufIndex-startingindex;
            char* currNodeName= (char*) malloc(sizeof(char)*toklen+1);
            strncpy(currNodeName, buffer+startingindex, toklen);
            currNodeName[toklen+1] = '\0';
            //printf("currNodeName:_%s_\n", currNodeName );
            startingindex = bufIndex+1; //move up starting index

            int i=0;
            int sum=0;
            int delimSum= delimSum+delim[0];
            for(i=0; i< toklen; i++){//getting the accumulated ascii value of the token
                sum+=currNodeName[i];
            }
            //printf("Token:_%s_\n", currNodeName);
            //printf("Delim:_%s_\n",delim );
            
        }
        bufIndex++;
       // printf("index: %d, startingindex %d\n", bufIndex, startingindex );
    }
    //printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //printf("\n\n\n\n\n\n\n");
    return;
}