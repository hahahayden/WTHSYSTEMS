#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{

    int fd1[2];
    int fd2[2];
    int fd3[2];
    /*pipe(fd1);
    pipe(fd2);
    pipe(fd3);*/

    int myInt = 0;
    int buffer = 0;
    int i = 0;
    int array[10];

    int flag = 1;
    int count = 0;
    int x = 0;
    int status;
    char progs[2][101];

    int y = 0;

    i = 1;
    printf("arguments %d", argc);
    while (argv[i] != '\0')
    {
        for (x = 0; x < strlen(argv[i]); x++)
        {

            if (argv[i][x] == ';')
            {

                y++;
                count = 0;
                continue;
            }
            progs[y][count] = argv[i][x];
            count++;
        }

        i++;
        progs[y][strlen(progs[y])] = '\0';
    }

    progs[y][count] = '\0';

    printf("first program: %s \n", progs[0]);
    printf("second program: %s \n", progs[1]);
    pid_t childpid1, childpid2;
    int fd[2];
    int outfileStream;

    if (pipe(fd))
    {
        perror("Pipe failed.\n");
        exit(-1);
    }

    if ((childpid1 = fork()) == -1)
    {
        perror("Fork 1 failed.\n");
        exit(-1);
    }

    if (childpid1 == 0)
    {

        dup2(fd[1], 1);

        close(fd[0]);
        close(fd[1]);

        /*execlp("ls", "ls", 0);*/
        execlp(progs[0], progs[0], 0);

        perror("execl didn't work");
        exit(-1);
    }
    else
    {

        if ((childpid2 = fork()) == -1)
        {
            perror("Fork 2 failed.\n");
            exit(-1);
        }

        if (childpid2 == 0)
        {

            outfileStream = open("outfile.txt", O_WRONLY | O_CREAT | O_TRUNC,
                                 S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

            dup2(fd[0], 0);

            dup2(outfileStream, 1);

            close(fd[0]);
            close(fd[1]);

            /* execlp("sort", "sort", "-r", 0);*/
            execlp(progs[1], progs[1], "-r", 0);
            close(outfileStream);

            perror("sort didn't work");
            exit(-1);
        }
        else
        {
            int status;

            close(fd[0]);
            close(fd[1]);

            if (-1 == wait(&status))
            {
                perror("wait error");
                exit(-1);
            }
            if (-1 == wait(&status))
            {
                perror("wait error");
                exit(-1);
            }

            exit(0);
        }
    }
}