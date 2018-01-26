#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
	int n = 0, SIG = -1;
	pid_t pid[16];

	int i;
	for(i=1 ; i<argc ; i++)
	{
		if(argv[i][0] == '-') SIG = atoi(&argv[i][1]);
		else pid[n++] = (pid_t) atoi(argv[i]);
	}

	for(i = 0 ; i<n ; i++) {
		if(SIG == -1) {
			if(kill(pid[i], SIGKILL) == -1)
				perror("kill()");
		} else if(kill(pid[i], SIG) == -1) perror("kill()");
	}
	return 0;
}
