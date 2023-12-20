#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

void auth(mode_t file_mode){
/*
function name: auth
description: function for printing file right for UGO
input: file_mode
output: file right for user, group, other
*/
	if(S_ISDIR(file_mode)) printf("d");
	else if(S_ISREG(file_mode)) printf("r");
	else if(S_ISCHR(file_mode)) printf("c");
	else if(S_ISBLK(file_mode)) printf("b");
	else if(S_ISFIFO(file_mode)) printf("p");
	else printf("-");

	if(file_mode & S_IRUSR) printf("r");
	else printf("-");

	if(file_mode & S_IWUSR) printf("w");
	else  printf("-");
	
	if(file_mode & S_IXUSR) printf("x");
	else  printf("-");

	if(file_mode & S_IRGRP) printf("r");
	else  printf("-");

	if(file_mode & S_IWGRP) printf("w");
	else  printf("-");

	if(file_mode & S_IXGRP) printf("x");
	else  printf("-");

	if(file_mode & S_IROTH) printf("r");
	else  printf("-");

	if(file_mode & S_IWOTH) printf("w");
	else printf("-");

	if(file_mode & S_IXOTH) printf("x");
	else printf("-");

	printf(" ");
}

int main(){
/*
filename: mylsl.c
description: work like ls -l
input: none
output: none
*/
	struct passwd *pwd;
	DIR *dir;
	struct dirent *dentry;
	struct group *group;
	struct stat file_info;
	mode_t file_mode;
	struct tm *tminfo;
	int return_stat;
	char buf[100];
	char *month[] = {"Jan", "Feb","Mar","Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
	if((dir = opendir("./"))==NULL) exit(1);
	while(dentry = readdir(dir)){
		if((return_stat = stat(dentry->d_name, &file_info)) == -1){
			perror("dir_error : ");
			exit(0);
		}
		if(dentry->d_name[0]=='.') continue;
		else{
			file_mode = file_info.st_mode;
			auth(file_mode);
			printf(" %ld ", file_info.st_nlink);
			pwd = getpwuid(file_info.st_uid);
			printf("%s", pwd->pw_name);
			group = getgrgid(file_info.st_gid);
			printf(" %s ", group->gr_name);
			printf(" %5ld ", file_info.st_size);
			tminfo = localtime(&(file_info.st_mtime));
			printf(" %d %d %2d:%2d ", tminfo->tm_mon+1, tminfo->tm_mday, tminfo->tm_hour, tminfo->tm_min);
			if(dentry->d_ino != 0) printf(" %s\n", dentry->d_name);
		}	
	}
	closedir(dir);
	return 0;
}
