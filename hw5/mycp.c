#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024
/*
filename: mycp
description: copy file
input: 2 files
output: copied file
*/
int main(int argc, char *argv[]){
	int fd1,fd2, count;
	char buf[MAX_BUF];
	
	if(argc!=3){
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	//original file
	if((fd1 = open(argv[1], O_RDONLY)) <0){
		perror("open");
		exit(1);
	}
	//copied file
	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) <0){
		perror("open");
		exit(1);
	}
	//read file using read function until output goes 0
	while((count=read(fd1, buf, MAX_BUF))>0){
		write(fd2, buf, count);
	}
	close(fd1);
	close(fd2);
}
