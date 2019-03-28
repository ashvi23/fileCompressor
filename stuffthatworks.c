#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void listdir(const char* dirname, const char* codebook);
void parseFile(const char* pathName);
char* getNextToken(char* buffer, int size);
void openfiles(const char* filename)
void writefiles(const char* filename)


void listdir(const char* dirname, const char* codebook){
	DIR* currdir = opendir(dirname);
    struct dirent *dir_info;
    if(!currdir){
        printf("Error: Cannot open directory\n");
        return;
    }
    // create file paths
    else{
        //while there are still more files/dir left to read
        while((dir_info = readdir(currdir))){
            //errors? --> permissions, null idk
            //strcat current dir name --> creates strings of the name of dirname and curdir

            int dirname_len = strlen(dirname);
            int currdir_len = strlen(dir_info->d_name);
            char* currdir_name = (char*) malloc(currdir_len);
            currdir_name = dir_info->d_name;

            // + '/' + '\0'
            char* path = (char*) malloc(dirname_len+currdir_len+2);
            memcpy(path, dirname, dirname_len);
            memcpy(path+dirname_len , "/", 1);
            memcpy(path+dirname_len+1 , currdir_name, currdir_len+1);
            //path now contains the current directory path

            // "."
            if(strncmp(dir_info->d_name, ".", sizeof(char))==0){
                continue;
            }
            //is directory
            else if(dir_info->d_type == DT_DIR){
                //printf("in directory\n");
                listdir(path);
            }
            //is file
            else if(dir_info->d_type == DT_REG){
               // printf("in file\n");
                //if contains flags, apply/call flag functions here
                printf("path: %s\n",path);
                openfiles(path);
                //writefiles(path);
                //parseFile(path);
                compress(path, codebook);
            }
            //error check
            else if(dir_info->d_type == DT_UNKNOWN){
                printf("Error: Directory type is unknown\n");
                return;
            }

        }
    closedir(currdir);
    }

}

void parseFile(char* tokenize ){
    int fd= open(pathName, O_RDONLY);
    int size=lseek(fd, 0, SEEK_END);//find the size of the file
    int l= lseek(fd, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char));
    int bufIndex=0;    read(fd, buffer,size);
    int startingindex=0;
    int toklen=0;
    while( bufIndex < size){
        if(buffer[bufIndex]=='\n' || buffer[bufIndex]==' '|| buffer[bufIndex]=='\t' || buffer[bufIndex]=='\r' || buffer[bufIndex]== '\v'){
            
            char* delim=(char*) malloc(sizeof(char)*2);
            delim[0]=buffer[bufIndex];
            delim[1]='\0';
            buffer[bufIndex]='\0';
            toklen = bufIndex-startingindex;
            char* currNodeName= (char*) malloc(sizeof(char)*toklen+1);
            strncpy(currNodeName, buffer+startingindex, toklen);
            currNodeName[toklen+1] = '\0';
            startingindex = bufIndex+1; 
            
        }
        bufIndex++;
    }
    return;
}

/*
function to open file and output the contents to stdout
*/
void openfiles(const char* filename){
    int tr= open(filename, O_RDONLY);
    int size=lseek(tr, 0, SEEK_END);//find the size of the file
    printf("size %d\n",size );
    int l= lseek(tr, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char));
    int bufIndex =0;
    //add in permission error checks
    if(tr == -1){
        printf("Error: file is null");
        return;
    }
    else{
        //print character by character
        read(tr, buffer, 1);
        while(bufIndex < size){
           
            printf("%s",buffer );

            read(tr, buffer, 1);
            if(buffer == '\0'){
                break;
            }
            bufIndex++;
        }

        printf("\n");
    }
    free(buffer);
    close(tr);
}
// i dont know how to create a new file everytime, but works otherwise!
void writefiles(const char* filename){
    int tw = open("tester.txt", O_CREAT | O_WRONLY | O_APPEND);
    int tr= open(filename, O_RDONLY);
    //add in permission error checks
    int size=lseek(tr, 0, SEEK_END);//find the size of the file
    int l= lseek(tr, 0, SEEK_SET);
    char* buffer= NULL;
    buffer = (char*)malloc(size*sizeof(char));

    if(tr == -1){
        printf("Error: file is null");
        return;
    }
    if(tw == -1){
        printf("error unable to create file\n");
        return;
    }
    else{
        read(tr, buffer, 1);
        int bufIndex =0;
        while(bufIndex < size){
            
            printf("%s",buffer);
            write(tw, buffer,1 );
            read(tr, buffer, 1);
            bufIndex++;
        }

        printf("\n");
    }
    close(tr);
    close(tw);
    free(buffer);
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
