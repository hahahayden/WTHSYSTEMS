#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int fd;
FILE *fp;

void read_file(int fd)
{
    const int size = 1;
    unsigned char buffer[size];
    int bytes_read;
    do
    {
        printf("h");
        bytes_read = read(fd, buffer, size);

    } while (bytes_read > 0);
    printf("%s\n", buffer);
}

void readfile(FILE *file)
{
    const int size = 2048;
    unsigned char buffer[size];
    int bytes_read;
    int value;
    int maxSize;
    int pageSize;
    while (!feof(file))
    {
        bytes_read = fread(buffer, 1, size, file);
        printf("%s\n", buffer);
    }
    value = sysconf(_SC_CHILD_MAX);
    printf("%d", value);
    maxSize = sysconf(_SC_OPEN_MAX);
    pageSize = sysconf(_SC_PAGE_SIZE);
    printf("%d", maxSize);
    printf("%d", pageSize);
}

void mallocTest()
{
    int *value;

    value = (int *)malloc(10000000);
    if (value == NULL)
    {
        printf("H");
    }
    else
    {
        printf("HERE");
    }
}
int main()
{
    fd = open("test.txt", O_RDONLY);
    fp = fopen("test.txt", "r");
    read_file(fd);
    readfile(fp);
    mallocTest();
    return 0;
}