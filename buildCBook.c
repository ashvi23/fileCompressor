#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>
void buildCBook.c(struct AVLNode* hufftree, const char* pathname);
void traverseTree(struct AVLNode* root, int filedesc,  int dir);//dir is direction
void buildCBook.c(struct AVLNode* hufftree, const char* pathname){
		int fd=open("./HuffmanCodebook", O_WRONLY | O_CREAT, 00644);
		//traverse through Huffman tree (inorder), printing each time a leaf is found(i.e. leftchild/rightchild ==null or ig if name != NULL), print on each traversal downward.
		if(write(fd, "^%\n", 3)!=3){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			} 
		traverseTree(hufftree, fd, -1);
		int success=close(fd);
		if(success!=0){
		printf("problem closing file descriptor\n");
		}
		return;
	}

void traverseTree(struct AVLNode* root, int filedesc, int dir){
		if(dir==0){
			//print 0 to file
			if(write(fd, "0", 1)!=1){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			}
		}
		else{
			//print 1 to file
			if(write(fd, "1", 1)!=1){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
			}
		}
		if(root->left == NULL && root->right==NULL){
			//when you get to the leaf node, print a space and then the name contained in that node
			write(fd, "\t",1);
			int currTokLen=strlen(root->name);
			if(strncmp(root->name, '\n')==0){//writing escape characters to file
				if(write(fd, "^%\\n", currTokLen+2)!=(currTokLen+2)){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(strncmp(root->name, '\t')==0 ){
				if(write(fd, "\\t", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			} 
			else if(strncmp(root->name, '\r'){
				if(write(fd, "\\r", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(strncmp(root->name, '\v')==0){
				if(write(fd, "\\v", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else if(strncmp(root->name, '\')==0 ){
				if(write(fd, "\\", currTokLen+2)!=currTokLen+2){
				write(2,"There was an error writing to HuffmanCodebook\n", 47);
				}
			}
			else{
				write(fd, root->name, currTokLen);
				}
			write(fd, "\n", 1);//write newline
		}
		traverseTree(root->left,fd, 0);
		traverseTree(root->right,fd, 1);
		return;
	}
