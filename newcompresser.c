#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

//void listdir(const char* dirname, const char* codebook);
//void openfiles(const char* filename);
//int is_directory(const char *dir);
//void writefiles( char* filename);

char* getNextToken(char* buffer, int size);
void compress(char* tocompress, char* codebook);
char* retcode(char* tofind, char* codebook);
int isFile(char *to_read) ;


int isFile(char *to_read) {

  struct stat s;
  if(stat(to_read, &s) == 0) {

    if(s.st_mode & S_IFDIR) {
      return 0;
    } else if(s.st_mode & S_IFREG) {
      return 1;
    } else {
      printf("Error, not found\n");
      return -1;
    }

  } else {


    return -1;
  }
}
char* getNextToken(char* buffer, int size){
    //printf("buffer %s\n", buffer);
    printf("sinze:%d\n", size);
    int bufIndex=0;
    char* token = (char*) malloc(size+1* sizeof(char));
    char* tempchar = (char*) malloc(2* sizeof(char));
    token[0] = '\0';
    while(bufIndex < size){
        tempchar[0] = buffer[bufIndex];
        tempchar[1] = '\0';
        strcat(token, tempchar);
        printf("Token: %s\n", token);
        printf("index:%d\n", bufIndex);

        if(buffer[bufIndex]=='\n' || buffer[bufIndex]==' '|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v'){
            printf("Delim:_%s_\n", tempchar);
            //free(tempchar);
            return token; 
            break;   
        }
        //last token
        if(bufIndex == size-1){
            free(tempchar);
            return token;
            break;
        }

        bufIndex++;
    }
    free(token);
    return "Error: Can't tokenize"; 
}
void compress(char* tocompress, char* codebook){
    if (isFile(tocompress) == 1){
         int fd= open(tocompress, O_RDONLY);
        //create the output file
        char* filename ;
        filename = strcat(tocompress, ".hcz");
        printf("filename %s\n", filename );
        int towrite = open(filename, O_CREAT | O_WRONLY | O_APPEND);
        int size=lseek(fd, 0, SEEK_END);//find the size of the string
        int l= lseek(fd, 0, SEEK_SET);
        char* buffer= NULL;
        buffer = (char*)malloc(size*sizeof(char));
        read(fd, buffer,size);
printf("86\n\n\n");
        int totaltokcounter=0;
        int tokenlength=0;
        char* token;
        char* delim;
        char* delimcode;
        char* tokencode;
        printf("111\n\n\n");
        
        printf("94\n\n\n");
        while(totaltokcounter < size){
            token = getNextToken(buffer+ totaltokcounter, size - totaltokcounter);
            printf("IN COMPRESS \n\n");
            tokenlength = strlen(token);
            totaltokcounter = totaltokcounter + tokenlength;
            printf("99\n\n\n");
            delim = (char*) malloc(2 * sizeof(char));
            if(tokenlength == 1){
                delim[0] = token[0];
                token[0] = '\0';
                delim[1] = '\0';
            }
       // printf("104\n\n\n");
            else if(tokenlength >1){
                if(token[tokenlength]=='\n' || token[tokenlength]==' '|| token[tokenlength]=='\t' || token[tokenlength]=='\r' || token[tokenlength]== '\v' ){
                    delim[0] = token[tokenlength];
                    delim[1] = '\0';
                    token[tokenlength] = '\0';
                    printf("Token:%s \n", token);
                }
            }
printf("111\n\n\n");
            
            if(tokenlength > 1){
                tokencode = retcode(token, codebook);
                printf("token: %s\n", tokencode);

                write(towrite, tokencode, strlen(tokencode));
            }
        printf("120\n\n\n");
            delimcode = retcode(delim, codebook);
            write(towrite, delimcode, strlen(delimcode));
printf("delim: %s\n", delimcode);
            if(totaltokcounter +1 == size){
                return;
            }
        }


    }

 else{
        printf("Not a file\n");
        return;
    }
}

char* retcode(char* tofind, char* codebook){
if (isFile(codebook) == 1){
    int cb = open(codebook, O_RDWR);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char)); //holds codebook
    read(cb, buffer, 1);
    char* code;
    char* token;
    int codebookread=0;
    int tokenlength=0;
    int codelength=0;
printf("in ret code\n");
printf("ret code size: %d, %d", size, cb);
    while(codebookread < size){
        code = getNextToken( buffer+codebookread, size- codebookread);
        printf("in ret code:%s.\n",code);
        codelength = strlen(code);
        codebookread = codebookread+codelength;

        token = getNextToken( buffer+codebookread, size- codebookread);
        printf("in ret code, %s\n",token);
        tokenlength = strlen(token);
        codebookread = codebookread+tokenlength;

        token[tokenlength] = '\0'; // delete delimiter
        code[codelength] = '\0';
        if(strcmp(tofind, token) ==0 ){
            return code;
        }
        else{
            continue;
        }


    }
    return "Error: Code not found";
}
else{
   return "Not a file\n";
    
}
}
/*
void compress(char* tocompress, const char* codebook){
    //FILE* tc = fopen(tocompress);
    if(isFile(tocompress) == 1){
        //char* newfile = strcat(tocompress, ".hcz");
    int fd= open(tocompress, O_RDONLY);
    //create the output file
    char* filename ;
    //if isFile then strcat
    filename = strcat(tocompress, ".hcz");
    printf("filename %s\n", filename );
    int towrite = open(filename, O_CREAT | O_WRONLY | O_APPEND);
    int size=lseek(fd, 0, SEEK_END);//find the size of the file
    int l= lseek(fd, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char));
    read(fd, buffer,size);
    int toklen=0;
    int toklencounter=0;
    while(toklencounter<size){
//case: if the token is just a delimiter
        char* currNodeName;
        currNodeName = getNextToken(buffer+toklencounter, size);
        toklen = strlen(currNodeName); 
        printf("toklen: %d\n", toklen );
        char* delim=(char*) malloc(sizeof(char)*2);
        delim[0]=currNodeName[toklen-1];
        delim[1]='\0';

        currNodeName[toklen-1]='\0';
        printf("currNodeName: %s\n", currNodeName);
        printf("delim:%s\n", delim);
        toklencounter = toklencounter + toklen;
        printf("tokencounter %d\n", toklencounter );

        // ~ ~ ~ next part gets the huffman code ~ ~ ~ \\

        char* hcode;
        char* delimcode;

        hcode = retcode(currNodeName, codebook);  
        delimcode = retcode(delim, codebook);
        printf("HuffmanToken: %s HuffmanDelim: %s\n",hcode, delimcode);

        if(hcode == NULL){
            printf("Error: Huffman Code for the token %s, doesn't exist\n",currNodeName );
            return;
        }
         if(delimcode == NULL){
            printf("Error: Huffman Code for the token %s, doesn't exist\n",delim );
            return;
        }

        else{
            if(!(towrite)){
                printf("unable to create file\n");
                return;
            }
            else{
                printf("HuffmanToken: %s HuffmanDelim: %s\n",hcode, delimcode);
                write( towrite, hcode, sizeof(hcode)); 
                write( towrite, delimcode, sizeof(delimcode));
            }
        }
}
close(towrite);
close(fd);
}
else{
    printf("Not a file");
    return;
}
}
char* retcode(char* tofind, const char* codebook){
    int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char)); //holds codebook
    //int bufIndex=0;
    int hcodelen, toklen, toklencounter;
    toklencounter =0;
    //read(cb, buffer, 1);
    while(toklencounter < size){

        char* hcode = (char*) malloc(strlen(buffer) * sizeof(char));
        char* token = (char*) malloc(strlen(buffer) * sizeof(char));
       //printf("made tokens\n");
        hcode = getNextToken(buffer+toklencounter, 10);
        printf("hcode: %s\n",hcode);
        hcodelen = strlen(hcode);
        hcode = (char*) realloc(hcode, hcodelen);
        //printf("made it to realloc\n");
        hcode[hcodelen-1] = '\0';
        toklencounter = toklencounter+hcodelen;
 
        token = getNextToken(buffer+toklencounter, 10);
        toklen = strlen(token);
        token = (char*) realloc(token, toklen);
        token[toklen-1] = '\0';
        printf("TOKEN: %s\n",token);
        toklencounter = toklencounter+toklen;
  //printf("obtains next\n");
        if(strcmp(tofind, token) == 0){
            return hcode;
        }

    }

close(cb);
return 0;
}
*/
int main (int argc, char**argv){
    printf("Argv[2] %s\n",  argv[2]);
    compress(argv[1], argv[2]);

       
    return 0; 
    }
        