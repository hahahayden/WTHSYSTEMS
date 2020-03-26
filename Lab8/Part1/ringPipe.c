#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{

    int fd1[2];
    int fd2[2];
    pid_t p;
    int myInt = 0;
    int buffer = 0;

    int status;
    scanf("%d", &myInt);

    fflush(stdout);
    while (myInt != EOF)
    {
        pipe(fd1);
        pipe(fd2);
        p = fork();

        if (p < 0)
        {
            fprintf(stderr, "fork Failed");
            return 1;
        }
        else if (p == 0) /*child*/
        {

            close(fd1[1]);
            int buffer2 = 0;

            read(fd1[0], &buffer2, sizeof(int));

            printf("buffer[0]: %d \n", buffer2);

            int newInt = buffer2 + 1;

            close(fd1[0]);
            printf("Result%i\n", newInt);
            fflush(stdout);
            write(fd2[1], &newInt, sizeof(int));
            exit(0);
        }
        else if (p > 0)
        {
            close(fd1[0]);

            fflush(stdout);

            int myNew = myInt * myInt;

            write(fd1[1], &myNew, sizeof(int));
            printf("Input Value from Parent: %d \n", myNew);
            close(fd1[1]);

            wait(NULL);
            printf("Child process exited with %d status\n", WEXITSTATUS(status));
            fflush(stdout);

            read(fd2[0], &buffer, sizeof(int));

            printf("Here is the final result: %i\n", buffer);
            fflush(stdout);
            close(fd2[1]);
        }

        scanf("%d", &myInt);
    }
}