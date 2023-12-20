#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *src;
	FILE *dst;
	int arr[20];
	if(argc!=3){
		fprintf(stderr, "Usage: %s destination\n", argv[0]);
	}
	
	if((src=fopen(argv[1], "r"))==NULL){
		perror("fopen");
		exit(1);
	}
	if((dst=fopen(argv[2],"w"))==NULL){
		perror("fopen");
		exit(1);
	}
	int num;
	int b=0;
	while(fscanf(src, "%d", &num)!=EOF){
		arr[b]=num;
		b++;
	}
	for(int i=0; i<10; i++){
		int index = i;
		for(int j=i+1;j<10; j++){
			if(arr[j]<arr[index])
				index = j;
		}
		int temp = arr[i];
		arr[i] = arr[index];
		arr[index] = temp;
		
	}
	char buf[20];
	for (int i=0;i<10; i++){
		buf[i*2]=(char)(arr[i]+48);
		buf[i*2+1] = ' ';
	}

	for(int i=0; i<20;i++){
		putc(buf[i],dst);
	}
//	fwrite(20,1,20,dst);	
	return 0;
}
