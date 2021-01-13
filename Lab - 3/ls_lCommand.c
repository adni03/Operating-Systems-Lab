//LS -L 
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<time.h>

void printPermissions(struct stat s){
	if(s.st_size == 0) printf("d");
	if(s.st_mode&S_IRUSR) printf("r");
	else printf("-");
	if(s.st_mode&S_IWUSR) printf("w");
	else printf("-");
	if(s.st_mode&S_IXUSR) printf("x");
	else printf("-");
	if(s.st_mode&S_IRGRP) printf("r");
	else printf("-");
	if(s.st_mode&S_IWGRP) printf("w");
	else printf("-");
	if(s.st_mode&S_IXGRP) printf("x");
	else printf("-");
	if(s.st_mode&S_IROTH) printf("r");
	else printf("-");
	if(s.st_mode&S_IWOTH) printf("w");
	else printf("-");
	if(s.st_mode&S_IXOTH) printf("x");
	else printf("-");	
}

void printUser(){
	struct passwd *pass;
	pass = getpwuid(getuid());
	printf(" %s ", pass->pw_name);
}

char* formatdate(char *date, time_t val){
	strftime(date, 36, "%d.%m.%Y %H:%M", localtime(&val));
	return date; 
}

void printTime(struct stat s){
	char date[36];
	printf("%s ", formatdate(date, s.st_mtime));
}

int main(int argc, char **argv){
	DIR* p = opendir(".");
	struct dirent* d;
	if(p){
		while(1){
			d = readdir(p);
			if(d){
				struct stat s;
				stat(d->d_name, &s);
				printPermissions(s);
				printUser();
				printTime(s);
				printf("%s\n", d->d_name);
				//printf(" %s", ctime(&s.st_ctime));
			}
			else break;
		}
	}
	return 0;
}
