
#include <stdlib.h>
#include <stdio.h>
//#include <sys/types.h>
#include <string.h>
//#include <errno.h>
/* "readdir" etc. are defined here. */
#include <dirent.h>
void compress(const char* tocompress, const char* codebook);
char* retcode(char* tofind, const char* codebook);
//void tokenize(char* pathName);

void compress(const char* tocompress, const char* codebook){
    /*
    open tocompress //its a path
    open codebook

    read the file to compress, tokenize it
    send the token and codebook to a method that finds the string and returns its huffman code
    take this code and append it to the compressedfile
    */
    //FILE* tc = fopen(tocompress);
    int fd= open(tocompress, O_RDONLY);
    int size=lseek(fd, 0, SEEK_END);//find the size of the file
    char* buffer= NULL;
    buffer=(char*) malloc(sizeof(char)*size);
    int bufIndex=0;
    //create the output file
    FILE* towrite = fopen("compressedfile.txt.hcz" , "ab+");
  
    while(read(fd, buffer,1)){
        //check for error
        //find token 
        if(buffer[bufIndex]=='\n' || buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v' || buffer[bufIndex]==' '){
            char* delim=(char*) malloc(sizeof(char)*2); //stores the delim that ends token
            delim[0]=buffer[buffIndex];
            delim[1]='\0';
            buffer[bufIndex]='\0';
            char* currNodeName= (char*) malloc(sizeof(char)*bufIndex+1); //stores token
            strncpy(currNodeName, buffer, buffIndex+1);
            int count =0;
            for(i=0; i<buffIndex+1; i++){//getting the accumulated ascii value of the token
                sum+=currNodeName[i];
                count++; // should be the length of token??
            }
            //might have to malloc, if u do, use count ^ as length 
            char* hcode;
            char* delimcode;
            //find the token in the codebook and return its code
            hcode = retcode(currNodeName, codebook);  
            delimcode = retcode(delim, codebook);

             if(towrite == NULL){
                printf("unable to create file\n");
                return;
            }
            else{
            //appends huffman code for the token and delim
               fputs(hcode, towrite); 
               fputs(delimcode, towrite);
            }

        }
        bufIndex++;
    }    
fclose(towrite);
close(fd);

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
