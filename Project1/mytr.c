#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* to put char into new file*/
void rebuild(int i, char x[])
{

    putchar(x[i]);
}
/* parse finds the unique characters and puts them into one*/
char *parse(char *arrayChar, char argument2[], int count, int count2)
{
    int count1 = 0;
    for (count1 = 0; count1 < strlen(argument2); count1++)
    {

        int ascValue = argument2[count1];
        if (ascValue == 92)
        {

            if (argument2[count1 + 1] == 'n')
            {

                arrayChar[count2] = '\n';
                count1 += 1;
                count2 += 1;
            }
            else if (argument2[count1 + 1] == 't')
            {
                arrayChar[count2] = '\t';
                count1 += 1;
                count2 += 1;
            }
            else if (argument2[count1 + 1] == 92)
            {
                arrayChar[count2] = '\\';
                count1 += 1;
                count2 += 1;
            }
            else if (strlen(argument2) == count1 + 1)
            {
                arrayChar[count2] = 92;
                count2 += 1;
            }
            else
            {
                arrayChar[count2] = argument2[count1 + 1];
                count1 += 1;
                count2 += 1;
            }
        }
        else
        {
            arrayChar[count2] = argument2[count1];
            count2 += 1;
        }
    }
    return arrayChar;
}
int main(int argc, char *argv[])
{
    char inputs[132] = {""};
    int count1 = 0;
    int i;
    int count2 = 0;

    int count5 = 0;
    int count6 = 0;
    int count0 = 0;
    int count11 = 0;

    int count10 = 0;
    char arrayChar[100];
    char arrayChar2[100];

    char *arrayCharPt = arrayChar;
    char *arrayCharPt2 = arrayChar2;
    int ascValue;
    /* if this argument is -d and has size of four usage error*/
    if (argv[1][0] == '-' && argv[1][1] == 'd' && argc == 4)
    {
        fprintf(stderr, "Usage Error: mytr -d 'set1'");
        exit(0);
    }

    /* if this argument has size of two it means its neither the delete or transalte*/
    else if (argc == 2)
    {

        fprintf(stderr, "Usage Error: mytr 'set1' 'set2'");
        exit(0);
    }
    /* if this argument has size of two it means its either the delete or transalte*/
    else if ((argv[1][0] == '-' && argv[1][1] == 'd' && argc == 3 && strlen(argv[1]) == 2) || (argc == 3 && argv[1][0] != '-'))
    {

        if (argv[1][0] == '-' && argv[1][1] == 'd' && argc == 3 && strlen(argv[1]) == 2) /* delete part*/
        {
            char arrayCharDelete[100];

            parse(arrayCharDelete, argv[2], count1, count10);

            for (count2 = 0; count2 < strlen(arrayCharDelete); count2++)
            {
                int ascValue = arrayCharDelete[count2];
                inputs[ascValue] = 'd';
            }
        }

        else if (argc == 3) /* if its argument count is 3 then translate and its not -d*/
        {
            count5 = 0;

            parse(arrayCharPt, argv[1], count5, count0);
            parse(arrayCharPt2, argv[2], count6, count11);

            for (count1 = 0; count1 < strlen(arrayChar); count1++)
            {

                ascValue = arrayChar[count1];
                /* ascValue2 = arrayChar2[count1];*/

                if (count1 >= (strlen(arrayChar2) - 1)) /* if set 1 greater than set 2 then use last character*/
                {
                    /*ascValue2 = arrayChar2[strlen(arrayChar2) - 1];*/
                    inputs[ascValue] = arrayChar2[strlen(arrayChar2) - 1];
                }
                else
                {

                    inputs[ascValue] = arrayChar2[count1];
                }
            }
        }
        else
        {
            fprintf(stderr, "Usage Error: mytr 'set1' 'set2'");
            exit(0);
        }

        while ((i = getchar(), i != EOF)) /* put into the file*/
        {
            if (argv[1][0] == '-' && argv[1][1] == 'd') /* if -d then check if the table is -d as well if so don't add it into table*/
            {
                if (inputs[i] != 'd')
                {
                    putchar((int)i);
                }
            }
            else if (inputs[i] == '\0')
            {
                putchar((int)i);
            }

            else
            {
                rebuild(i, inputs);
            }
        }
    }
    else
    {
        fprintf(stderr, "Usage Error: mytr 'set1' 'set2'");
        exit(0);
    }
    return 0;
}
