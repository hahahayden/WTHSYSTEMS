#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    /*
    if (argv[1] == 'c')
    {
        archiveCreation();
    }
    */
    char path[100];
    DIR filesArray[2100];
    printf("Enter path to list files: ");
    scanf("%s", path);
    int x = 0;
    listFilesRecursively(path, filesArray, x);
}
void listFilesRecursively(char *basePath, struct dirent filesArray, int count)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            filesArray[count] = dp;
            printf("%s\n", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
            count++;
        }
    }

    closedir(dir);
}