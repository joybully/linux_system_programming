#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
/*
filename: hole.c
description: create file.hole and put char '\0' in front
input: none
ouput: file.hole
*/
int main(){
	int fd;
	char ch = '\n';
	//create file "file.hole"
	if((fd = creat("file.hole", 0400))<0){
		perror("creat");
		exit(1);
	}
	//put char '\0' in front
	if(lseek(fd, 1023,SEEK_SET)<0){
		perror("lseek");
		exit(1);
	}
	write(fd, &ch, 1);
	close(fd);
}
