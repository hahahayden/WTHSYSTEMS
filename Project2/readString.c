#include <stdio.h>
#include <stdlib.h>

int main()
{

    int c;

    int countChar = 0;
    /*
    int capacity = 5;
    int used = 0;
    
    char *charPtr = (char *)malloc(sizeof(char) * capacity);*/

    /*ptr_new = (int *)realloc(ptr, sizeof(int) * 3);*/
    FILE *fp;

    fp = fopen("test.txt", "r");
    while (countChar < 10)
    {

        c = fgetc(fp);

        if (c == '\n')
        {
            printf("h1");
            break;
        }
        countChar += 1;

        printf("%c", c);
        /*
        charPtr[i] = c;

        i++;
        used++;

        if (used == capacity)
        {
            capacity *= 2;
            charPtr = (char *)realloc(charPtr, sizeof(int) * capacity);
        }
        */
    }
    return 0;
}
