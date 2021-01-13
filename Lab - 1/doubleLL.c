#include<stdlib.h>
#include<stdio.h>
#define n 10

struct Dllnode{
	int data;
	struct Dllnode* left;
	struct Dllnode* right;
};

struct Dllnode* Insert(struct Dllnode* L, int nums[]){
	for(int i=0; i<n; i++){
		struct Dllnode* temp = (struct Dllnode*)malloc(sizeof(struct Dllnode));
		temp->data = nums[i];
		temp->left = temp->right = NULL;
		if(!L) L = temp;
		else{
			temp->right = L;
			L->left = temp;
			L = temp;
		}
	}
}

struct Dllnode* Delete(struct Dllnode* L, int x){
	struct Dllnode* temp = L, *temp2;
	while(temp){
		if(temp->data == x){
			if(temp->left) temp->left->right = temp->right;
			if(temp->right) temp->right->left = temp->left;
			if(temp->left){
				temp2 = temp->right;
				free(temp);
				temp = temp2;
			}
			else{
				temp2 = temp;
				temp = temp->right;
				free(temp2);
				L = temp;
			}
				
		}
		else temp = temp->right;		
	}
	return L;
}

void search(struct Dllnode* L, int x){
	if(!L) printf("Element not found");
	else if(L->data == x) printf("Element found");
	else search(L->right, x);
}

void print(struct Dllnode* L){
	if(L){
		printf("%d ", L->data);
		print(L->right);
	}
}

int main(){
	struct Dllnode* L = NULL;
	int i=0, j=n-1, temp;
	int nums[n] = {2, 1, 3, 5, 6, 8, 4, 9, 20, 12};
	while(i <= j){
		temp = nums[j];
		nums[j] = nums[i];
		nums[i] = temp;
		++i;
		--j;
	}
	L = Insert(L, nums);
	print(L);
	printf("\n");
	printf("Enter element to be deleted : ");
	scanf("%d", &temp);
	Delete(L, temp);
	print(L);
	printf("\n");
	printf("Enter element : ");
	scanf("%d", &temp);
	search(L, temp);
	printf("\n");
	return 0;
}
