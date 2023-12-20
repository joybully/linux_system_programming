#include <stdio.h>
/*
filename: tab.c
description: create file name which ends with '.t' and make tab to 4 blank spaces
input: files(*argv[])
output: files which has same data but different file name and tab to 4 blank spaces
using fputc, fdest, fopen
*/
enum { FALSE, TRUE };

void
conv(char *fname)
{
	FILE	*src, *dst;
	char	fdest[40];
	int		ch, first;

	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	}

	strcpy(fdest, fname);
	strcat(fdest, ".t");
	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");
		return;
	}

	first = TRUE;
	while ((ch = fgetc(src)) != EOF)  {
		if (first && ch == '\t')  {
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else  {
			fputc(ch, dst);
			if (first)
				first = FALSE;
			if (ch == '\n')
				first = TRUE;
		}
	}

	fclose(src);
	fclose(dst);
}

int main(int argc, char *argv[])
{
	while (--argc)  {
		conv(argv[argc]);
	}
}
