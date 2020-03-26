#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

pid_t pid;
void handler(int num)
{
   printf("Killing\n");
   kill(getpid(), 9);
   kill(pid, 9);
}

int main(int argc, char **argv)
{
   struct sigaction sa;
   struct itimerval timer;
   char *cmd = calloc(2, sizeof(char));
   sa.sa_handler = handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   int cap = 0;
   for (int i = 2; i < argc; i++)
   {
      cap += strlen(argv[i]) + 2;
      realloc(cmd, cap);
      strcat(cmd, argv[i]);
      strcat(cmd, " ");
   }
   if (-1 == sigaction(SIGALRM, &sa, NULL))
   {
      perror("sigaction");
      exit(1);
   }
   if ((pid = fork()) != 0)
   {
      timer.it_interval.tv_usec = 0;
      timer.it_interval.tv_sec = 0;
      timer.it_value.tv_usec = 0;
      timer.it_value.tv_sec = atoi(argv[1]);

      if (-1 == setitimer(ITIMER_REAL, &timer, NULL))
         return -1;
      printf("Sleeping\n");
      system(cmd);
   }
   else
   {
      ;
   }
}