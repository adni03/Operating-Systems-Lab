// DEADLOCK AVOIDANCE ALGORITHM
#include<stdio.h>
#include<stdlib.h>

#define m 3 // total number of resources available
#define n 5 // total number of processes

int available[m] = {10, 5, 7};
int max[n][m] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
int allocation[n][m] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
int need[n][m];
int order[n];

void calculateNeed(){
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}
}

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

int avoidDeadlock(){
	calculateNeed();
	int k=0, flag = 0;
	int *initAvailable = calculateAvailable();
	int *work = initAvailable;
	/*for(int i=0; i<m; i++){
		printf("%d ",*(initAvailable+i));
	}*/
	
	int finish[n] = {0};
	
	for(; ;){
		flag = 0;
		for(int i=0; i<n; i++){
			if(!finish[i] && compare(need[i], work, -1)){
				flag = 1;
				finish[i] = 1;
				order[k++] = i;
				for(int j=0; j<m; j++){
					*(work+j) = *(work+j) + allocation[i][j];
				}
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
	int lol = avoidDeadlock();
	printf("Deadlock detected? : %d", lol);
	if(!lol){
		printf("\nOrder is : \n");
		for(int i=0; i<n; i++){
			printf("P%d ", order[i]);
		}
		printf("\n");
	}
	return 0;
}
