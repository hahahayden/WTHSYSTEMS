#include <string.h>
#include <stdio.h>
int main(int argc, char *argv[])
{

    int i = 0;
    while (argv[i])
    {
        /*
        int *pointer1 = i;
        int count;
        
        for (count = 0; count < strlen(argv[i]); count++)
        {
            */
        /*printf("%c", argv[i]);*/
        /*char *pointer = argv[pointer1][count];*/

        if (argv[i][0] == '-')
        /*if (strcmp(args[i][0], '-') == 0)*/
        {
            printf("%s", argv[i]);
            printf("\n");
        }
        i++;
    }
    return 0;
}
