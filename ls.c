#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void findPermissions(char perms[], struct stat *s)
{
	strcpy(perms,"----------\0");
	int mode = (int) s->st_mode;

	if(S_ISDIR(s->st_mode)) perms[0] = 'd';

	if(S_IRUSR & mode) perms[1] = 'r';
	if(S_IWUSR & mode) perms[2] = 'w';
	if(S_IXUSR & mode) perms[3] = 'x';
	
	if(S_IRGRP & mode) perms[4] = 'r';
	if(S_IWGRP & mode) perms[5] = 'w';
	if(S_IXGRP & mode) perms[6] = 'x';

	if(S_IROTH & mode) perms[7] = 'r';
	if(S_IWOTH & mode) perms[8] = 'w';
	if(S_IXOTH & mode) perms[9] = 'x';

	return;
}

int main(int argc , char *argv[])
{
	int i, path_index = -1;
	long long int file_size = 0;
	char *path = (char*) malloc(sizeof(char)*256);
	char *mtime = (char*) malloc(sizeof(char)*64);
	char perms[] = "----------\0";
	bool FLG_LIST = false, FLG_ALL = false, FLG_RECUR = false;
	DIR *dp;
	struct stat file_info;
	struct dirent *entry = NULL;
	struct tm *mod_time;
	struct passwd *user_info;
	struct group *grp_info;

	for(i=1 ; i<argc ; i++) {
		if(argv[i][0] == '-') {
			if(!strcmp(argv[i],"-l"))
				FLG_LIST = true;
			else if(!strcmp(argv[i],"-a"))
				FLG_ALL = true;
			else if(!strcmp(argv[i],"-R"))
				FLG_RECUR = true;
		} else { 
			strcpy(path, argv[i]);
			path_index = i;
		}
	}

	if(!strlen(path))
		strcpy(path , ".\0");
	
	dp = opendir(path);

	if(!dp) {
		perror("opendir()");
		exit(2);
	}

	while(entry = readdir(dp)) {
		if(FLG_ALL || entry->d_name[0] != '.') {
			if(FLG_LIST) {
				
				if(path_index != -1) strcpy(path, argv[path_index]);
				else strcpy(path, ".");

				strcat(path, "/");
				strcat(path, entry->d_name);

				if(-1 == stat(path, &file_info)) {perror("stat()"); exit(1);}

				mod_time = localtime(&file_info.st_atime);
				user_info = getpwuid(file_info.st_uid);
				grp_info = getgrgid(file_info.st_gid);

				strftime(mtime, 64, "%b %d %R", mod_time);
				findPermissions(perms, &file_info);
				file_size = (long long int)file_info.st_size;
				
				printf("%s  %lli  \t%s\t%s\t%s\t%s\n",
						perms,
						file_size,
						user_info->pw_name,
						grp_info->gr_name,
						mtime,
						entry->d_name);
			} else {
				printf("%s  " , entry->d_name);
			}
		}
	}
	closedir(dp);
	printf("\n");

	return 0;
}
