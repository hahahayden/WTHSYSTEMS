#include <stdio.h>
#include <stdlib.h>

int main()
{

    int c;

    int countChar = 0;

    FILE *fp;

    fp = fopen("test.txt", "r");
    while (countChar < 10)
    {

        c = fgetc(fp);
        printf("%c", c);
        fflush(fp);
        if (c == '\n')
        {
            /*
            printf("\n");
            */
            printf("h1");
            break;
        }
        printf("%c", c);
        fflush(fp);
        countChar += 1;
    }
    return 0;
}
