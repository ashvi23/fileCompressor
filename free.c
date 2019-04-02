struct HashNode{
	char* token;
	int key;
	int frequency;
	struct HashNode* next;
};
struct HeapNode{
	int frequency;
	char* name;//token gathered
	int height;
	struct HeapNode* left;
	struct HeapNode* right;
};
struct LLNode{
	struct HeapNode* Tree;
	struct LLNode* next;
	};
void freeLL(struct LLNode* headLL){
	struct LLNode* temp;
	while(head!=NULL){
		tmp=head;
		freeTree(tmp->tree);
		head=head->next;
		free(temp);
		}
	return;
	}

void freeHash(struct HashNode* headHash){
	int i=0;
	for(i=0; i<10000; i++){
		headHash[i];
	}
}

void freeTree(struct HeapNode* headTree){
	if(head==NULL){
		return;
	}
	freeTree(head->left);
	freeTree(head->right);
	free(head->name);
	free(head);
}
