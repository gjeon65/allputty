#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

void forkexample()
{
	int x =1;
	if (fork() == 0)
		printf("child has x = %d\n", ++x);
	else
		printf("parent has x= %d\n", --x);
}

	void fork4()
	{
		printf("L0\n");
		if (fork() != 0)
		{
			printf("L1\n");
			if (fork() != 0)
			{
				printf("L2\n");
				fork();
			}
		}
		printf("Bye\n");
	}


int main(){
	//forkexample();
	fork4();
	return 0;
}
