#include <stdlib.h>
#include <stdio.h>
//#include <sys/types.h>
#include <string.h>
//#include <errno.h>
/* "readdir" etc. are defined here. */
#include <dirent.h>
/* limits.h defines "PATH_MAX". */
//#include <limits.h>

/* List the files in "dir_name". */
void listdir(int flag, const char* dirname);
void openfiles(const char* filename);
int is_directory(const char *dir);
int isFile(char *to_read) ;
void writefiles(const char* filename);
//add parameters: boolean int to signify if there are flags happening/how many flags and string containing the flags to call 
int main (int argc, char**argv){

   if (is_directory(argv[1])) {
       // printf("yes\n");
        listdir(argv[1]);
    }
    else{
        printf("error: no directory found\n");
    }
       
    return 0; 
    }

void listdir(int flag, const char* dirname){
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
            char* currdir_name = (char*) malloc(currdir_len*sizeof(char));
            if(currdir_name == NULL){
            printf("malloc failed in directory walk \n");
            return;
            }
            currdir_name = dir_info->d_name;

            // + '/' + '\0'
            char* path = (char*) malloc(dirname_len+currdir_len+2 *sizeof(char));
             if(currdir_name == NULL){
            printf("malloc failed in directory walk \n");
            return;
            }
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
                //printf("in file\n");
                //if contains flags, apply/call flag functions here
                //printf("path: %s\n",path);
				
				if(flag == 2){
				compress(path, codebook);
				}
				if(flag == 3){
				}
            }
            //error check
            else if(dir_info->d_type == DT_UNKNOWN){
                printf("Error: Directory type is unknown\n");
                return;
            }
		//free(path);
        //free(currdir_name);
    	
        }
       closedir(currdir);
    }

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
/*
function to open file and output the contents to stdout
*/
void openfiles(const char* filename){
    //ab+ == append to file or create file for updates
   // FILE* towrite = fopen("tester.txt" , "ab+")
    FILE* toread = fopen(filename, "r");
    //add in permission error checks
    if(toread == NULL){
        printf("Error: file is null");
        return;
    }
    else{
        //print character by character
        char cr = fgetc(toread);
        while(cr!= EOF){
            if(feof(toread)){
                return;
            }
            printf("%c",cr );

            cr = fgetc(toread);
        }

        printf("\n");
    }
    fclose(toread);
}

void writefiles(const char* filename){
      //ab+ == append to file or create file for updates
    FILE* towrite = fopen("tester.txt" , "ab+");
    FILE* toread = fopen(filename, "r");
    //add in permission error checks
    if(toread == NULL){
        printf("Error: file is null");
        return;
    }
    if(towrite == NULL){
        printf("unable to create file\n");
        return;
    }
    else{
        //printf("in writefiles\n");
        //print character by character
        char cr = fgetc(toread);
        while(cr!= EOF){
            if(feof(toread)){
                return;
            }
            fputc(cr, towrite);

            cr = fgetc(toread);
        }

        printf("\n");
    }
    fclose(toread);
    fclose(towrite);

   // towrite = fopen("output.txt","r"); 
  
    // Reading the string from file 
  /*  fgets(received_string,20,fp); 
  
    printf("%s", received_string); 
  
    fclose(fp); 
*/
}
int isFile(char *to_read) {

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
}




/*
pass in the initial directory name, and this should not be changed
open the pass in directory 
create a struct dirent for it to store info when reading

while dirinfo= readdir directory
print directory name and curent 



void(const char* dirname){
    struct dirent* dir_info;


}

void listdir(* dr){


      if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory\n" ); 
        return ; 
    } 
  struct dirent *dirinfo;  // Pointer for directory entry 
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((dirinfo = readdir(dr)) != NULL) {

            if(dirinfo->d_type == DT_DIR){
                printf("in dir %s \n", dirinfo->d_name);
                DIR* diro = opendir(dirinfo);
                listdir(diro);
            }
            else if(dirinfo->d_type == DT_REG){

            printf("FILE: %s\n", dirinfo->d_name); 
            }

        
    }
    return;
}
*/

