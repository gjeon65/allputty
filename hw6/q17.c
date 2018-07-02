/*
	Geun Jeon
	CS283
	HW6 Q12.17
	2018-03-15

	Q17A : main runs before thread runs completely  
*/

//Q17B:

#include "csapp.h"

void *thread(void *vargp);
int main()
{
	pthread_t tid;
	Pthread_create(&tid, NULL, thread, NULL);
	// exit(0); take this out
	Pthread_exit(NULL);
}
/* Thread routine */
void *thread(void *vargp)
{
	Sleep(1);
	printf("Hello, world!\n");
	return NULL;
}
