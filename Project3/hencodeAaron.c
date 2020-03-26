#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hencode.h"

int fd1;
int fd2;

struct nodeLL *createNodeLL(unsigned ch, int freq)
{
   struct nodeLL *node = (struct nodeLL *)calloc(1, sizeof(struct nodeLL));
   node->ch = ch;
   node->freq = freq;
   node->next = node->left = node->right = NULL;
   return node;
}

struct huffNode createHuffNode(unsigned ch, int *code, int freq, int size)
{
   struct huffNode node;
   node.ch = ch;
   node.freq = freq;
   node.code = calloc(size, sizeof(int));
   memcpy(node.code, code, sizeof(int) * size);
   node.size = size;
   return node;
}

int isLeaf(struct nodeLL *root)
{
   return !(root->left) && !(root->right);
}

struct nodeLL *pop(struct nodeLL **head)
{
   struct nodeLL *temp = calloc(1, sizeof(struct nodeLL));
   temp = *head;
   if (temp->next)
   {
      (*head) = temp->next;
      (*head)->size = temp->size;
   }
   (*head)->size--;
   return temp;
}

void insertSorted(struct nodeLL **head, struct nodeLL *node)
{
   struct nodeLL *current;
   if (*head == NULL || (*head)->freq >= node->freq)
   {
      node->next = *head;
      node->size = (*head)->size;
      *head = node;
   }
   else
   {
      current = *head;
      while (current->next != NULL && current->next->freq < node->freq)
      {
         current = current->next;
      }
      node->next = current->next;
      current->next = node;
   }
   (*head)->size++;
   current = *head;
}
void insert(struct nodeLL **head, struct nodeLL *node)
{
   struct nodeLL *current;
   if (*head == NULL || (*head)->freq > node->freq)
   {
      node->next = *head;
      node->size = (*head)->size;
      *head = node;
   }
   else
   {
      current = *head;
      while (current->next != NULL && current->next->freq <= node->freq)
      {
         current = current->next;
      }
      node->next = current->next;
      current->next = node;
   }
   (*head)->size++;
}

struct nodeLL *head;
struct nodeLL *createLinkedList(unsigned *data, int *freq, int size)
{
   struct nodeLL *temp = calloc(1, sizeof(struct nodeLL));
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
   return head;
}

struct huffNode *printCodes(struct nodeLL *root, int arr[], int top, struct huffNode nodes[])
{

   if ((root)->left)
   {
      arr[top] = 0;
      printCodes((root)->left, arr, top + 1, nodes);
   }
   if ((root)->right)
   {
      arr[top] = 1;
      printCodes((root)->right, arr, top + 1, nodes);
   }
   if (isLeaf((root)))
   {
      nodes[(unsigned)(root)->ch] = createHuffNode((root)->ch, arr, (root)->freq, top);
   }
   return nodes;
}

struct nodeLL *buildHuffTree(unsigned chars[], int freq[], int size)
{
   struct nodeLL *left, *right, *top;
   head = createLinkedList(chars, freq, size);
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
   return head;
}

int *cntFreq(unsigned contents[], int size)
{
   int *freq = (int *)calloc(256, sizeof(int));
   unsigned c;
   int i;
   for (i = 0; i < size; i++)
   {
      c = contents[i];
      freq[c]++;
   }
   return freq;
}

unsigned *readfile(int fd, int size)
{
   unsigned *buffer = calloc(size + 1, sizeof(unsigned));
   unsigned c = 0;
   int i = 0;
   int bytes_read;
   do
   {
      bytes_read = read(fd, &c, 1);
      buffer[i] = (unsigned)c;
      i++;
   } while (bytes_read != 0);
   buffer[size] = '\0';
   return buffer;
}

struct huffNode *hencode(unsigned *data, int *freq, int size)
{
   int i = 0;
   int *arr = calloc(size, sizeof(int));
   struct huffNode *nodes = calloc(256 + 1, sizeof(struct huffNode));
   struct nodeLL *root = buildHuffTree(data, freq, size);
   printCodes(root, arr, i, nodes);
   return nodes;
}

void encode(struct huffNode *codes, unsigned *contents, unsigned fsize)
{
   int sum = 0;
   int i, j;
   unsigned ch;
   int cap = 10;
   int size = 0;
   int binary[8] = {128, 64, 32, 16, 8, 4, 2, 1};
   int extraBits;
   unsigned *string = calloc(cap, sizeof(unsigned));
   for (i = 0; i < fsize; i++)
   {
      ch = (unsigned)contents[i];
      for (j = 0; j < codes[ch].size; j++)
      {
         if (size == cap)
         {
            cap *= 2;
            string = realloc(string, (cap + 1) * sizeof(unsigned));
         }
         string[size] = codes[ch].code[j];
         size++;
      }
   }
   extraBits = size % 8;
   for (i = 0, j = 1; i < size; i++, j++)
   {
      if (string[i] == 1)
      {
         sum += binary[i % 8];
      }
      if (j % 8 == 0)
      {
         write(fd2, &sum, sizeof(uint8_t));
         sum = 0;
      }
   }
   if (extraBits > 0)
   {
      write(fd2, &sum, sizeof(uint8_t));
   }
}

void writeHeader(unsigned size, unsigned *chars, int *freq)
{
   int i;
   write(fd2, &size, sizeof(uint32_t));
   for (i = 0; i < size; i++)
   {
      print("%d", freq[i]);
      write(fd2, &chars[i], sizeof(uint8_t));
      write(fd2, &freq[i], sizeof(uint32_t));
   }
}

int main(int argc, char **argv)
{
   fd1 = open(argv[1], O_RDONLY);
   if (fd1 < 0)
   {
      printf("No such file or file is empty.\n");
      exit(1);
   }
   fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
   int fsize;
   unsigned i, chars;
   unsigned *contents;
   int *freqs;
   int newFreqs[256];
   unsigned data[256];

   fsize = lseek(fd1, 0, SEEK_END);
   lseek(fd1, 0, SEEK_SET);
   contents = readfile(fd1, fsize);
   freqs = cntFreq(contents, fsize);
   chars = 0;
   for (i = 0; i < 256; i++)
   {
      if (freqs[i] != 0)
      {
         newFreqs[chars] = freqs[i];
         data[chars] = i;
         chars++;
      }
   }
   writeHeader(chars, data, newFreqs);
   if (chars > 1)
      encode(hencode(data, newFreqs, chars), contents, fsize);
   free(contents);
   free(freqs);
   return 0;
}
