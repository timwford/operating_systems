/******************************************************************************
 * FILE: threads.c
 * DESCRIPTION:
 *   A Pthreads program to time thread creation and termination.
 *
 * (1) Compile this program:
 *          gcc -o threads threads.c -lpthread 
 * 
 * AUTHOR: Christer Karlsson
 * LAST REVISED: 02/27/20
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_THREADS	32700

/* Parallel function */
void *PrintHello(void *threadid)
{
   //printf("Hello World! It's me, thread #%ld!\n", (long)threadid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   pthread_t threads[MAX_THREADS];
   int       rc; 
   long      thread;
   struct    timeval start, end;
   double    time_spent;

   time_spent = 0.0;
   // Start timer
   gettimeofday(&start, 0);
   for(thread = 0; thread < MAX_THREADS; thread++) {
     rc = pthread_create(&threads[thread], NULL, PrintHello, (void *)thread);
     if (rc) {
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
     }
     //pthread_join(threads[thread], NULL);
   }
   for (thread = 0; thread < MAX_THREADS; thread++) 
	  pthread_join(threads[thread], NULL);
   // Stop timer
   gettimeofday(&end, 0);
   time_spent = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

   printf("The elapsed time is %8.2f ms\n", time_spent/1000);

   return 0;
}
