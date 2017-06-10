#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(void)
{
	int i = 9999;
	char name[20];

	itoa(i,name,10);
	printf("i=%s\n",name);
	return 0;
}
