#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_BUF 1024
/*
filename: upper.c
description: open file and convert chars to upper chars, store in file(*argv[])
input: original file(argv[1]) and *argv[]
output: file to store (argv[2])
*/
/*
how to use
gcc -o upper upper.c (or just make)
touch a
touch b
vi a(and edit file whatever you want)
./upper a b
*/
int main(int argc, char* argv[]){
	int  fd1, fd2, count;
	char buf[MAX_BUF];
	char store[MAX_BUF];
	
	if(argc!=3){
		printf("Usage: %s source destinaiton\n", argv[0]);
		exit(1);
	}
	if((fd1=open(argv[1], O_RDONLY))<0){
		perror("fopen");
		exit(1);
	}
	if((fd2=open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644))<0){
		perror("open");
		exit(1);
	}
	while((count=read(fd1, buf, MAX_BUF))>0){
		for(int i=0; i<strlen(buf); i++){
			store[i] = toupper(buf[i]);
		}
		write(fd2, store, count);
	}
	close(fd1);
	close(fd2);
	return 0;
}
