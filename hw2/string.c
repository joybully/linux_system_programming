#include <stdio.h>
//return length of the string using pointer
int strlen_p(char *str){
	int len = 0;
	//if the pointer goes to end, pointer value would be null char
	while(*str++){
		len++;
	}
	return len;
}

//return length of the string using array
int strlen_a(char str[]){
	int i;
	for(i = 0; str[i] != '\0'; i++);
	return i;
}

//copy string from source to destination using pointer
void strcpy_p(char *dst, char *src){
	while(*src){
		*dst = *src;
		*dst++;
		*src++;
	}
	*dst = *src;
}

//copy string from source to destination using array
void strcpy_a(char dst[], char src[]){
	int i;
	for(i = 0; src[i]!='\0';i++)
		dst[i] = src[i];
	dst[i] = src[i];
}

//add source string to destination using pointer
void strcat_p(char *dst, char *src){
	while(*dst++);
	dst--;
	while(*src){
		*dst++=*src++;
	}
	*dst = *src;
}

//add source string to destination using array
void strcat_a(char dst[], char src[]){
	int i, j;
	for(i=0; dst[i]!='\0';i++);
	for(j=0; src[j]!='\0';j++)
		dst[i+j] = src[j];
	dst[i+j] = src[j];
}

//compare two string
//if destination is bigger, return plus value
//if destination is smaller, return minus value
//if same, return 0
//using pointer
int strcmp_p(char *dst, char *src){
	while(*dst!='\0'||*src!='\0'){
		if(*dst!=*src)
			return(*dst-*src);
		dst++;
		src++;
	}
	return 0;
}

//compare two string
//if destination is bigger, return plus value
//if destinaiton is smaller, return minus value
//if same, return 0
//using array
int strcmp_a(char dst[], char src[]){
	int i=0;
	while(dst[i]!='\0'||src[i]!='\0'){
		if(dst[i]!=src[i])
			return (dst[i]-src[i]);
		i++;
	}
	return 0;
}

void main(){
	int len1, len2;
	char str1[20], str2[20];
	char str3[20];
	
	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p%d a=%d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	strcpy_p(str3, str1);
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);
	
	//p1 and p2 are result of function strcmp_p
	//a1 and a2 are result of function strcmp_a
	//p1 and a1 are compared result of same string
	//p2 and a2 are compared result of different string
	int result1 = strcmp_p(str1,str2);
	int result2 = strcmp_p(str1,str3);
	int result3 = strcmp_a(str1,str2);
	int result4 = strcmp_a(str1,str3);
	printf("strcmp: p1=%d, p2=%d, a1=%d, a2=%d\n",result1,result2,result3,result4);
}

