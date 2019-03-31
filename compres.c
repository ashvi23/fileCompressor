#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

//void listdir(const char* dirname, const char* codebook);
//void openfiles(const char* filename);
//int is_directory(const char *dir);
//void writefiles( char* filename);

char* getNextToken(char* filename, int size, int offset);
int compress(char* tocompress, char* codebook);
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
      return -7;
    }

  } else {


    return -6;
  }
}
char* getNextToken(char* filename, int size, int offset){
    if(size ==0){
        return "3";
    }
    int fd= open(filename, O_RDONLY);
    int l= lseek(fd, offset, SEEK_SET);
    char* token= NULL;
    token = (char*)malloc(2*sizeof(char));
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
//printf("GNT Tempchar:_%s_\n", tempchar);
        strcat(token, tempchar);
        //printf("GNT: token before realloc:[%s]\n", token);
        tokensize = sizeof(token);
        char* temp = (char*) realloc(token , tokensize+1* sizeof(char) );
        if(temp ==NULL){
            printf("Error: Cannot malloc space for token\n");
            return "3";
        }
        else{
            token = temp;
           // printf("GNT: token after realloc:[%s]\n", token);
        }
int tempcharint = tempchar[0];
        if( tempchar[0]==' '|| iscntrl( tempchar[0])>0) {
            printf("GNT Token:[%s] ",token );
            printf("GNT Delim:_%s_\n", tempchar);
          
            //free(tempchar);
            //free(temp);
            close(fd);
            return token; 
            break;   
        }
        if(tokcounter+1 == size ){
            //free(temp);
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
   // printf("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    if (isFile(tocompress) == 1){
         int fd= open(tocompress, O_RDONLY);
     
        //create the output file
        int clen = strlen(tocompress)+ strlen(".hcz");
        char* filename = (char*) malloc(clen*sizeof(char));
        if(filename == NULL){
        printf("Error mallocing space for filename in Compress\n");
        return 0;}
        int le = strlen(tocompress);
       // int comp = strlen(tocompress);
       // comp = comp - 4;
        //strncpy(filename, tocompress, comp);
        //strcat(filename, "1.txt");
        strcat(filename, ".hcz");
        int towrite = open(filename, O_CREAT | O_RDWR | O_APPEND);
        int size=lseek(fd, 0, SEEK_END);//find the size of the string
        int l= lseek(fd, 0, SEEK_SET);
        int totaltokcounter=0;
        int tokenlength=0;
        //char* token;
        char* delim;
        //char* delimcode;
        //char* tokencode;

        while(totaltokcounter < size){
printf("STARTING COMPRESS \n\n");
            if(totaltokcounter+1  == size){
                close(fd);
                free(delim);
                return 1;
            }
            char* token = getNextToken(tocompress, size - totaltokcounter, totaltokcounter);
            if(strcmp(token, "3")==0){
            	printf("Error: Could not get next token.\n");
            	remove(filename);
            	return 0;
            }
            tokenlength = strlen(token);
            totaltokcounter = totaltokcounter + tokenlength;
            printf("C token:[%s] COUNTER:%d\n\n", token, totaltokcounter);
            delim = (char*) malloc(2 * sizeof(char));
			if(delim == NULL){
			printf("Error: Malloc returned NULL\n");
			remove(filename);
			return 0;
			}
            //token is a delimiter --> no letters. null [len]/[1] & cntrlcode [len-1] /[0]
            printf("the token:[%s]\n",token );
            if(tokenlength == 1){
                delim[0] = token[0];
// if is char or if is cntrl token[0]
                if(iscntrl(token[tokenlength-1]) > 0 || token[tokenlength-1] == ' '){
                token[tokenlength] = '\0';
                printf("COMPRESS TOKEN:[%s]\n", token);
                }
                //token[tokenlength-1] = '\0';
                delim[1] = '\0';
            }
printf("111\n\n\n");
            //contains at least 1 letter/non delimiter 
            // delimiter[len-1] 
            if(tokenlength >1){
                if(iscntrl(token[tokenlength-1]) >0||token[tokenlength-1] == ' ' ){
                    delim[0] = token[tokenlength-1];
                    delim[1] = '\0';
                    token[tokenlength-1] = '\0';
                    //printf("C Token:[%s] \n", token);
                }

            }
            printf("C token:[%s] delim:[%d] totaltokcounter:%d size:%d\n",token, delim[0], totaltokcounter, size );
//printf("111\n\n\n");
            printf("C token:[%s] delim:[%s]\n", token, delim );
            if(strcmp(token, "3") !=0){
                if(tokenlength > 1){
                    char* tokencode = retcode(token, codebook);
                    //printf("C token:[%s]\n", tokencode);
                    if(strcmp(tokencode,"3") !=0 ){
                        printf("C Towrite- tokencode:[%s] for token:[%s]\n",tokencode, token);
                        write(towrite, tokencode, strlen(tokencode));
                    }
                    else if(strcmp(tokencode,"3") ==0){
                	printf("Error: Token %s doesn't exist in codebook\n", token);
                	remove(filename);
                	return 0;
                	}
                }
        }
      // printf("120\n\n\n");
        printf("Now finding delim code\n");

            if(strcmp(delim, "3") !=0 && (iscntrl(delim[0])>0 || delim[0]==' ') ){
                char* delimcode = retcode(delim, codebook);
                if(strcmp(delimcode,"3") !=0){
                    printf("C Towrite- delim: %s for delim:[%s]\n", delimcode, delim);
                    write(towrite, delimcode, strlen(delimcode));
                }
                else if(strcmp(delimcode,"3") ==0){
                	printf("Error: Token %s doesn't exist in codebook\n", delim);
                	remove(filename);
                	return 0;
                	}
            }

            
            printf("C tokencode delimcode completed\n" );

            if(totaltokcounter+1  == size){
     
                //free(buffer);
                free(delim);
                
                return 1;
            }
        }
        
     

  //close(cb);
    close(fd);
    }

 else{
        printf("Not a file\n");
        return -4;
    }

    return 0;
}


char* retcode(char* tofind, char* codebook){

if (isFile(codebook) == 1){
    int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET); 
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
    //size is the bytes in the codebook
     while(codebookread < size){
        //int tfint = tofind[0];
        printf("RC: To Find:[%d]\n", tofind[0]);
        code = getNextToken(codebook, size - codebookread , codebookread);
        codelength = strlen(code);
        codebookread = codebookread + codelength;
        printf("RC code:[%s]\n",code);

        if(strcmp(code, "3")==0){
            printf("Error: Could not retrieve code in HuffmanCodebook at offset:%d \n", codebookread);
            
            return "3";
        }
       
        token = getNextToken( codebook,  size - codebookread, codebookread);    
        tokenlength = strlen(token);
        codebookread = codebookread+tokenlength;
         if(token[0] == ' ' || iscntrl(token[0])>0){
            char* discard = getNextToken(codebook, size -codebookread, codebookread);
            tokenlength = strlen(discard);
            codebookread = codebookread+tokenlength;
            printf("for token:[%s]\n", token );

        }
        if(strcmp(token, "3")==0){
            printf("Error: Could not retrieve token in HuffmanCodebook at offset:%d \n", codebookread);
            return "5";
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
        printf("247\n");
        if(strcmp(tofind, token) ==0 ){
            printf("RC Returning Code:%s\n", code );
            return code;
        }
        printf("248\n");
        if(codebookread+1 == size && strcmp(tofind, token) !=0){
            printf("RC reached size\n");
            return "3";
        }
        else{
            printf("\n\n");
            continue;
        }


        }
        printf("RC Errors: Code not found\n");
        return "3";
    }
else{
   return "RC Not a file\n";
   }
}
int main (int argc, char**argv){

    //printf("Argv[2] %s\n",  argv[2]);
    compress(argv[1], argv[2]);
   // getNextToken("  i", 8);
   // retcode("button", argv[1]);
//free(send);
       
    return 0; 
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



///BEFORE DELETING PRINT STATEMENTS--- HAD BUS ERROR 10


char* retcode(char* tofind, char* codebook){
 int cb = open(codebook, O_RDONLY);
if (isFile(codebook) == 1){
    int cb = open(codebook, O_RDONLY);
    int size=lseek(cb, 0, SEEK_END);//find the size of the file
    int l= lseek(cb, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char)); //holds codebook
    read(cb, buffer, size);
    char* code;
    char* token;
    int codebookread=0;
    int tokenlength=0;
    int codelength=0;
printf("in ret code\n");
printf("ret code size: %d, %d\n", size, cb);
printf("RC buffer:%s\n", buffer);
int tofindlen = strlen(tofind);
printf("RC TOFIND BEFORE%s\n", tofind );
if(iscntrl(tofind[tofindlen]) > 0 && tofind[tofindlen]!= '\0'){
    printf("RC word:[%s]len:%d char:[%c]\n", tofind, tofindlen, tofind[tofindlen]);
    tofind[tofindlen-1] = '\0';
   printf("RC AFTER MESS word:%s___len:%d___char:%c_\n", tofind, tofindlen, tofind[tofindlen]);
}

 printf("RC wor 2 d:%s___len:%d___char:%c_\n", tofind, tofindlen, tofind[tofindlen]);
if(tofind[tofindlen-1]== ' ' && tofindlen>1){
      printf("RC wor 2 d:%s___len:%d___char:%c_\n", tofind, tofindlen, tofind[tofindlen]);
    tofind[tofindlen-1] = '\0';
    printf("RC AFTER MESS word:%s___len:%d___char:%c_\n", tofind, tofindlen, tofind[tofindlen]);

}


//}

    while(codebookread < size){
         printf("RC to find afterWHILE:[%s], codebookread:%d\n", tofind, codebookread);
        code = getNextToken( codebook, size - codebookread , codebookread);
        printf("in ret code:%s)\n",code);
        codelength = strlen(code);
        printf("RCcodelength: %d\n", codelength);
        codebookread = codebookread + codelength;
        printf("RC codebookread:%d\n", codebookread);


        token = getNextToken( codebook,  size - codebookread, codebookread);
        printf("RC token:%s\n",token);
        tokenlength = strlen(token);
        codebookread = codebookread+tokenlength;
        printf("RC codebookread:%d\n", codebookread);
        
        token[tokenlength-1] = '\0' ; // delete delimiter
        if(codelength > 1){
        code[codelength-1] = '\0' ;
        }

        printf("RC code length after:%lu tokenlength after: %lu ~ ~\n", strlen(code), strlen(token));

       // printf("tokenlength after: %lu_%c_ codelength after:%lu_%c_\n", strlen(token), token[tokenlength+1], strlen(code), code[codelength+1] );
        printf("RC TO FIND:[%s] tofindlen:[%c]  token:[%s/\n", tofind, tofind[tofindlen-1], token);
        printf("RC COMPARED: %d\n",strcmp(tofind, token) );
        if(strcmp(tofind, token) ==0 ){
            printf("RC returning: %s for token:%s\n", code, token );
            return code;
        }
        if(codebookread+1 == size && strcmp(tofind, token) !=0){
printf("RC reached size\n");
            return "-2";
        }
        else{
            printf("\n\n\n\n\n\n");
            continue;
        }


    }
    printf("RC Errors: Code not found\n");
    return "-3";
}
else{
   return "RC Not a file\n";
    
}
}
int main (int argc, char**argv){

    //printf("Argv[2] %s\n",  argv[2]);
    int sf = strlen(argv[2]);
    char* send =(char*) malloc(sf*sizeof(char));
     int df = strlen(argv[1]);
    char* send1 =(char*) malloc(df*sizeof(char));
    compress(argv[1], argv[2]);
   // getNextToken("  i", 8);
   // retcode("button", argv[1]);
//free(send);
       
    return 0; 
    }
*/
        
