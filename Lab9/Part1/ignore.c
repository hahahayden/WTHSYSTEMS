#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void handler(int num)
{
   write(STDOUT_FILENO, "Invincible! Kill with ^Z\n", 26);
}

int main(int argc, char **argv)
{
   char *string = malloc(100000);

   while (1)
   {
      signal(SIGINT, handler);
      scanf("%s", string);

      if (strcmp(string, "quit") == 0)
      {
         printf("here");

         exit(1);
      }
   }
}