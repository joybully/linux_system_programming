#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
/*
filename: stat.c
description: print file on argv[] based on stat
input: char *argv[]
output: informations about files
*/
	struct stat	statbuf;//struct of file state info
	char		*mode; //check on each file and store 
	int			i;

	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ", argv[i]);
		if (lstat(argv[i],&statbuf)<0 /* lstat */)  { //check the state of file  
			perror("lstat");	//print error if there's no right
			continue;
		}
		if (S_ISREG(statbuf.st_mode)) //the case of regular file
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode)) //the case of folder
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode)) //the case of char file
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode)) //the case of block file
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode)) //the case of FIFO file
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode)) //the case of symbolic file 
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode)) //the case of socket file for newworking 
			mode = "socket";
		// print information of the file
		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode);		//sort of file&file mode
		printf("\tst_ino = %ld\n", statbuf.st_ino);			// inode number
		printf("\tst_dev = %ld\n", statbuf.st_dev);			//file device id
		printf("\tst_rdev = %ld\n", statbuf.st_rdev);		//special file device id
		printf("\tst_nlink = %ld\n", statbuf.st_nlink);		// number of hard linked
		printf("\tst_uid = %d\n", statbuf.st_uid);			// id of file user
		printf("\tst_gid = %d\n", statbuf.st_gid);			// id of file group
		printf("\tst_size = %ld\n", statbuf.st_size);		//size of file
		printf("\tst_atime = %ld\n", statbuf.st_atime);		//last time of file access
		printf("\tst_mtime = %ld\n", statbuf.st_mtime);		//last time of file write
		printf("\tst_ctime = %ld\n", statbuf.st_ctime);		//last time of change of file status
		printf("\tst_blksize = %ld\n", statbuf.st_blksize);	//size of file I/O block
		printf("\tst_blocks = %ld\n", statbuf.st_blocks);	//number of 512B block
	}

	return 0;
}
