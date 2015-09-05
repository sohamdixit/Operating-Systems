#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool FLG_PRNTL = false, FLG_PRNTW = false, FLG_PRNTC = false;

int main(int argc , char *argv[]) 
{
	int chc = 0, wrdc = 0, linec = 0, n = 0;
	int tchc = 0, twc = 0, tlc = 0;
	char ch, prev;
	char *path[16];
	FILE *fp;

	int i;
	for(i=1 ; i<argc ; i++) {
		if(argv[i][0] == '-') {
			if(!strcmp(argv[i],"-c"))
				FLG_PRNTC = true;
			else if(!strcmp(argv[i],"-l"))
				FLG_PRNTL = true;
			else if(!strcmp(argv[i],"-w"))
				FLG_PRNTW = true;
		} else path[n++] = argv[i];
	}

	if(!FLG_PRNTW && !FLG_PRNTL && !FLG_PRNTC)
		FLG_PRNTW = FLG_PRNTL = FLG_PRNTC = true;
	
	for(i=0 ; i<n ; i++) {
		wrdc = linec = chc = 0;
		prev = '\0';	
		fp = fopen(path[i] , "r");

		if(!fp) {perror("fopen()"); continue;}

		while((ch = fgetc(fp)) != EOF)
		{
			chc++;
			if((ch == ' ' || ch == '\t' || ch == '\n') && prev != ' ' && prev != '\t' && prev != '\n') wrdc++;
			if(ch == '\n') linec++;
			prev = ch;
		}
		printf("%s  ", path[i]);
		if(FLG_PRNTL) printf("%i ", linec);
		if(FLG_PRNTW) printf("%i ", wrdc);
		if(FLG_PRNTC) printf("%i", chc);
		printf("\n");
		tchc += chc; twc += wrdc; tlc += linec;
	}
	
	if(n != 1) printf("total  %i %i %i\n", tlc, twc, tchc);
}
