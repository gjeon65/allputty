/*
	Hw1. reverse file output
	Geun Jeon
	CS 283
*/
#include <stdio.h>
#include <string.h>
 
int main(char *argv[])
{
	FILE *fp1;
		      
	int count = 0;
	int i   = 0;
					     			      
	fp1 = fopen("test.txt","r");
	if( fp1 == NULL )
	{
		 printf("File can not be opened :");
		 return -1;
	}

	fseek(fp1,0,SEEK_END);
	count = ftell(fp1);
											       
	while( i < count )
	{
		i++;
		fseek(fp1,-i,SEEK_END);
		printf("%c",fgetc(fp1));
	}
		printf("\n");
		fclose(fp1);
															       
		return 0;
}
