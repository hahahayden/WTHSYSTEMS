#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <libgen.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int check, status;

    if (argc < 2 || argc > 2)
    {
        fprintf(stderr, "usage: tryit command\n");
        exit(0);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(0);
    }

    if (pid == 0)
    {
        if ((check = execv(argv[1], &argv[1])) < 0)
        {
            printf("here");
            perror(argv[1]);
            exit(1);
        }
        exit(0);
    }
    else
    {
        wait(&status);
        if (status == 0)
        {
            printf("Process %d succeeded.\n", getpid());
        }
        else
        {
            printf("Process %d exited with an error value.\n", getpid());
        }
    }

    return 0;
}