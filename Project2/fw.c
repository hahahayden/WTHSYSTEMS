
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fwFunc.c"
#include "fw.h"

int main(int argc, char *argv[])
{
    int searchResult;
    int counter;
    struct word *root;
    struct word tmp;
    int firstWord = 0;
    int arguments = 0;
    int capacity = 5;
    int used = 0;
    int count = 0;
    int i = 0;
    int wordCounter = 0;
    int character;
    char *charPtr;
    int mode = 0;
    FILE *fp;

    struct listNode *words;

    if (argc != 1 && argc != 2 && argv[1][0] == '-' && argv[1][1] == 'n')
    {

        if (argc != 1)
        {
            if (atoi(argv[2]) == 0)
            {

                fprintf(stderr, "usage: fw [-n num] [ file1 [ file 2 ...] ]");
                return 0;
            }
        }
    }

    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'n')
    {
        fprintf(stderr, "Not enough arguments");
        return 0;
    }
    else if (argc == 1 || (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'n')) /*stdin*/
    {

        charPtr = (char *)malloc(sizeof(char) * capacity);
        while ((character = getchar(), character != EOF))
        {
            if (isalpha(character) != 0)
            {

                if (used == capacity)
                {
                    capacity *= 2;

                    charPtr = (char *)realloc(charPtr, sizeof(char) * capacity);
                }

                charPtr[used] = tolower(character);

                used += 1;
            }
            else if (isalpha(character) == 0 && strlen(charPtr) > 0)
            {
                if (used == capacity)
                {
                    capacity *= 2;

                    charPtr = (char *)realloc(charPtr, sizeof(char) * capacity);
                }
                charPtr[used] = '\0';
                if (firstWord == 0)
                {

                    root = NULL;
                    root = insert(root, charPtr);

                    firstWord += 1;
                }
                else
                {

                    insert(root, charPtr);
                }

                free(charPtr);

                capacity = 5;
                used = 0;
                charPtr = (char *)malloc(sizeof(char) * capacity);
            }
        }
    }
    else /*not standard in*/
    {

        if (argv[1][0] == '-' && argv[1][1] == 'n')
        {

            arguments = 3;
            mode = 1;
        }
        else
        {
            arguments = 1;
            mode = 0;
        }
        for (arguments = arguments; arguments < argc; arguments++)
        {
            fp = fopen(argv[arguments], "r");

            if (fp == NULL)
            {
                fprintf(stderr, "%s: No such file or directory \n", argv[arguments]);
                if (mode == 0)
                {
                    printf("The top 10 words (out of 0) are: \n");
                }
                else if (mode == 1)
                {
                    printf("The top %d words (out of 0) are: \n", atoi(argv[2]));
                }
                return 0;
            }

            charPtr = (char *)malloc(sizeof(char) * capacity);

            while ((character = fgetc(fp), character != EOF))
            {

                if (isalpha(character) != 0)
                {

                    if (used == capacity)
                    {
                        capacity *= 2;

                        charPtr = (char *)realloc(charPtr, sizeof(char) * capacity);
                    }

                    charPtr[used] = tolower(character);

                    used += 1;
                }
                else if (isalpha(character) == 0 && strlen(charPtr) > 0)
                {

                    if (used == capacity)
                    {
                        capacity *= 2;

                        charPtr = (char *)realloc(charPtr, sizeof(char) * capacity);
                    }
                    charPtr[used] = '\0';

                    if (firstWord == 0)
                    {

                        root = NULL;
                        root = insert(root, charPtr);

                        firstWord += 1;
                    }
                    else
                    {

                        insert(root, charPtr);
                    }

                    free(charPtr);

                    capacity = 5;
                    used = 0;
                    charPtr = (char *)malloc(sizeof(char) * capacity);
                }
            }
        }
    }
    if (firstWord == 0 && argv[1][0] == '-' && argv[1][1] == 'n') /* if empty file and has -n as argument display message*/
    {

        counter = atoi(argv[2]);

        free(charPtr);

        printf("The top %d words (out of %d) are: \n", counter, wordCounter);
        return 0;
    }
    else if (firstWord == 0) /* else if it is still empty but no -n*/
    {

        free(charPtr);

        printf("The top 10 words (out of 0) are: \n");
        return 0;
    }
    wordCounter = counting(root, wordCounter);
    words = malloc(wordCounter * sizeof(struct listNode));
    toArray(words, root, count);

    qsort(words, wordCounter, sizeof(struct listNode), cmpNodes);

    if (argc != 1 && argv[1][0] == '-' && argv[1][1] == 'n')
    {

        if (atoi(argv[2]) > wordCounter) /* if word counter is less than the argument given then write that many numbers*/
        {
            counter = wordCounter;
        }

        else
        {
            counter = atoi(argv[2]);
        }

        printf("The top %d words (out of %d) are: \n", atoi(argv[2]), wordCounter);
        for (i = 0; i < counter; i++)
        {
            printf("%9d %s \n", words[i].count, words[i].string);
        }
    }

    else
    {

        if (wordCounter < 10)
        {
            counter = wordCounter;
        }
        /* if the -n isn't given then the count depends on how many words or 10 by default; */
        else
        {
            counter = 10;
        }
        printf("The top 10 words (out of %d) are: \n", wordCounter);
        for (i = 0; i < counter; i++)
        {
            printf("%9d %s \n", words[i].count, words[i].string);
        }
    }
    /* free everything*/
    free(charPtr);
    freeFunc(root);
    free(words);

    return 0;
}
