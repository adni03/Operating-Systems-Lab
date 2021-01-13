//IMPLEMENTATION OF ls -l COMMAND
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>

void printStatus(long tpid){
	char path[40], line[100], *p;
	FILE* statusf;
	snprintf(path, 40, "/proc/%ld/status", tpid);
	statusf = fopen(path, "r");
	if(!statusf) return ;
	while(fgets(line, 100, statusf)){
		if(strncmp(line, "State:", 6) != 0) continue;
		p = line + 7;
		while(isspace(*p)) ++p;
		printf("%6ld %s", tpid, p);
		break;
	}
	fclose(statusf);
}

int main(int argc, char **args){
	struct dirent* ent;
	DIR* proc = opendir("/proc");
	long tpid;
	if(proc == NULL){
		printf("Error\n");
		return 0;
	}
	while(ent = readdir(proc)){
		if(!isdigit(*ent->d_name)) continue;
		tpid = strtol(ent->d_name, NULL, 10);
		printStatus(tpid);
	}
	closedir(proc);
	return 0;
}
