#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc , char *argv[]) {
	
	bool FLG_PROMPT = false;
	int n = 0;
	char *path[16], response;
	
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
	for(j = 0 ; j<n ; j++) 
	{
		response = 'n';
		if(FLG_PROMPT)
		{
			printf("Unlink %s ? (y/n): ", path[j]);
			scanf(" %c", &response);
			if(response == 'y' || response == 'Y')
				if(unlink(path[j]) == -1) perror("unlink()");
		} else if(unlink(path[j]) == -1)
			perror("unlink()");
	}
}
