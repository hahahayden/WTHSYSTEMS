#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "hencode.h"
#include <stdint.h>
struct node *createNode(int c, unsigned freq)
{
    struct node *n = (struct node *)malloc(sizeof(struct node));
    n->ch = (int)c;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}
struct nodeLL *createNodeLL(char ch, int freq)
{
    struct nodeLL *node = (struct nodeLL *)malloc(sizeof(struct nodeLL));
    node->ch = ch;
    node->freq = freq;
    node->next = node->left = node->right = NULL;
    return node;
}
struct huffNode *createHuffNode(char ch, int *code)
{
    struct huffNode *node = (struct huffNode *)malloc(sizeof(struct huffNode));
    node->ch = ch;
    node->code = code;
    return node;
}
int isLeaf(struct nodeLL *root)
{
    return !(root->left) && !(root->right);
}

struct node *createArray(char *data, int *freq, int size)
{
    struct node *array = (struct node *)malloc(sizeof(struct node) * (size));
    int i;
    for (i = 0; i < (size); i++)
    {
        array[i].ch = (int)data[i];
        array[i].freq = freq[i];
    }
    return array;
}
struct nodeLL *head;
struct nodeLL *createLinkedList(char *data, int *freq, int size)
{
    struct nodeLL *temp = malloc(sizeof(struct nodeLL));
    int i;
    temp->ch = data[0];
    temp->freq = freq[0];
    temp->next = temp->left = temp->right = NULL;
    head = temp;
    head->size = 1;
    for (i = 1; i < size; i++)
    {
        insert(&head, createNodeLL(data[i], freq[i]));
    }
    /*for (i = 1; i < size; i++)
   {
      struct nodeLL *temp2 = malloc(sizeof(struct nodeLL));
      temp->next = temp2;
      temp2->ch = data[i];
      temp2->freq = freq[i];
      temp2->next = temp2->left = temp2->right = NULL;
      temp = temp2;
      head->size++;
   }
   */
    return head;
}
struct nodeLL *pop(struct nodeLL **head)
{
    struct nodeLL *temp = malloc(sizeof(struct nodeLL));
    temp = *head;
    if (temp->next)
    {
        (*head) = temp->next;
        (*head)->size = temp->size;
        printf("%c", (*head)->ch);
    }
    (*head)->size--;

    printf("Popped: %c\n", temp->ch);
    printf("%d", temp->freq);
    return temp;
}
void insertSorted(struct nodeLL **head, struct nodeLL *node)
{
    struct nodeLL *current;
    /* Special case for the head end */
    if (*head == NULL || (*head)->freq >= node->freq)
    {
        node->next = *head;
        node->size = (*head)->size;
        *head = node;
    }
    else
    {
        /* Locate the node before the point of insertion */
        current = *head;
        while (current->next != NULL && current->next->freq < node->freq)
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
    /*if (node->next != NULL)
      printf("Inserting %c,%i before %c,%i\n",node->ch,node->freq,node->next->ch,node->next->freq); 
   else
      printf("Inserting %c,%i at the end\n",node->ch,node->freq);*/
    (*head)->size++;
    current = *head;
    while (current->next)
    {
        printf("%c,%i ", current->ch, current->freq);
        current = current->next;
    }
    printf("%c,%i ", current->ch, current->freq);
    printf("\n");
    /*printf("Inserted: %c,%i\n",node->ch,node->freq);  
   printf("Head: %c,%i\n",(*head)->ch,(*head)->freq);  */
}
void insert(struct nodeLL **head, struct nodeLL *node)
{
    struct nodeLL *current;
    /* Special case for the head end */
    if (*head == NULL || (*head)->freq > node->freq)
    {
        node->next = *head;
        node->size = (*head)->size;
        *head = node;
    }
    else
    {
        /* Locate the node before the point of insertion */
        current = *head;
        while (current->next != NULL && current->next->freq <= node->freq)
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
    /*if (node->next != NULL)
      printf("Inserting %c,%i before %c,%i\n",node->ch,node->freq,node->next->ch,node->next->freq); 
   else
      printf("Inserting %c,%i at the end\n",node->ch,node->freq);*/
    (*head)->size++;
    while (current->next)
    {
        printf("%c,%i ", current->ch, current->freq);
        current = current->next;
    }
    printf("%c,%i ", current->ch, current->freq);
    printf("\n");
}

void insert(struct nodeLL **head, struct nodeLL *node)
{
    struct nodeLL *current;
    /* Special case for the head end */
    if (*head == NULL || (*head)->freq > node->freq)
    {
        node->next = *head;
        node->size = (*head)->size;
        *head = node;
    }
    else
    {
        /* Locate the node before the point of insertion */
        current = *head;
        while (current->next != NULL && current->next->freq <= node->freq)
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
    /*if (node->next != NULL)
      printf("Inserting %c,%i before %c,%i\n",node->ch,node->freq,node->next->ch,node->next->freq); 
   else
      printf("Inserting %c,%i at the end\n",node->ch,node->freq);*/
    (*head)->size++;
    while (current->next)
    {
        printf("%c,%i ", current->ch, current->freq);
        current = current->next;
    }
    printf("%c,%i ", current->ch, current->freq);
    printf("\n");
}

void printArr(int *array, int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%i", array[i]);
    }
    printf("\n");
}

struct codes *printCodes(struct codes *codes, struct nodeLL *root, int arr[], int top)
{
    int j;
    int i;

    if (root->left)
    {

        arr[top] = 0;
        printCodes(codes, root->left, arr, top + 1);
    }

    if (root->right)
    {

        arr[top] = 1;
        printCodes(codes, root->right, arr, top + 1);
    }

    if (isLeaf(root))
    {
        printf("%c(0x%x) :", root->ch, root->ch);

        printArr(arr, top);
        printf("%d", root->ch);
        codes[(int)root->ch].character = root->ch;

        codes[(int)root->ch].frequency = root->freq;
        codes[(int)root->ch].code = malloc(sizeof(int) * top);
        memcpy(codes[(int)root->ch].code, arr, top * sizeof(int));
        /*codes[(int)root->ch].code = arr;*/
        codes[(int)root->ch].size = top;
        return codes;
    }
}
void printHeader(int sizeOG)
{
    printf("size %d", sizeOG);
    int *bufferInt = malloc(sizeof(int));
    bufferInt = sizeOG;

    write(fd2, &bufferInt, sizeof(uint32_t));
}
void printCount(char *chars, int *freq, int size)
{
    int i = 0;
    /* int *bufferCount= malloc(sizeof(int)*size);
    bufferCount=chars;
*/
    for (i = 0; i < size; i++)
    {

        write(fd2, &chars[i], sizeof(uint8_t));
        write(fd2, &freq[i], sizeof(uint32_t));
    }
}

struct codes *buildHuffTree(char chars[], int freq[], int size)
{
    struct nodeLL *left, *right, *top;
    int arr[size];

    int i;
    int *s;
    int x = size;
    printHeader(size);
    printCount(chars, freq, size);
    head = createLinkedList(chars, freq, size);
    printf("size %d", size);
    s = &size;
    while (head->size > 1)
    {
        left = pop(&head);
        /*printf("Left: %i\n",left->freq);*/
        if (head->size != 1)
        {
            right = pop(&head);
            /*printf("Right: %i\n",right->freq);*/
            top = createNodeLL('^', left->freq + right->freq);
            top->left = left;
            top->right = right;
            insertSorted(&head, top);
        }
        else
        {
            top = createNodeLL('^', left->freq + head->freq);
            top->left = left;
            top->right = head;
            head = top;
        }
    }

    printf("%i", head->size);

    i = 0;

    struct codes *codeHash = malloc(sizeof(struct codes) * 128);

    printCodes(codeHash, head, arr, i);
    printf("Here%c", codeHash[98].character);
    printf("SUp");
    /* printArr(codeHash[98].code, codeHash[98].size);*/
    for (i = 0; i < codeHash[99].size; i++)
    {
        printf("%d", codeHash[99].code[i]);
    }
    printf("size %d", size);

    /*printCodes(codeHash, size);*/

    return codeHash;
}

char *readfile(int fd, int size)
{
    char *buffer = malloc(sizeof(char) * (size + 1));
    char c = 0;
    int i = 0;
    int bytes_read;
    do
    {
        bytes_read = read(fd, &c, 1);
        buffer[i] = c;
        i++;
    } while (bytes_read != 0);
    buffer[size] = '\0';

    return buffer;
}
void printEncodes(int fd, struct codes *codeHash)
{

    int sum = 0;
    int i = 0;
    unsigned size;
    char *contents;
    char *stringBig;

    int stringCapacity = 10;
    int used = 0;
    int j = 0;
    int bitMath[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    contents = readfile(fd, size);
    stringBig = malloc(stringCapacity * sizeof(char));
    for (i = 0; i < size; i++)
    {
        printf("%c", contents[i]);

        printf("Contents%d,%c\n", codeHash[(int)contents[i]].size, codeHash[contents[i]].character);

        for (j = 0; j < codeHash[(int)contents[i]].size; j++)
        {
            if (used == stringCapacity)
            {
                stringCapacity *= 2;

                stringBig = (char *)realloc(stringBig, sizeof(char) * stringCapacity);
            }
            /*printf("%d", codeHash[(int)contents[i]].code[j]);*/
            stringBig[used] = codeHash[(int)contents[i]].code[j];

            used += 1;
        }
    }
    printf("DOne");

    for (j = 0; j < used; j++)
    {
        printf("%d", stringBig[j]);
    }
    int totalSplits = used / 8;
    int padNeeded = used % 8;
    int runTime = 0;
    int countBit;
    countBit = 0;
    int *buffer;
    char *buffer2;
    printf("Size%d \n", used);
    for (j = 0; j < used; j++)
    {

        if (stringBig[j] == 1)
        {
            printf("%d   ", bitMath[countBit]);
            sum += bitMath[countBit];
            countBit += 1;
        }
        else if (stringBig[j] == 0)
        {
            countBit += 1;
        }
        if (countBit == 8)
        {
            printf("%d Hered", sum);
            buffer = sum;
            write(fd2, &buffer, sizeof(uint8_t));
            sum = 0;
            countBit = 0;
            buffer = 0;
            runTime += 1;
        }
    }

    if (padNeeded > 0)
    {
        buffer = sum;

        write(fd2, &buffer, sizeof(uint8_t));
    }
}

int *cntFreq(char contents[])
{
    int *freq = (int *)malloc(sizeof(int) * 128);
    int c;
    int i;
    for (i = 0; i < strlen(contents); i++)
    {
        c = contents[i];
        freq[c]++;
    }
    return freq;
}

struct codes *hencode(char *data, int *freq, int size)
{
    struct codes *code = buildHuffTree(data, freq, size);
    return code;
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