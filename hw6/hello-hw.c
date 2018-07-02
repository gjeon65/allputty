#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 8

char *messages[NUM_THREADS];
//getting arguments
struct arguments{
 int message_index;
 int sum;
 int task_id;
};

void *PrintHello(void *threadarg)
{
	//get arg from structure
 struct arguments *arg = threadarg;
 int taskid, sum;
 char *hello_msg;
 
 taskid = arg -> task_id;
 sum = arg -> sum;
 hello_msg = messages[arg -> message_index];

 printf("Thread %d %s Sum=%d\n", taskid, hello_msg, sum);
 pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
 pthread_t threads[NUM_THREADS];
 int rc, t, sum;
 sum=0;
 messages[0] = "Hello-0";
 messages[1] = "Hello-1";
 messages[2] = "Hello-2";
 messages[3] = "Hello-3";
 messages[4] = "Hello-4";
 messages[5] = "Hello-5";
 messages[6] = "Hello-6";
 messages[7] = "Hello-7";
 for(t = 0; t < NUM_THREADS; t++)
  {
   sum = sum + t;
   //get arg from structure
   struct arguments arg;
   
   arg.message_index = t;
   arg.sum = sum;
   arg.task_id = t;

   printf("Creating thread %d\n", t);

   pthread_create(&threads[t],NULL,PrintHello,(void*)&arg);
   pthread_join(threads[t],NULL);
  }
 pthread_exit(NULL);
}
