/******************************************************************************
 * FILE: posix-rt.c
 * DESCRIPTION:
 *   A simple pthread program illustrating RT pthread scheduling.
 *   Figure 5.25
 *   Operating System Concepts  - Tenth Edition
 *   Copyright John Wiley & Sons - 2018.
 *
 * (1) Compile this program:
 *	gcc posix-rt.c -o posix-rt -lpthread
 *
 * AUTHOR: Christer Karlsson
 * LAST REVISED: 02/28/20
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 5

/* the thread runs in this function */
void *runner(void *param); 

int main(int argc, char *argv[])
{
   int i, policy;
   pthread_t tid[NUM_THREADS];  /* the thread identifier */
   pthread_attr_t attr;         /* set of attributes for the thread */

   /* get the default attributes */
   pthread_attr_init(&attr);

   /* get the current scheduling policy */
   if (pthread_attr_getschedpolicy(&attr,&policy) != 0)
      fprintf(stderr, "Unable to get policy.\n");
   else {
      if (policy == SCHED_OTHER)
         printf("SCHED_OTHER\n");
      else if (policy == SCHED_RR)
         printf("SCHED_RR\n");
      else if (policy == SCHED_FIFO)
         printf("SCHED_FIFO\n");
   }
	
   /* set the scheduling policy - FIFO, RT, or OTHER */
   if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0)
      printf("unable to set scheduling policy to SCHED_FIFO \n");

   /* create the threads */
   for (i = 0; i < NUM_THREADS; i++) 
      pthread_create(&tid[i],&attr,runner,NULL); 

   /**
    * Now join on each thread
    */
   for (i = 0; i < NUM_THREADS; i++) 
      pthread_join(tid[i], NULL);

   return 0;
}

/**
 * The thread will begin control in this function.
 */
void *runner(void *param) 
{
	/* do some work ... */

	pthread_exit(0);
}

