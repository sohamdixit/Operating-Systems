#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <readline/readline.h>

#define DELIM_CHARS " \t\n"
#define ABNORMAL_EXIT 3
//#define DEBUG

int i, ERR_FLG = 0, token_cnt = 0, PWD_SIZ = 512;
int STDIN_CPY, STDOUT_CPY;
char *pwd, *line = NULL, *PATH;
char **tokens = NULL;
const char *ENV_PATH;

char *builtin_str[] = {
	"cd",
	"exit",
	"clear"
};

char **parse(char *);
void runShell();
void preprocess(char ***);

int shell_cd(char **args) {
	if(chdir(args[1]) == -1)
		perror("");
	getcwd(pwd, PWD_SIZ);
	strcat(pwd, "> ");
}

int shell_exit(char **args) {
	printf("\033[2J\033[1;1H");
	exit(0);
}

int shell_clear(char **args) {
	printf("\033[2J\033[1;1H");
}

int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_exit,
  &shell_clear
};


int builtin_cnt() {
	return (sizeof(builtin_str) / sizeof(char*));
}

void preprocess(char ***tokens) {
	//Preprocess for things like IO redirection and Background execution

	char **new_tokens = (char**) malloc((sizeof(*tokens) + 1) * sizeof(char*));
	char **old_tokens = *tokens;
	int n = 0;

	for(i = 0 ; i<token_cnt ; i++) {
		if(!strcmp(old_tokens[i], ">")) { //redirect o/p stream to file
			char *fname = (i+1 < token_cnt)?old_tokens[++i]:NULL;
			int OUT_FD = open(fname, O_WRONLY|O_CREAT, 0666);

			#ifdef DEBUG
				printf("Attempting to redirect output to %s\n", fname);
			#endif

			if(OUT_FD == -1 || dup2(OUT_FD, STDOUT_FILENO) == -1) {
				perror("");
				#ifdef DEBUG
					printf("Failed.\n");
				#endif
				ERR_FLG = 1;
				return;
			}

		} else if(!strcmp(old_tokens[i], "<")) { //redirect i/p stream to file
			char *fname = (i+1 < token_cnt)?old_tokens[++i]:NULL;
			int IN_FD = open(fname, O_RDONLY);

			#ifdef DEBUG
				printf("Attempting to redirect input to %s\n", fname);
			#endif

			if(IN_FD == -1 || dup2(IN_FD, STDIN_FILENO) == -1) {
				perror("");
				#ifdef DEBUG
					printf("Failed.\n");
				#endif
				ERR_FLG = 1;
				return;
			}
		} else if(!strcmp(old_tokens[i], "&")) { //Process is to be executed in background
			
		} else { //The token is an argument for a process
			new_tokens[n++] = old_tokens[i];
		}
	}

	*tokens = new_tokens;
	token_cnt = n;
	(*tokens)[token_cnt] = NULL;
	return;
}

char **parse(char *line) {

	char **tokens = (char **) malloc(sizeof(char*) * 32);
	char *token = NULL;
	char *pwd = (char*) malloc(sizeof(char) * 512);

	token = strtok(line, DELIM_CHARS);

	while(token) {
		tokens[token_cnt++] = token;
		token = strtok(NULL, DELIM_CHARS);
	}

	#ifdef DEBUG
		printf("tokens are: ");
		for (i = 0; i < token_cnt; ++i)
			printf("%s,", tokens[i]);
		printf("\n");
	#endif
	
	return tokens;
}

void shellExecute(char **tokens) {

	if(!tokens[0]) return;
	int flg = 0;

	for(i=0 ; i<builtin_cnt() ; i++) {
		if(!strcmp(tokens[0], builtin_str[i])) {
			//Buit-in command
			flg = 1;
			(*builtin_func[i])(tokens);
		}
	}
	if(!flg) {
		//Not built-in command

		if(!strncmp("./", tokens[0], 2)) { //Search in current directory
			pid_t pid = fork();

			if(pid == 0) {
				if(execv(tokens[0], tokens) == -1) {
					perror("execv()");
					exit(0);
				}
			} else {
				wait(pid);
				return;
			}
		}

		char *ENV_PATH_CPY = (char*) malloc(strlen(ENV_PATH) * sizeof(char));
		strcpy(ENV_PATH_CPY, ENV_PATH);
		char *tmp = strtok(ENV_PATH_CPY, ":");

		struct stat check;
		int valid = 0;

		while(tmp) {
		
			char *path = (char*) malloc(strlen(tmp)+strlen(tokens[0])+2);
			strcpy(path, tmp);
			strcat(path, "/");
			strcat(path, tokens[0]);

			if(stat(path, &check) != -1) {

				#ifdef DEBUG
					printf("Valid path found: %s\n", path);
				#endif

				pid_t pid = fork();
				valid = 1;

				if(pid == 0) { //child
					#ifdef DEBUG
						printf("Inside child process for: %s\n", tokens[0]);
						printf("argv[] = { ");
						i = 0; while(tokens[i] != NULL) printf("%s ", tokens[i++]);
						printf("}\n");
					#endif
					if(execv(path, tokens) == -1) {
						perror("execv()");
						exit(ABNORMAL_EXIT);
					}
				} else if(pid != -1) { //parent
					wait(pid);
					#ifdef DEBUG
						printf("%s: Child has died.\n", tokens[0]);
					#endif
					return;
				} else {
					perror("fork()");
					return;
				}
			}
			tmp = strtok(NULL, ":");
		}
		if(!valid) {
			printf("%s: Invalid command\n", tokens[0]);
		}
	}
}

void reset() {
	token_cnt = 0;
	ERR_FLG = 0;
	dup2(STDIN_CPY, 0);
	dup2(STDOUT_CPY, 1);
}

void runShell() {

	size_t line_siz = 0;
	pwd = (char*) malloc(sizeof(char) * PWD_SIZ);
	getcwd(pwd, PWD_SIZ);
	strcat(pwd, "> ");

	shell_clear(NULL);

	while(1) {
		//Reset
		reset();
		//Get line from user
		line = readline(pwd);
		#ifdef DEBUG
			printf("line is '%s'\n", line);
		#endif
		//Parse line
		tokens = parse(line);
		preprocess(&tokens);
		if(!ERR_FLG) shellExecute(tokens);
	}
}

int main() {
	
	ENV_PATH = getenv("PATH");
	STDIN_CPY = dup(0);
	STDOUT_CPY = dup(1);
	runShell();
	return 0;

}
