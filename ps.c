#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main(int argc , char* argv[]) {
	
	int id;
	DIR *dirp;
	FILE *f;
	char procbuf[128], state;
	char *name = (char *) malloc(64);
	struct dirent *d = NULL;
	
	dirp = opendir("/proc");
	if(dirp == NULL) {
		perror("/proc");
		return 1;
	}
	
	printf("PID\tNAME\n");
	while(d = readdir(dirp))
	{
		if(d->d_name[0] != '.') {
			strcpy(procbuf , "/proc/");
			strcat(procbuf , d->d_name);
			strcat(procbuf , "/stat");
		
			f = fopen(procbuf , "r");
			if(f == NULL) continue;
			fscanf(f,"%i %s %c", &id, name, &state);
			if(state == 'R') printf("%i\t%s\n", id, name);
		}
	}
	return 0;
}
