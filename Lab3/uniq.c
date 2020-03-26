#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *read_long_line(FILE *fp)
{
    int c;
    int i = 0;
    int capacity = 5;
    int used = 0;
    int count = 0;
    int capacity2 = 5;
    char *charPtr = (char *)malloc(sizeof(char) * capacity);

    char *charPtr2 = (char *)malloc(sizeof(char) * capacity2);
    while (c = fgetc(fp), c != EOF)
    {
        if (c != '\n')
        {
            if (used == capacity)
            {
                capacity *= 2;
                charPtr = (char *)realloc(charPtr, sizeof(int) * capacity);
            }

            charPtr[used] = c;
            used += 1;
        }
        else
        {
            /*
            for (i = 0; i < used; i++)
            {
                printf("%d", count);
                printf("%c", charPtr[i]);
                printf("\n");
            }
            count += 1;
            for (i = 0; i < used; i++)
            {
                printf("%d", count);
                printf("%c", charPtr2[i]);
            }
*/
            count += 1;

            if (strcmp(charPtr, charPtr2) != 0)
            {

                free(charPtr2);
                charPtr2 = (char *)malloc(sizeof(char) * capacity);
                for (i = 0; i < capacity; i++)
                {
                    charPtr2[i] = charPtr[i];
                }
                /*charPtr2 = charPtr;*/
                for (i = 0; i < used; i++)
                {

                    printf("%c", charPtr[i]);
                }
                printf("\n");
                capacity = 5;
                used = 0;
                free(charPtr);

                charPtr = (char *)malloc(sizeof(char) * capacity);
            }

            else if (strcmp(charPtr, charPtr2) == 0)
            {

                free(charPtr2);
                charPtr2 = (char *)malloc(sizeof(char) * capacity);

                for (i = 0; i < capacity; i++)
                {
                    charPtr2[i] = charPtr[i];
                }
                /* charPtr2 = charPtr;*/

                capacity = 5;
                used = 0;
                free(charPtr);

                charPtr = (char *)malloc(sizeof(char) * capacity);
            }
        }
    }

    return charPtr;
}

int main()
{

    FILE *fp;

    fp = fopen("test.txt", "r");

    read_long_line(fp);

    return 0;
}
