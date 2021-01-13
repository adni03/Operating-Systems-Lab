#include<stdlib.h>
#include<stdio.h>
#define n 10

struct bstnode{
	int data;
	struct bstnode* left;
	struct bstnode* right;
};

struct bstnode* insAux(struct bstnode* T, int x){
	if(!T){
		T = (struct bstnode*)malloc(sizeof(struct bstnode));
		T->data = x;
		T->left = NULL;
		T->right = NULL;
		return T;
	}
	else if(T->data > x) T->left = insAux(T->left, x);
	else T->right = insAux(T->right, x);
	return T;
}

struct bstnode* Insert(struct bstnode* T, int nums[]){
	for(int i=0; i<n; i++){
		T = insAux(T, nums[i]);
	}
	return T;
}

void print(struct bstnode* T, int val){
	if(T){
		if(val == 0){ // preorder
			printf("%d ", T->data);
			print(T->left, val);
			print(T->right, val);
		}
		else if(val == 1){ // inorder
			print(T->left, val);
			printf("%d ", T->data);
			print(T->right, val);
		}
		else{ // postorder
			print(T->left, val);
			print(T->right, val);
			printf("%d ", T->data);
		}
	}
}

int main(){
	struct bstnode* T = NULL;
	int nums[n] = {2, 1, 3, 5, 6, 8, 4, 9, 20, 12};
	T = Insert(T, nums);
	printf("PreOrder Traversal : ");
	print(T, 0);
	printf("\n");
	printf("InOrder Traversal : ");
	print(T, 1);
	printf("\n");
	printf("PostOrder Traversal : ");
	print(T, 2);
	printf("\n");
	return 0;
}
