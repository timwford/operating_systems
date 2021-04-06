/******************************************************************************
 * FILE: forks.c
 * DESCRIPTION:
 *   A program to time process creation and termination.
 * 
 * (1) Compile this program:
 *          gcc -o forks forks.c
 *
 * AUTHOR: Christer Karlsson
 * LAST REVISED: 02/28/20
******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

#define MAX_PROCESSES	 8192

/* Parallel function */
void PrintHello(long pid)
{
   //printf("Hello World! It's me, child #%ld!\n", pid);
}

int main(int argc, char *argv[])
{ 
   long      proc;
   pid_t     child_pid, wpid;
   struct    timeval start, end;
   double    time_spent;

   printf("In main: starting test\n");
   time_spent = 0.0;
   // Start timer
   gettimeofday(&start, 0);
   for(proc = 0; proc < MAX_PROCESSES; proc++) {
     if ((child_pid = fork()) == 0) {
       PrintHello(getpid());
       return 0;
     }
   }
   while ((wpid = wait(NULL)) > 0)
   // Stop timer
   gettimeofday(&end, 0);
   time_spent = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

   printf("The elapsed time is %8.2f ms\n", time_spent/1000);

   return 0;
}
