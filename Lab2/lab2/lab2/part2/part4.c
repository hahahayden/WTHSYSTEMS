#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "part4.h"
#include <string.h>

char *lower(char x[], char y[])
{
    int size;
    int i;
    /*char *ptr = x;
    char *ptr2 = y;
    */

    size = strlen(x);

    for (i = 0; i <= size; i++)
    {
        int *p = &i;
        y[*p] = tolower(x[*p]);
    }
    return y;  //why
}

char *lowermutate(char x[])
{
    int i = 0;
    while (x[i])
    {
        int *p = &i;
        x[*p] = tolower(x[*p]);
        i++;
    }
    return x;
}

/*
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main()
{
    char arr[] = "abc def";
    char y[] = "";
    char *ptr = arr;
    char *ptr2 = y;

    int size = strlen(arr);
    printf("%d", size);
    printf("%s", ptr[6]);
}
*/