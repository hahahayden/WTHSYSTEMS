#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

void printFileProperties(struct stat stats);

void printFileProperties(struct stat stats)
{
    printf("%d", S_ISREG(stats.st_mode));

    if (S_ISREG(stats.st_mode) > 0)
    {
        printf("File");
    }
    else if (S_ISDIR(stats.st_mode))
    {
        printf("Directory");
    }
    else if (S_ISFIFO(stats.st_mode))
    {
        printf("Pipe");
    }

    printf("I-node number:            %ld\n", (long)stats.st_ino);
    printf("Link count:               %ld\n", (long)stats.st_nlink);
    printf("\nFile size: %d", stats.st_size);
}

/*
int file_access(struct stat stats, int x, char *str, int shift, int id)
{
    str[x] = ((stats->st_mode & (S_IROTH << shift)) ? 'r' : '-');
    x++;
    str[x] = ((stats->st_mode & (S_IWOTH << shift)) ? 'w' : '-');
    x++;
    if (id)
    {
        if (stats->st_mode & (S_ISGID << (shift - 3)))
        {
            str[x] = ((stats->st_mode & (S_IXGRP << (shift - 3))) ? 's' : 'S');
            x++;
        }
        else
        {
            str[x] - ((stats->st_mode & (S_IXGRP << (shift - 3))) ? 'x' : '-');
            x++;
        }
    }
    else
    {
        str[x] = ((stats->st_mode & S_IXOTH) ? 'x' : '-');
        x++;
    }
    return x;
}
*/
int main(int argc, char **argv)
{
    struct stat stats;
    char access[200];
    int x = 0;
    if (stat(argv[1], &stats) == 0)
    {
        printf("File Examining: %s\n", argv[1]);
        printFileProperties(stats);
        int fileMode = stats.st_mode;
        if ((fileMode & S_IRUSR))
            printf("r");
        else
            printf("-");
        if ((fileMode & S_IWUSR))
            printf("w");
        else
            printf("-");
        if ((fileMode & S_IXUSR))
            printf("x");
        else
            printf("-");
        /* Check group permissions */
        if ((fileMode & S_IRGRP))
            printf("r");
        else
            printf("-");
        if ((fileMode & S_IWGRP))
            printf("w");
        else
            printf("-");
        if ((fileMode & S_IXGRP))
            printf("x");
        else
            printf("-");
        /* check other user permissions */
        if ((fileMode & S_IROTH))
            printf("r");
        else
            printf("-");
        if ((fileMode & S_IWOTH))
            printf("w");
        else
            printf("-");
        if ((fileMode & S_IXOTH))
            /* because this is the last permission, leave 3 blank spaces after print */
            printf("x   ");
    }
    return 0;
}
