void listdir(const char* dirname, const char* codebook){
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
            //path now contains the current directory path

            // "."
            if(strncmp(dir_info->d_name, ".", sizeof(char))==0){
                continue;
            }
            //is directory
            else if(dir_info->d_type == DT_DIR){
                listdir(path);
            }
            //is file
            else if(dir_info->d_type == DT_REG){
                if(flag ==1){
                	build(1, path);
                }
                else if(flag == 2){
			compress(path, codebook);
		}
		else if(flag == 3){
			decompress(path, codebook);
		}
            }
            //error check
            else if(dir_info->d_type == DT_UNKNOWN){
                printf("Error: Directory type is unknown\n");
                return;
            }

        }
	if (flag == 1){
		struct HeapNode* sortedHeapHead=NULL;
		sortedHeapHead= hashToArr();
		struct HeapNode *treehead = buildhTree(sortedHeapHead, treehead);
		buildCBook(treehead);  
	}
    closedir(currdir);
    }

}

