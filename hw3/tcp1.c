#include <stdio.h>
/*
filename: tcp1.c
description: change char to int(ascii code) and print on terminal
input: file, modified file(*argv[])
output: modified file
*/
int main(int argc, char *argv[])
{
	FILE	*src, *dst;
	int		ch;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src))!=EOF )  {
		printf("%d", ch);
		fputc(ch, dst);
	}

	fclose(src);
	fclose(dst);
}
