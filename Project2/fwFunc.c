#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fw.h"
/*
struct word
{
    char *string;
    int count;
    struct word *left, *right;
}

struct listNode
{
    char *string;
    int count;
}
*/
int cmpNodes(const void *a, const void *b) /* comparator for quick sort*/
{
    if (((struct listNode *)a)->count < ((struct listNode *)b)->count)
        return 1;
    else if (((struct listNode *)a)->count == ((struct listNode *)b)->count)
    {
        if (strcmp(((struct listNode *)a)->string, ((struct listNode *)b)->string) < 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if (((struct listNode *)a)->count > ((struct listNode *)b)->count)
    {
        return -1;
    }
    return 0;
}
/* convert the BST to a array*/

int toArray(struct listNode words[], struct word *wordie, int count)
{
    if (wordie->left != NULL)
    {
        count = toArray(words, wordie->left, count);
    }
    words[count].string = wordie->string;
    words[count].count = wordie->count;
    count++;
    if (wordie->right != NULL)
    {
        count = toArray(words, wordie->right, count);
    }
    return count;
    /*
    if (wordie == NULL)
        return;


    toArray(words, wordie->left, count);

 
    words[count].string = wordie->string;
    words[count].count = wordie->count;
    count += 1;
  
    toArray(words, wordie->right, count);
    */
}
/*create a new word structure*/
struct word *newWord(char *key)
{

    struct word *temp = (struct word *)malloc(sizeof(struct word));
    temp->string = malloc((strlen(key) + 1));

    strcpy(temp->string, key);
    /*temp->string = key;*/
    temp->count = 1;

    temp->left = temp->right = NULL;
    return temp;
}
struct word *insert(struct word *word, char *key)
{

    /* If the tree is empty, ret    urn a new node */
    if (word == NULL)
        return newWord(key);

    /* Otherwise, recur down the tree */
    if (strcmp(key, word->string) == 0)
    {
        word->count = word->count + 1;
    }

    else if (strcmp(key, word->string) < 0)
        word->left = insert(word->left, key);
    else if (strcmp(key, word->string) > 0)
        word->right = insert(word->right, key);

    return word;
    /* return the (unchanged) node pointer */
}
int search(struct word *root, char *key)
{
    /* Base Cases: root is null or key is present at root*/
    if (root == NULL)
    {
        return 0;
    }
    else if (strcmp(root->string, key) == 0)
    {
        /* return root;*/

        root->count += 1;
        return 1;
    }
    /* Key is greater than root's key*/
    if (strcmp(key, root->string) > 0)
        return search(root->right, key);

    /* Key is smaller than root's key*/
    return search(root->left, key);
}

int counting(struct word *wordie, int count)
{
    if (wordie->left != NULL)
    {
        count = counting(wordie->left, count);
    }

    count++;
    if (wordie->right != NULL)
    {
        count = counting(wordie->right, count);
    }
    return count;
}
void freeFunc(struct word *node)
{
    if (node != NULL)
    {
        freeFunc(node->left);
        freeFunc(node->right);
        free(node->string);
        free(node);
    }
}
