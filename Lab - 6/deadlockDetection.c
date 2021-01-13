// DEADLOCK DETECTION ALGORITHM
#include<stdlib.h>
#include<stdio.h>

#define m 3 // total number of resources available
#define n 5 // total number of processes

int available[m] = {7, 2, 6};
int request[n][m] = {{0, 0, 0}, {2, 0, 2}, {0, 0, 1}, {1, 0, 0}, {0, 0, 2}};
int allocation[n][m] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 3}, {2, 1, 1}, {0, 0, 2}};
int order[n];

int* calculateAvailable(){
	int *initAvailable = available;
	for(int j=0; j<m; j++){
		for(int i=0; i<n; i++){
			initAvailable[j] = initAvailable[j] - allocation[i][j];
		}	
	}
	return initAvailable;
}

int compare(int *a, int *b, int ch){
	int flag = 0;
	for(int i=0; i<m; i++){
		if(ch == -1){
			if(*(a+i) > *(b+i)){
				flag = 1;
				break;
			}
		}
	}
	return flag == 0;
}

int detectDeadlock(){
	int *initAvailable = calculateAvailable();
	int *work = initAvailable;
	int flag = 0, k = 0;
	int finish[n];
	
	/*for(int i=0; i<m; i++){
		printf("%d ",*(initAvailable+i));
	}*/
	
	for(int i=0; i<n; i++){
		flag = 0;
		for(int j=0; j<m; j++){
			if(allocation[i][j] != 0){
				flag = 1;
				break;
			}	
		}
		if(flag == 1) finish[i] = 0;
		else finish[i] = 1;
	}
	
	for(; ;){
		flag = 0;
		for(int i=0; i<n; i++){
			if(!finish[i] && compare(request[i], work, -1)){
				flag = 1;
				finish[i] = 1;
				order[k++] = i;
				for(int j=0; j<m; j++){
					*(work+j) = *(work+j) + allocation[i][j];
				}
				break;
			}
		}
		if(flag) continue;
		else{
			for(int j=0; j<n; j++){
				if(!finish[j]) return 1;
			}
			return 0;
		}
	}
}

int main(){
	int lol = detectDeadlock();
	printf("Deadlock detected? : %d\n", lol);
	if(!lol){
		printf("Order is : \n");
		for(int i=0; i<n; i++){
			printf("P%d ", order[i]);
		}
		printf("\n");
	}
	return 0;
}
