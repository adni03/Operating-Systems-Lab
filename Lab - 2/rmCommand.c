//IMPLEMENTATION OF rm COMMAND
#include<stdio.h>

int main(int argc, char **args){
	printf("%s", args[0]);
	while(argc){
		int status = remove(args[argc]);
		if(status == 0) printf("File %s has been deleted\n", args[argc]);
		else{
			printf("File couldnt be deleted\n");
			perror("With error");
		}
		--argc;
	}
	return 0;
}
