#include <stdio.h>
#include "csapp.h"

void *thread(void *vargp);

#define DEFAULT 4

int main(int argc, char* argv[]) {

	int N;
	if(argc>2)
		printf("error, too many param");
	else if (argc ==2)
		N = atoi(argv[1]);
	else
		N = DEFAULT;

	int i;
	//creating tread ID
	pthread_t tid;

	for (i = 0; i < N; i++) {
		Pthread_create(&tid, NULL, thread, NULL);
	}
	Pthread_exit(NULL);
}


//run thread
void *thread(void *vargp){
	printf("Hello, world \n");
	return NULL;
}
