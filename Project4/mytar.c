#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "mytar.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <arpa/inet.h>
#include <utime.h>

void getMode(struct stat fileStat, char *mode)
{
    int c;
    c = 0;
    if (fileStat.st_mode & S_IRUSR)
        c += 4;
    if (fileStat.st_mode & S_IWUSR)
        c += 2;
    if (fileStat.st_mode & S_IXUSR)
        c += 1;
    mode[4] = c + '0';
    c = 0;
    if (fileStat.st_mode & S_IRGRP)
        c += 4;
    if (fileStat.st_mode & S_IWGRP)
        c += 2;
    if (fileStat.st_mode & S_IXGRP)
        c += 1;
    mode[5] = c + '0';
    c = 0;
    if (fileStat.st_mode & S_IROTH)
        c += 4;
    if (fileStat.st_mode & S_IWOTH)
        c += 2;
    if (fileStat.st_mode & S_IXOTH)
        c += 1;
    mode[6] = c + '0';
    mode[7] = '\0';
}

void int_to_octal(int num, char *result)
{
    int arr[] = {40000, 8000, 4096, 512, 64, 8, 1};
    for (int i = 0; i < 7; i++)
    {
        if ((num / arr[i]) > 0)
        {
            result[i] = num / arr[i] + '0';
            num -= (num / arr[i]) * arr[i];
        }
        else
            result[i] = '\0';
    }
    result[7] = '\0';
}

char getFlag(struct stat fileStat)
{
    if (S_ISREG(fileStat.st_mode))
        return '0';
    else if (S_ISDIR(fileStat.st_mode))
        return '5';
    else if (S_ISLNK(fileStat.st_mode))
        return '2';
}

int insert_special_int(char *attribute, size_t size, int32_t val)
{
    int err = 0;
    if (val < 0 || size < sizeof(val))
        err++;
    else
    {
        memset(attribute, 0, size);
        *(int32_t *)(attribute + size - sizeof(val)) = htonl(val);
        *attribute |= 0x80;
    }
    return err;
}

int insert_octal(char *attribute, size_t size, int val)
{
    char buff[13];
    int err = 0;
    int len;

    sprintf(buff, "%0*o", (int)size - 1, val);
    len = strlen(buff);

    if (len > size - 1)
    {
        if (insert_special_int(attribute, size, val))
        {
            err++;
        }
    }
    else
    {
        memmove(attribute, buff, len);
    }
    return err;
}

void createHeader(char *filepath, char *filename, int tarfile, int verbose)
{
    struct stat fileStat;
    struct header *header = calloc(1, sizeof(struct header));
    unsigned char c = 0;
    int j = 100;
    int i = 0;
    unsigned sum = 0;
    int location;
    int count;
    int bytes_read;
    int fsize;
    int over = 0;
    int fd;

    char name[100] = "\0";
    char mode[9] = "00000000";
    char magic[6] = "ustar";
    char version[2] = "00";
    char userid[9] = "0";
    char groupid[9] = "0";
    char size[13] = "0";
    char mtime[13] = "0";
    char user[32];
    char typeflag;
    char checksum[8] = "\0";
    char link[100] = "\0";
    char prefix[155] = "\0";

    char *null = calloc(1024, sizeof(char));
    char *path = calloc(strlen(filepath) + strlen(filename) + 2, sizeof(char));
    strcpy(path, filepath);
    strcat(path, filename);
    int h = 0;
    int y = 0;
    int d = 0;
    if (strlen(path) > 100)
    {
        while (h < strlen(path))
        {
            if (path[h] != '/')
            {
                prefix[h] = path[h];
            }
            else if (path[h] == '/')
            {

                if (strlen(path) - h < 100)
                {

                    for (y = h + 1; y < strlen(path); y++)
                    {
                        name[d] = path[y];
                        d++;
                    }
                    name[d + 1] = '\0';
                    break;
                }
                else
                {
                    prefix[h] = '/';
                }
            }
            h++;
        }
        strncpy(header->name, name, 100);
        strncpy(header->prefix, prefix, 155);
    }
    else
    {
        strncpy(name, path, 100);
        strncpy(header->prefix, prefix, 155);
    }

    prefix[155] = '\0';
    name[100] = '\0';
    if ((fd = open(path, O_RDONLY)) == -1)
    {
        fprintf(stderr, "%s: No such file or directory\n", filename);
        return;
    }
    fsize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    unsigned char *bufferWrite = calloc(fsize, sizeof(unsigned char));

    if (lstat(path, &fileStat) < 0)
    {
        fprintf(stderr, "Stat Error\n");
        return;
    }
    if (S_ISDIR(fileStat.st_mode) && (path[strlen(path) - 1]) != '/')
    {
        return;
    }
    int padNeeded = fsize % 512;
    int numOfBlocks = fsize / 512;
    if (padNeeded > 0)
        numOfBlocks++;

    if (S_ISDIR(fileStat.st_mode))
        typeflag = '5';
    else if (S_ISLNK(fileStat.st_mode))
        typeflag = '2';
    else if (S_ISREG(fileStat.st_mode))
        typeflag = '0';
    getMode(fileStat, mode);
    sprintf(userid, "%07o", fileStat.st_uid);
    sprintf(groupid, "%07o", fileStat.st_gid);
    sprintf(size, "%011lo", fileStat.st_size);
    sprintf(mtime, "%011lo", fileStat.st_mtime);
    /*uid_t uid = geteuid();*/
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *grp;
    strcpy(user, pw->pw_name);
    /*getlogin_r(user,32);*/
    /*userid[7] = '\0';
   groupid[7] = '\0';
   size[11] = '\0';
   mtime[11] = '\0';*/
    if (strlen(mode) > 7)
    {
        if (insert_octal(header->mode, 8, (fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO))) != 0)
            fprintf(stderr, "Mode Write Failed!\n");
    }
    else
        strcpy(header->mode, mode);
    if (strlen(userid) > 7)
    {
        if (insert_octal(header->uid, 8, fileStat.st_uid) != 0)
            fprintf(stderr, "Uid Write Failed!\n");
    }
    else
        strcpy(header->uid, userid);
    if (strlen(groupid) > 7)
    {
        if (insert_octal(header->gid, 8, fileStat.st_gid) != 0)
            fprintf(stderr, "Gid Write Failed!\n");
    }
    else
        strcpy(header->gid, groupid);
    if (strlen(mtime) > 11)
    {
        printf("Hello");
        if (insert_octal(header->mtime, 12, fileStat.st_mtime) != 0)
            fprintf(stderr, "Mtime Write Failed!\n");
    }
    else
        strcpy(header->mtime, mtime);

    /*printf("%lu\n",fileStat.st_mtime);*/
    grp = getgrgid(fileStat.st_gid);
    if (S_ISLNK(fileStat.st_mode) || S_ISDIR(fileStat.st_mode))
        sprintf(size, "%011o", 0);
    if (strlen(size) > 11)
    {
        if (insert_octal(header->size, 12, fileStat.st_size) != 0)
            fprintf(stderr, "Size Write Failed!\n");
    }
    else
        strcpy(header->size, size);
    strncpy(header->name, name, 100);
    strncpy(header->magic, magic, 6);
    strncpy(header->version, version, 2);
    strncpy(header->uname, user, 32);
    strncpy(header->linkname, link, 100);
    header->typeflag = typeflag;
    strncpy(header->gname, grp->gr_name, 32);
    strncpy(header->prefix, prefix, 155);

    for (i = 0; i < 155; i++)
    {
        sum += (unsigned char)header->prefix[i];
    }
    for (i = 0; i < 100; i++)
    {
        sum += ((unsigned char)header->name[i]);
        sum += ((unsigned char)header->linkname[i]);
    }
    for (i = 0; i < 8; i++)
    {
        sum += abs((unsigned char)header->mode[i]);
        sum += abs((unsigned char)header->uid[i]);
        sum += abs((unsigned char)header->gid[i]);
    }
    for (i = 0; i < 12; i++)
    {
        sum += abs((unsigned char)header->mtime[i]);
        sum += abs((unsigned char)header->size[i]);
    }
    for (i = 0; i < 32; i++)
    {
        sum += ((unsigned char)header->uname[i]);
        sum += ((unsigned char)header->gname[i]);
    }
    for (i = 0; i < 6; i++)
        sum += ((unsigned char)header->magic[i]);
    for (i = 0; i < 2; i++)
        sum += ((unsigned char)header->version[i]);
    sum += ((unsigned char)header->typeflag);
    sum += (unsigned char)256;
    sprintf(checksum, "%07o", sum);

    if (strlen(checksum) > 7)
    {
        if (insert_octal(header->chksum, 8, sum) != 0)
            fprintf(stderr, "Checksum Write Failed!\n");
    }
    else
        strcpy(header->chksum, checksum);
    write(tarfile, header->name, 100 * sizeof(uint8_t));
    write(tarfile, header->mode, 8 * sizeof(uint8_t));
    write(tarfile, header->uid, 8) * sizeof(uint8_t);
    write(tarfile, header->gid, 8 * sizeof(uint8_t));
    write(tarfile, header->size, 12 * sizeof(uint8_t));
    write(tarfile, header->mtime, 12 * sizeof(uint8_t));
    write(tarfile, header->chksum, 8 * sizeof(uint8_t));
    write(tarfile, &header->typeflag, 1 * sizeof(uint8_t));
    write(tarfile, header->linkname, 100 * sizeof(uint8_t));
    write(tarfile, header->magic, 6 * sizeof(uint8_t));
    write(tarfile, header->version, 2 * sizeof(uint8_t));
    write(tarfile, header->uname, 32 * sizeof(uint8_t));
    write(tarfile, header->gname, 32 * sizeof(uint8_t));
    write(tarfile, header->devmajor, 8 * sizeof(uint8_t));
    write(tarfile, header->devminor, 8 * sizeof(uint8_t));
    write(tarfile, header->prefix, 155 * sizeof(uint8_t));
    write(tarfile, null, 12 * sizeof(uint8_t));
    i = 0;
    if (header->typeflag != '5')
    {
        bytes_read = read(fd, bufferWrite, fileStat.st_size);
        write(tarfile, bufferWrite, fileStat.st_size);
        write(tarfile, null, (512 * numOfBlocks) - fileStat.st_size);
    }
    close(fd);
    if (verbose == 1)
        printf("%s Added\n", path);
    free(header);
    free(bufferWrite);
}

void listFilesRecursively(char *basePath, int tarfile, int mode)
{
    char path[256];
    struct stat fileStat;
    struct dirent *dp;
    unsigned int c;
    unsigned int checksum;
    DIR *dir = opendir(basePath);
    if (!dir)
    {
        return;
    }
    if (basePath[strlen(basePath) - 1] != '/')
    {
        strcat(basePath, "/");
    }
    createHeader("", basePath, tarfile, mode);
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, dp->d_name);
            createHeader(basePath, dp->d_name, tarfile, mode);
            listFilesRecursively(path, tarfile, mode);
        }
    }
    closedir(dir);
}

int extract(char *path, char *tarfile, int argc)
{
    int fdTar, fdFile, fileSize, mode;
    char *end;
    struct stat fileStat4;
    char dir[255] = "";
    char *xBuff[514];
    int overflow;
    int fsize;
    struct header headerX;
    memset(&headerX, 0, sizeof(headerX));
    int check = 0;
    struct utimbuf modtime;
    char MAGIC[6] = "ustar";
    int fd6 = open(tarfile, O_RDONLY);
    fflush(stdout);
    if (stat(tarfile, &fileStat4) < 0)
    {
        fprintf(stderr, "h./mytar [x] f .tar path");
        return 0;
    }
    if (argc < 4)
    {
        while (read(fd6, &headerX, 512) != 0)
        {
            if (strstr(headerX.magic, MAGIC) != NULL)
            {
                char name[100] = "";
                strncpy(name, headerX.name, 100);
                char *filepath = calloc(255, sizeof(char));
                if (headerX.prefix[0] != '\0')
                {
                    strcat(filepath, headerX.prefix);
                    strcat(filepath, "/");
                }
                strcat(filepath, name);
                mode = strtol(headerX.mode, NULL, 8);
                if (path != NULL)
                {
                    if (strstr(headerX.name, path) == NULL)
                    {
                        if (headerX.typeflag == '5')
                        {
                            mode = strtol(headerX.mode, NULL, 8);
                            mkdir(headerX.name, mode);
                            memset(&headerX, 0, sizeof(headerX));
                            continue;
                        }
                        else if (headerX.typeflag == '2')
                        {
                            symlink(filepath, headerX.linkname);
                            memset(&headerX, 0, sizeof(headerX));
                            continue;
                        }
                        memset(&headerX, 0, sizeof(headerX));
                        continue;
                    }
                }

                if (headerX.typeflag == '5')
                {
                    strcpy(dir, filepath);
                    mode = strtol(headerX.mode, NULL, 8);
                    mkdir(filepath, mode);
                    memset(&headerX, 0, sizeof(headerX));
                    continue;
                }
                else if (headerX.typeflag == '2')
                {
                    char linkname[255];
                    strcpy(linkname, dir);
                    strcat(linkname, headerX.linkname);
                    mode = strtol(headerX.mode, NULL, 8);
                    fdFile = open(linkname, O_CREAT | O_TRUNC | O_WRONLY, mode);
                    fsize = strtol(headerX.size, NULL, 8);
                    modtime.modtime = strtol(headerX.mtime, NULL, 8);
                    modtime.actime = strtol(headerX.mtime, NULL, 8);
                    utime(headerX.name, &modtime);
                    symlink(headerX.linkname, filepath);
                    memset(&headerX, 0, sizeof(headerX));
                    continue;
                }
                fdFile = open(filepath, O_CREAT | O_TRUNC | O_WRONLY, mode);
                if (fdFile < 0)
                {
                    printf("Error in File: %s|%i\n", filepath, errno);
                    return 1;
                }

                name[100] = '\0';

                fsize = strtol(headerX.size, NULL, 8);
                char *buf = calloc(fsize, sizeof(char));
                read(fd6, buf, fsize);
                write(fdFile, buf, fsize);
                overflow = 512 - (fsize % 512);
                lseek(fd6, overflow, SEEK_CUR);
                modtime.modtime = strtol(headerX.mtime, NULL, 8);
                modtime.actime = strtol(headerX.mtime, NULL, 8);
                utime(headerX.name, &modtime);
                memset(&headerX, 0, sizeof(headerX));
                free(buf);
            }
        }
    }
    else
    {
        while (read(fd6, &headerX, 512) != 0)
        {
            if (strcmp(headerX.magic, MAGIC) == 0)
            {
                char name[100] = "";
                strncpy(name, headerX.name, 100);
                char *filepath = calloc(255, sizeof(char));
                if (headerX.prefix[0] != '\0')
                {
                    strcat(filepath, headerX.prefix);
                    strcat(filepath, "/");
                }
                strcat(filepath, name);
                mode = strtol(headerX.mode, NULL, 8);

                if (strstr(path, filepath) != NULL || strstr(filepath, path) != NULL)
                {
                    if (headerX.typeflag == '5')
                    {
                        mode = strtol(headerX.mode, NULL, 8);
                        mkdir(filepath, mode);
                        memset(&headerX, 0, sizeof(headerX));
                        continue;
                    }
                    else if (headerX.typeflag == '2')
                    {
                        symlink(filepath, headerX.linkname);
                        memset(&headerX, 0, sizeof(headerX));
                        continue;
                    }
                    if ((strcmp(path, filepath) == 0 && headerX.typeflag == '0') || (strcmp(path, filepath) != 0 && (strstr(filepath, path) != NULL)))
                    {
                        fdFile = open(filepath, O_CREAT | O_TRUNC | O_WRONLY, mode);
                        if (fdFile < 0)
                        {
                            printf("Error in File: %s|%i\n", filepath, errno);
                            return 1;
                        }
                        name[100] = '\0';
                        fsize = strtol(headerX.size, NULL, 8);
                        char *buf = calloc(fsize, sizeof(char));
                        read(fd6, buf, fsize);
                        write(fdFile, buf, fsize);
                        overflow = 512 - (fsize % 512);
                        lseek(fd6, overflow, SEEK_CUR);
                        modtime.modtime = strtol(headerX.mtime, NULL, 8);
                        modtime.actime = strtol(headerX.mtime, NULL, 8);
                        utime(headerX.name, &modtime);
                        memset(&headerX, 0, sizeof(headerX));
                        free(buf);
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }

    close(fd6);
}

int main(int argc, char **argv)
{
    struct stat fileStat;
    int fd2;
    int i = 0;
    int options;
    int countC = 0;
    int countT = 0;
    int countF = 0;
    int countX = 0;
    int countV = 0;
    int flag = 0;
    char *null = calloc(1024, sizeof(char));
    for (i = 0; i < strlen(argv[1]); i++)
    {
        if (argv[1][i] == 'c')
            countC += 1;
        else if (argv[1][i] == 'f')
            countF += 1;
        if (argv[1][i] == 'v')
            countV += 1;
        else if (argv[1][i] == 't')
            countT += 1;
        else if (argv[1][i] == 'x')
            countX += 1;
    }
    if (countF == 1 && countC == 1 && argc < 4)
    {
        fprintf(stderr, "./mytar [c]f .tar path\n");
        flag = 1;
        return 0;
    }
    else if (countF == 0)
    {
        fprintf(stderr, "./mytar [ctvx] f .tar path\n");
        return 0;
    }
    if (countC == 1 && ((countT && countX) == 0)) /* only creating the file*/
    {
        int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);

        for (int i = 3; i < argc; i++)
        {
            if (lstat(argv[i], &fileStat) < 0)
                continue;
            if (S_ISDIR(fileStat.st_mode))
                listFilesRecursively(argv[i], fd, countV);
            else
                createHeader("", argv[i], fd, countV);
        }
        write(fd, null, 1024);
    }
    else if (countX == 1 && ((countT && countC && countV) == 0))
    {
        if (argc > 3)
        {
            for (i = 3; i < argc; i++)
            {
                extract(argv[i], argv[2], argc);
            }
        }
        else
        {
            extract(NULL, argv[2], argc);
        }
    }
    else if (countT == 1 && ((countV == 0 && countC == 0 && countX == 0)))
    {
        char *tBuff[6];
        int sizeT = 0;
        int count2 = 0;
        char MAGIC[6] = "ustar";
        struct stat fileStat2;
        struct header headerT;
        int check = 0;
        fd2 = open(argv[2], O_RDONLY);
        if (fd2 < 0)
        {
            fprintf(stderr, "bad tar");
            return 0;
        }
        if (stat(argv[2], &fileStat2) < 0)
        {
            fprintf(stderr, "h./mytar [t] f .tar path");
            return 0;
        }
        int flag = 0;
        while (sizeT = read(fd2, &headerT, 512) != 0)
        {
            if (((strstr(headerT.magic, MAGIC) == NULL) && (flag == 0)))
            {
                fprintf(stderr, "BAD TAR");
                return 1;
            }
            else
            {
                flag = 1;
                if (((strcmp(headerT.magic, MAGIC) == 0) || strcmp(headerT.magic, "ustar  ") == 0) && argc == 3)
                {

                    if (headerT.prefix[0] != '\0')
                    {
                        printf("%s/", headerT.prefix);
                    }

                    printf("%.*s\n", 100, headerT.name);
                }
                else if (((strcmp(headerT.magic, MAGIC) == 0) || strcmp(headerT.magic, "ustar  ") == 0) && argc >= 3)
                {
                    for (int i = 3; i < argc; i++)
                    {
                        char path[255] = "";
                        if (strcmp(headerT.name, argv[i]) == 0)
                        {
                            if (headerT.prefix[0] != '\0')
                            {

                                printf(" %s/", headerT.prefix);
                                fflush(stdout);
                            }
                            else
                            {
                                printf("%.*s\n", 100, headerT.name);
                                fflush(stdout);
                            }
                        }
                        else
                        {
                            if (headerT.prefix[0] != '\0')
                            {
                                strcat(path, headerT.prefix);
                                strcat(path, "/");
                                strcat(path, headerT.name);
                            }
                            else if (headerT.prefix[0] == '\0')
                            {
                                strcat(path, headerT.name);
                            }
                            if (strcmp(path, argv[i]) == 0)
                            {
                                printf("%.*s\n", 255, path);
                            }
                            else if (strstr(path, argv[i]))
                            {
                                printf("%.*s\n", 100, path);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (countT == 1 && countV == 1 && ((countC && countX) == 0)) /* list all the files verbose*/
    {
        struct stat fileStat3;
        char *tBuff[514];
        int fd3;
        int sizeTV = 0;
        int count2 = 0;
        char MAGIC[6] = "ustar";
        fd3 = open(argv[2], O_RDONLY);
        if (stat(argv[2], &fileStat3) < 0)
        {

            fprintf(stderr, "h./mytar [t] f .tar path");
            return 0;
        }

        struct header headerTV;
        int check = 0;

        while (sizeTV = read(fd3, &headerTV, 512) != 0)
        {

            if ((((strcmp(headerTV.magic, MAGIC) == 0) || strcmp(headerTV.magic, "ustar  ") == 0) && argc == 3))
            {
                if (headerTV.typeflag == '5')
                {
                    printf("%c", 'd');
                }
                else if (headerTV.typeflag == '2')
                {
                    printf("%c", 'l');
                }
                else if (headerTV.typeflag == '0')
                {
                    printf("%c", '-');
                }
                for (int i = 4; i < 8; i++)
                {
                    switch (headerTV.mode[i])
                    {
                    case '7':
                        printf("rwx");
                        break;
                    case '6':
                        printf("rw-");
                        break;
                    case '5':
                        printf("r-x");
                        break;
                    case '4':
                        printf("r--");
                        break;
                    case '3':
                        printf("-wx");
                        break;
                    case '2':
                        printf("-w-");
                        break;
                    case '1':
                        printf("--x");
                        break;
                    case '0':
                        printf("---");
                        break;
                    }
                }
                printf(" ");
                int z = 0;
                int printCount = 0;
                for (z = 0; z < 32; z++)
                {
                    if (headerTV.uname[z] != '\0')
                    {
                        printCount += 1;
                        printf("%c", headerTV.uname[z]);
                    }
                    else if (headerTV.uname[z] == '\0')
                    {
                        printf("/");
                        break;
                    }
                }
                printCount++;
                z = 0;
                for (printCount; printCount < 18; printCount++) /* spacing check*/
                {
                    if (headerTV.gname[z] != '\0')
                    {
                        printf("%c", headerTV.gname[z]);
                    }
                    z++;
                }
                char *ptr;
                ptr = strtol(headerTV.size, NULL, 8);
                printf("%15d ", ptr);
                time_t total = strtol(headerTV.mtime, NULL, 8);
                char time[18];
                strftime(time, 18, "%Y-%m-%d %H:%M", localtime(&total));
                printf("%s ", time);
                printCount = 0;
                if (headerTV.prefix[0] != '\0')
                {
                    printf("%s/", headerTV.prefix);
                }
                printf("%.*s\n", 100, headerTV.name);
            }

            else if (strcmp(headerTV.magic, MAGIC) == 0 && argc >= 4)
            {
                for (int i = 3; i < argc; i++)
                {
                    char path[255] = "";
                    if (headerTV.prefix[0] != '\0')
                    {
                        strcat(path, headerTV.prefix);
                        strcat(path, "/");
                        strcat(path, headerTV.name);
                    }
                    else if (headerTV.prefix[0] == '\0')
                    {
                        strcat(path, headerTV.name);
                    }
                    if (strcmp(argv[i], headerTV.name) == 0 || strcmp(argv[i], path) == 0 || strstr(path, argv[i]))
                    {
                        if (headerTV.typeflag == '5')
                        {

                            printf("%c", 'd');
                        }
                        else if (headerTV.typeflag == '2')
                        {
                            printf("%c", 'l');
                        }
                        else if (headerTV.typeflag == '0')
                        {
                            printf("%c", '-');
                        }
                        for (int i = 4; i < 8; i++)
                        {
                            switch (headerTV.mode[i])
                            {
                            case '7':
                                printf("rwx");
                                break;
                            case '6':
                                printf("rw-");
                                break;
                            case '5':
                                printf("r-x");
                                break;
                            case '4':
                                printf("r--");
                                break;
                            case '3':
                                printf("-wx");
                                break;
                            case '2':
                                printf("-w-");
                                break;
                            case '1':
                                printf("--x");
                                break;
                            case '0':
                                printf("---");
                                break;
                            }
                        }
                        printf(" ");
                        int z = 0;
                        int printCount = 0;
                        for (z = 0; z < 32; z++)
                        {
                            if (headerTV.uname[z] != '\0')
                            {
                                printCount += 1;
                                printf("%c", headerTV.uname[z]);
                            }
                            else if (headerTV.uname[z] == '\0')
                            {
                                printf("/");
                                break;
                            }
                        }
                        printCount++;
                        z = 0;
                        for (printCount; printCount < 18; printCount++) /* spacing check*/
                        {

                            if (headerTV.gname[z] != '\0')
                            {
                                printf("%c", headerTV.gname[z]);
                            }
                            z++;
                        }
                        char *ptr;
                        ptr = strtol(headerTV.size, NULL, 8);
                        printf("%16d", ptr);

                        time_t total = strtol(headerTV.mtime, NULL, 8);
                        char time[18];
                        strftime(time, 18, "%Y-%m-%d %H:%M", localtime(&total));
                        printf("%s", time);
                        printCount = 0;
                        if (headerTV.prefix[0] != '\0')
                        {
                            printf("%s/", headerTV.prefix);
                            printf("%.*s\n", 100, headerTV.name);
                        }
                        else if (headerTV.prefix[0] == '\0')
                        {
                            printf("%.*s\n", 100, headerTV.name);
                        }
                    }
                }
            }
        }
    }
}
