#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>



char* getNextToken(char* filename, int size, int offset){
    if(size ==0){
        return "-5";
    }
    int fd= open(filename, O_RDONLY);
    int l= lseek(fd, offset, SEEK_SET);
    char* token= NULL;
    token = (char*)malloc(size*sizeof(char));
    char* tempchar = (char*) malloc(2 * sizeof(char));
    int tokcounter =0;
    token[0] = '\0';
    while(read(fd, tempchar,1 )){
printf("Delim:_%s_\n", tempchar);
        strcat(token, tempchar);

        if( tempchar[0]==' '|| iscntrl( tempchar[0])>0) {
            printf("Delim:_%s_\n", tempchar);
          
            //free(tempchar);
            close(fd);
            return token; 
            break;   
        }
        if(tokcounter == size ){
            free(tempchar);
            return token;
        }
        tokcounter++;
    }
free(token);
return "3";

}

//}

int main (int argc, char**argv){

    //printf("Argv[2] %s\n",  argv[2]);
    //int sf = strlen(argv[2]);
    //char* send =(char*) malloc(sf*sizeof(char));
     //int df = strlen(argv[1]);
    //char* send1 =(char*) malloc(df*sizeof(char));
    //compress(argv[1], argv[2]);
   // getNextToken("  i", 8);
   // retcode("button", argv[1]);
//free(send);
    int size, offset;
    size = 21;
    offset =0;
    char* tok = (char*) malloc(size* sizeof(char));
    while(offset<21){
        char* tok = getNextToken("/Users/ashvis/Documents/GitHub/Asst2/inthis/folder1/folder1.txt", size, offset );
        int len = strlen(tok);
        offset = offset+len;
        size = size - len;
        printf("token:[%s] offset: %d size: %d\n", tok, offset, size );
        if(offset == 21){
            break;
        }
  printf("^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ^ \n");
    }


    return 0; 
    }
//above code about chopping buffer can be called trimbuffer
    /*
    open tocompress //its a path
    open codebook

    read the file to compress, tokenize it
    send the token and codebook to a method that finds the string and returns its huffman code
    take this code and append it to the compressedfile
    */

/*
void compress(char* tocompress, const char* codebook){
    //FILE* tc = fopen(tocompress);
    int fd= open(tocompress, O_RDONLY);
    //create the output file
    char* filename ;
    filename = strcat(tocompress, ".txt.hcz");
    printf("filename %s\n", filename );
    int towrite = open(filename, O_CREAT | O_WRONLY | O_APPEND);
    int size=lseek(fd, 0, SEEK_END);//find the size of the file
    int l= lseek(fd, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char));
    read(fd, buffer,size);
    int toklen=0;
    while(strlen(buffer) > 0){
        char* currNodeName;
        currNodeName = getnexttoken(buffer, size);
        int toklen = strlen(currNodeName);

        char* delim=(char*) malloc(sizeof(char)*2);
        delim[0]=currNodeName[toklen - 2];
        delim[1]='\0';
        currNodeName[toklen-2]='\0';

        memmove(buffer, buffer+toklen-1, strlen(buffer)-tokelen-1);

        char* hcode;
        char* delimcode;
        //find the token in the codebook and return its code
        hcode = retcode(currNodeName, codebook);  
        delimcode = retcode(delim, codebook);

        if(hcode == NULL){
            printf("Error: Huffman Code for the token %s, doesn't exist\n",currNodeName );
            return;
        }
         if(delimcode == NULL){
            printf("Error: Huffman Code for the token %s, doesn't exist\n",delim );
            return;
        }

        else{
            if(towrite == NULL){
                printf("unable to create file\n");
                return;
            }
            else{
            //appends huffman code for the token and delim
                
               write(towrite, hcode, sizeof(hcode)); 
               write(delimcode, towrite, sizeof(delimcode) );
            }
        }
            
      
    }
close(towrite);
close(fd);
}

        
char* retcode(char* tofind, const char* codebook){
    
    int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char)); //holds codebook
    //int bufIndex=0;
    int hcodelen;
    read(cb, buffer, size);
    while(strlen(buffer) > 0){
        char* hcode = (char*) malloc(size * sizeof(char));
        char* token = (char*) malloc(size * sizeof(char));
       
        hcode = getnexttoken(buffer, strlen(buffer));
        hcodelen = strlen(hcode)-1;
        hcode = (char*) realloc(hcode, hcodelen);
        hcode[hcodelen] = '\0';

        token = getnexttoken(buffer, strlen(buffer));
        toklen = strlen(token)-1;
        token = (char*) realloc(token, toklen);
        token[toklen] = '\0';

        if(strcmp(tofind, token) == 0){
            return hcode;
        }
        memmove(buffer, buffer+toklen-1, sizeof(buffer)-tokelen-1);

        if(strlen(buffer) == 0){
            break;
        }

    }

close(codebook);
}
/*
 getNextToken(buffer, size);
        
            
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

            //printf("Token:_%s_\n", currNodeName);
            //printf("Delim:_%s_\n",delim );
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
        bufindex++;


        //copy this into your parent function
void outsideCode(char* token){

    while(buffersize >0)
        getNextToken(buffer);
    //ill have to use memmove instead of all of this or realloc buffer everytime and strcpy newbuffer back into buffer
    int toklen = strlen(token);
    char* delim=(char*) malloc(sizeof(char)*2);
    delim[0]= token[toklen -2];
    delim[1]='\0';
    token[toklen-2]='\0';
    int newsize = size - toklen;
    memmove(buffer, buffer+toklen, sizeof(buffer)-toklen); 





    char* newbuffer =(char*) malloc(sizeof(char)*newsize);
    strcpy(newbuffer, buffer+toklen);
    realloc(buffer, newsize);
    strcpy(buffer, newbuffer)
    free(newbuffer);
    

   // if(cb == NULL){
     //   printf("error cant open codebook\n");
       // return "-1";
    //}
   // char* code, word; //might have to malloc this
    //else{ code from ret code
        char* retcode;
        int codelen;

        char* hcode;
        hcode = getnexttoken(buffer, size);
        int toklen = strlen(hcode);
        //extract delim
        char* delim=(char*) malloc(sizeof(char)*2);
        hcodetoklen-2]='\0';
        


        memmove(buffer, buffer+toklen-1, sizeof(buffer)-tokelen-1);
        size = strlen();


        /*
            hcode = getnexttoken
            //extract delim
            //do all the buffer stuff
            word = getnexttoken
            //extract delim
            if strcmp(word, tofind) == 0 
                ret hcode
            else
                continue
        
        //find the word for loop
      
       
    //}
}
            
*/



