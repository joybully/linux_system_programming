#include<stdio.h>
#include<stdlib.h>

#define max_buf 1024

int main(int argc, char *argv[]){
	char buf[max_buf];
	if(argc!=3){
		fprintf(stderr, "Usage: %s destination\n", argv[0]);
	}
	char *str = argv[1];
	int num = atoi(argv[2]);
	for(int i=0; i<num; i++){
		printf("%c",str[i]);
	}
	printf("\n");
	return 0;
}
