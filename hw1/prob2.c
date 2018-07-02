/*
	hw1. prob2
	Geun Jeon
	Convert decimal to binary

*/

#include <stdio.h>
 
int main()
{
	int i, j, k;
	printf("input integer: ");
	scanf("%d", &i);
	printf("%d result: \n", i);
			   
	for (j= 31; j >= 0; j--)
	{
		k = i >> j;
		if (k & 1)
			printf("1");
		else
			printf("0");
	}
	printf("\n");
	return 0;
}
