
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

int main(void)
{
    pid_t pid;

    printf("Hello, world!\n");

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(0);
    }

    if (pid == 0)
    {
        printf("This is the child, pid: %d\n", getpid());
        exit(0);
    }
    else
    {
        printf("This is the parent, pid: %d\n", getpid());
        wait(0);
        printf("This is the parent, pid: %d, signing off.\n", getpid());
    }

    return 0;
}