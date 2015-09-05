#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

	const char *ENV_PATH = getenv("PATH");
	printf("ENV_PATH: \n", ENV_PATH);
	char *ENV_PATH_CPY = (char*) malloc(strlen(ENV_PATH)*sizeof(char));

	printf("%i\n", isatty(STDOUT_FILENO));
}
