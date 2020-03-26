#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "hencode.h"
#include <stdint.h>
int fd2;
int fd;
/*
struct node *createNode(unsigned char c, unsigned freq)
{
   struct node *n = (struct node *)malloc(sizeof(struct node));
   n->ch = c;

   n->freq = freq;
   n->left = n->right = NULL;
   return n;
}
*/
struct codes
{
    unsigned char character;
    int frequency;
    int *code;
    int size;
};
struct nodeLL *createNodeLL(unsigned char ch, int freq)
{
    struct nodeLL *node = (struct nodeLL *)malloc(sizeof(struct nodeLL));

    node->ch = (unsigned char)ch;
    /*printf("Char: %i\n", node->ch);*/
    node->freq = freq;
    node->next = node->left = node->right = NULL;
    return node;
}

/*
struct node* pop(struct node* array,int* size)
{
   int i;
   struct node* min = createNode((int)array[0].ch,array[0].freq);
   /*for (i = 0; i < (*size);i++)
   {
      printf("array: %c,%i\t",array[i].ch,array[i].freq);
   }
   printf("%i\n",min->ch);
   for (i = 0; i < (*size)-1; i++)
   {
      array[i] = array[i+1];
   }
   (*size)--;

   printf("popped: %i\n",min->ch);
   return min;
}*/

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
        /*
      printf("%c,%i ", current->ch, current->freq);*/
        current = current->next;
    }
    /*
   printf("%c,%i ", current->ch, current->freq);
   printf("\n");
   */
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

    /*
   printf("%c,%i ", current->ch, current->freq);
   printf("\n");
   */
}
int isLeaf(struct nodeLL *root)
{
    return !(root->left) && !(root->right);
}

/*
void insertNode(struct node *array, struct node *top, int *size)
{
   int i, j;
   int position = *size;
   struct node temp;
   for (i = 0; i < (*size); i++)
   {
      if (array[i].freq == top->freq)
      {
         position = i;
         break;
      }
   }
   for (i = (*size) + 1; i < position; i--)
      ;
   {
   }
   /*for (i = 0; i < (*size);i++)
   {
      printf("array: %c,%i\t",array[i].ch,array[i].freq);
   }
   printf("\n");
}
*/

struct nodeLL *head;
struct nodeLL *createLinkedList(unsigned char *data, int freq[], int size)
{
    struct nodeLL *temp = malloc(sizeof(struct nodeLL));
    int i;
    temp->ch = (unsigned char)data[0];
    temp->freq = freq[0];
    temp->next = temp->left = temp->right = NULL;

    head = temp;
    /* memcpy(head, temp, sizeof(struct nodeLL));*/
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
    }
    (*head)->size--;

    /* printf("Popped: %c\n", temp->ch);
   printf("%d", temp->freq);*/
    return temp;
}
/*
void insert(struct nodeLL **head_ref, struct nodeLL *new_node)
{
   struct nodeLL *current;
   
   if (*head_ref == NULL || (*head_ref)->freq >= new_node->freq)
   {
      new_node->next = *head_ref;
      new_node->size = (*head_ref)->size;
      *head_ref = new_node;
   }
   else
   {
    
      current = *head_ref;
      while (current->next != NULL && current->next->freq < new_node->freq)
      {
         current = current->next;
      }
      new_node->next = current->next;
      current->next = new_node;
   }
   (*head_ref)->size++;
}
*/

void printArr(int *array, int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%i", array[i]);
    }
    printf("\n");
}

struct codes *printCodes(struct codes *codes, struct nodeLL *root, unsigned int arr[], int top)
{

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
void printHeader(int sizeOG, int stdFlag)
{

    /*int *bufferInt= malloc(sizeof(int)); DONT KNOWWW*/
    /*bufferInt = sizeOG;*/
    if (stdFlag == 0)
    {
        write(fd2, &sizeOG, sizeof(uint32_t));
    }
    else if (stdFlag == 1)
    {
        write(1, &sizeOG, sizeof(uint32_t));
    }
}
void printCount(unsigned char chars[], int freq[], unsigned int size, int stdFlag)
{
    int i = 0;
    /* int *bufferCount= malloc(sizeof(int)*size);
    bufferCount=chars;
*/
    for (i = 0; i < size; i++)
    {

        write(fd2, &(chars[i]), sizeof(uint8_t));
        write(fd2, &(freq[i]), sizeof(uint32_t));
    }
}

struct codes *buildHuffTree(unsigned char chars[], int freq[], unsigned int size, int stdFlag)
{
    struct nodeLL *left, *right, *top;
    unsigned int arr[size];

    int i;
    int *s;

    printHeader(size, stdFlag);
    printCount(chars, freq, size, stdFlag);
    head = createLinkedList(chars, freq, size);

    s = &size;
    while (head->size > 1)
    {
        left = pop(&head);
        if (head->size != 1)
        {
            right = pop(&head);
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

    /*printf("%i", head->size);*/

    i = 0;

    struct codes *codeHash = malloc(sizeof(struct codes) * 256);

    printCodes(codeHash, head, arr, i);
    /*printf("Here%c", codeHash[98].character);
   printf("SUp");
   */
    /* printArr(codeHash[98].code, codeHash[98].size);*/
    /*
   for (i = 0; i < codeHash[99].size; i++)
   {
      printf("%d", codeHash[99].code[i]);
   }
   
   printf("size %d", size);*/
    /* freeFunc(top);*/
    /*printCodes(codeHash, size);*/
    freeFunc(top);

    return codeHash;
}

void freeFunc(struct nodeLL *node)
{
    if (node != NULL)
    {
        free(node->left);
        free(node->right);

        freeFunc(node->next);

        free(node);
    }
}
void freeF(struct nodeLL *node)
{
    if (node != NULL)
    {

        freeFunc(node->left);

        free(node);
    }
}

unsigned char *readfile(int fd, int size)
{
    unsigned char *buffer = malloc(sizeof(unsigned char) * (size + 1));
    unsigned char c = 0;
    int i = 0;
    int bytes_read;

    do
    {

        bytes_read = read(fd, &c, 1);

        buffer[i] = (unsigned char)c;

        i++;
    } while (bytes_read != 0);
    buffer[size] = '\0';

    return buffer;
}
void printEncodes(int fd, struct codes *codeHash, int stdFlag)
{

    unsigned int sum = 0;
    int i = 0;
    unsigned size;
    unsigned char *contents;
    unsigned char *stringBig;

    unsigned int stringCapacity = 10;
    int used = 0;
    int j = 0;
    unsigned int bitMath[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    contents = readfile(fd, size);
    /*
   for (j = 0; j < size; j++)
   {
      printf("%c", contents[j]);
   }*/
    stringBig = malloc(stringCapacity * sizeof(unsigned char));
    for (i = 0; i < size; i++)
    {

        for (j = 0; j < codeHash[contents[i]].size; j++)
        {
            if (used == stringCapacity)
            {
                stringCapacity *= 2;

                stringBig = (unsigned char *)realloc(stringBig, sizeof(unsigned char) * stringCapacity);
            }
            /*printf("%d", codeHash[(int)contents[i]].code[j]);*/
            stringBig[used] = codeHash[(unsigned char)contents[i]].code[j];

            used += 1;
        }
    }
    /* stringBig[used + 1] = '\0';*/

    int totalSplits = used / 8;
    unsigned int padNeeded = used % 8;
    int runTime = 0;
    int countBit;
    countBit = 0;
    unsigned buffer;

    for (j = 0; j < used; j++)
    {

        if (stringBig[j] == 1)
        {

            sum += bitMath[countBit];
            countBit += 1;
        }
        else if (stringBig[j] == 0)
        {
            countBit += 1;
        }
        if (countBit == 8)
        {

            buffer = sum;
            if (stdFlag == 0)
            {
                write(fd2, &buffer, sizeof(uint8_t));
            }
            else if (stdFlag == 1)
            {
                write(1, &buffer, sizeof(uint8_t));
            }

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
    free(stringBig);
}

int *cntFreq(unsigned char contents[], int size)
{
    int *freq = (int *)malloc(sizeof(int) * 256);
    int i;
    unsigned int c;
    for (i = 0; i < size; i++)
    {
        c = contents[i];
        freq[c]++;
    }
    return freq;
}

struct codes *hencode(unsigned char *data, int *freq, int size, int stdFlag)
{
    struct codes *code = buildHuffTree(data, freq, size, stdFlag);
    return code;
}

int main(int argc, char **argv)
{
    int stdFlag;
    if (argc > 3)
    {
        write(2, "Too much arguments", sizeof(char));
    }
    else if (argc == 2)
    {
        stdFlag = 1;
    }

    else if (argc == 3)
    {
        stdFlag = 0;
    }
    fd = open(argv[1], O_RDONLY);
    fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd2 < 0)
    {
        fprintf(stderr, "File 2 Not Found");
    }
    /*write(2, "Hello", 34);*/

    unsigned int size;
    unsigned i;
    unsigned char *contents;
    int *freqs;
    int newFreqs[256];
    unsigned char data[256];
    char *empty = "\0";
    size = lseek(fd, 0, SEEK_END);

    lseek(fd, 0, SEEK_SET);
    if (size == -1)
    {

        fprintf(stderr, " %s No such File in Directory", argv[1]);
        return 0;
    }

    contents = readfile(fd, size);

    freqs = cntFreq(contents, size);
    for (i = 0; i < 256; i++)
    {
        printf("%d", freqs[i]);
        printf("\n");
    }
    size = 0;

    for (i = 0; i < 256; i++)
    {
        if (freqs[i] != 0)
        {
            newFreqs[size] = freqs[i];
            data[size] = i;
            size++;
        }
    }

    struct codes *codes = hencode(data, newFreqs, size, stdFlag);

    close(fd);
    /*
    int fd3 = open(argv[1], O_RDONLY);
    printEncodes(fd3, codes, stdFlag);

    close(fd2);
    for (i = 0; i < size; i++)
    {
        free(codes[i].code);
    }
    free(codes);

    free(freqs);
    free(contents);
    */
    return 0;
}
