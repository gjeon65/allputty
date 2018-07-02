#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUMTHRDS 4
#define VECLEN 100000
pthread_t callThd[NUMTHRDS];
double *array_a;
double *array_b;
double big_sum;
int veclen;

pthread_mutex_t lock;

void *dotprod(void *arg)
{
 int start = (long)arg * veclen;
 int end = start + veclen;
 double *x = array_a;
 double *y = array_b;
 int mysum = 0;
 for (int i=start; i<end ; i++)
  {
   mysum += (x[i] * y[i]);
  }
 //make immutable with lock
 pthread_mutex_lock(&lock);
 big_sum += mysum;
 pthread_mutex_unlock(&lock);
}

int main (int argc, char *argv[])
{
 long i;
 double *a, *b;
 void *status;
 a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
 b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
 for (i=0; i<VECLEN*NUMTHRDS; i++)
  {
   a[i]=1;
   b[i]=a[i];
  }
 veclen = VECLEN;
 array_a = a;
 array_b = b;
 big_sum = 0;
 
 if (pthread_mutex_init(&lock, NULL) != 0)
 {
	printf("\n mutex init has failed\n");
	return 1;
 }
 /* ... */
 /* create threads */
 /* ... */
 for(i=0;i<NUMTHRDS;i++)
  {
   /* Each thread works on a different set of data.
	  The offset is specified by 'i'. The size of
	  the data for each thread is indicated by VECLEN.
   */
  pthread_create(&callThd[i],NULL,dotprod,(void*)i);
  }
 /* Wait on the other threads */
 for(i=0;i<NUMTHRDS;i++)
 {
	 pthread_join(callThd[i],NULL);
 } 
 pthread_mutex_destroy(&lock);
 printf ("Sum = %f \n", big_sum);
 free (a);
 free (b);
}
