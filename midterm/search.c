#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
int main(int argc, char *argv[]){
	FILE *src;
	if(argc!=3){
		fprintf(stderr,"Usage: %s destiantion\n", argv[0]);
	}
	if((src=fopen(argv[1], "r"))==NULL){
		perror("fopen");
		exit(1);
	}
	int arr[20];
	int num2;
	int b=0;
	int num1 = atoi(argv[2]);
	while(fscanf(src, "%d", &num2)!=EOF){
		arr[b] = num2;
		b++;
}
	bool flag = false;
	for(int i=0; i<10; i++){
		if(num1==arr[i])
			printf(num1);
	}
	

	return 0;
}
