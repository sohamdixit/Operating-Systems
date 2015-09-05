#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>

void remove_util(char *);

//Global flags
bool FLG_PROMPT = false;

void del(char *path) {
	struct stat s;
	char path_bkp[256];
	char *p2 = (char *) &path_bkp;

	strcpy(p2, path);
	stat(path, &s);

	if(S_ISDIR(s.st_mode)) {
		//Directory
		DIR *dp = opendir(path);
		struct dirent *entry = NULL;
		
		//Recur for contents of directory
		while(((entry = readdir(dp)) != NULL) && strcmp(".",entry->d_name) && strcmp("..",entry->d_name)) {
			strcat(path, entry->d_name);
			del(path);
			strcpy(path, p2);
		}
		
		//remove the directory itself
		remove_util(path);

	} else //File
		remove_util(path);
}

void remove_util(char *path) {

 	printf("\nr_u() path: %s", path);

	char response = 'n';
	if(FLG_PROMPT) {
		printf("Unlink %s ? (y/n): ", path);
		scanf(" %c", &response);
		if(response == 'y' || response == 'Y')
			if(unlink(path) == -1) perror("unlink()");
	} else if(unlink(path) == -1)
		perror("unlink()");
}

int main(int argc , char *argv[]) {
	
	int n = 0;
	char *path[16];
	
	int i;
	for(i = 1 ; i<argc ; i++)
	{
		if(argv[i][0] == '-')
		{
			if(!strcmp(argv[i],"-i"))
				FLG_PROMPT = true;
		} else path[n++] = argv[i];
	}

	if(!n) {
		printf("No file(s) specified!\n");
		exit(2);
	}
	
	int j;
	for(j = 0 ; j<n ; j++)  {
		del(path[j]);
	}
}
