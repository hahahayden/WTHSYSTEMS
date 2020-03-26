#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "hencode.h"
#include <stdint.h>

int fd;
int fd2;
struct node *createNode(int c, unsigned freq)
{
   struct node *n = (struct node *)malloc(sizeof(struct node));
   n->ch = (int)c;
   n->freq = freq;
   n->left = n->right = NULL;
   return n;
}
struct codes
{
   unsigned char character;
   int frequency;
   int *code;
   int size;
};
struct nodeLL *createNodeLL(unsigned char ch, int freq)
{
   struct nodeLL *node = (struct nodeLL *)calloc(1, sizeof(struct nodeLL));
   node->ch = (unsigned char)ch;
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
struct nodeLL *createLinkedList(unsigned char *data, int *freq, int size)
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
   }
   (*head)->size--;

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

      current = current->next;
   }

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
/*
void printArr(int *array, int n)
{
   int i;
   for (i = 0; i < n; ++i)
   {
      printf("%i", array[i]);
   }
   printf("\n");
}
*/
struct codes *printCodes(struct codes *codes, struct nodeLL *root, unsigned int arr[], int top)
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

      codes[root->ch].character = root->ch;

      codes[root->ch].frequency = root->freq;
      codes[root->ch].code = malloc(sizeof(int) * top);
      memcpy(codes[root->ch].code, arr, top * sizeof(int));
      /*codes[(int)root->ch].code = arr;*/
      codes[(unsigned int)root->ch].size = top;

      return codes;
   }
}
void *printCodesDecode(struct nodeLL *root, unsigned char arr[], unsigned char *final, int size, int total)
{
   int i;
   int j = 0;

   struct nodeLL *curr = root;
   for (i = 0; i < size; i++)
   {
      if (j == (total))
      {
         break;
      }
      if (arr[i] == 0)
         curr = curr->left;
      else
         curr = curr->right;

      if (curr->left == NULL && curr->right == NULL)
      {

         final[j] = curr->ch;
         curr = root;
         j++;
      }
   }

   return arr;
}
/*struct nodeLL *printCodesDecode(struct nodeLL *top, struct nodeLL *root, char arr[], int size, int bufferSize, int count, char *final, int maxCount)*/

/*struct nodeLL *printCodesDecode(struct nodeLL *top, struct nodeLL *root, char arr[], int size)
{
   int j;
   int i;

   printf("%d", arr[3]);

   if (arr[size] == 1)
   {

      printCodesDecode(top, root->right, arr, size + 1);
   }

   if (arr[size] == 0)
   {

      printCodesDecode(top, root->left, arr, size + 1);
   }

   if (isLeaf(root))
   {

      printf("%c", root->ch);
      return top;
      /*
      if (count != maxCount)
      {
         final[count] = root->ch;
         count += 1;
      }
      else
      {
         return top;
      }
      
   }
}
*/

struct nodeLL *buildHuffTree(unsigned char chars[], int freq[], int size)
{
   struct nodeLL *left, *right, *top;
   unsigned int arr[size];

   int i;
   int *s;
   int x = size;
   /*printHeader(size);*/
   /*
   printCount(chars, freq, size);*/
   head = createLinkedList(chars, freq, size);

   /* s = &size;*/

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

   i = 0;

   struct codes *codeHash = malloc(sizeof(struct codes) * 256);

   printCodes(codeHash, head, arr, i);

   /* printArr(codeHash[98].code, codeHash[98].size);*/
   /* for (i = 0; i < codeHash[99].size; i++)
   {
      printf("%d", codeHash[99].code[i]);
   }
   printf("size %d", size);
   */

   /*printCodes(codeHash, size);*/

   return head;
}

/*void print(struct node* root)
{
   if(root->left != NULL)
   {
      print(root->left);
   }
   printf("%c",root->ch)
}*/

struct nodeLL *hencode(unsigned char data[], int freq[], int size)
{
   struct nodeLL *head = buildHuffTree(data, freq, size);

   return head;
}
/*
int int_to_bin(int a, char *buffer, int buff_size)
{
   int i;
   int count2 = 0;
   int count = (buff_size - 1);
   for (i = buff_size - 1; i >= 0; i--)
   {
      buffer[i] = (a & 1) + '0';
      a = a >> 1;

      count--;
      count2++;
   }
   for (i = 0; i <= count2; i++)
   {
      printf("%d", buffer[i]);
   }

   return count2;
}
*/
/*
int decToBinary(int n, char *array, int count)
{

   int binaryNum[8];

   int i = 0;
   while (n > 0)
   {

      binaryNum[i] = n % 2;
      n = n / 2;
      i++;
   }
   int j;

   for (j = 8 - 1; j >= 0; j--)
   {
      
      array[count] = binaryNum[j];
      count += 1;
   }
   return count;
}
*/

int decToBinary(int n, unsigned char *array, unsigned int count)
{

   int binaryNum[8];

   int i = 0;
   while (n > 0)
   {

      binaryNum[i] = n % 2;
      n = n / 2;
      i++;
   }
   int j = 0;
   int k = 8 - i;
   int h = 0;
   if (i < 8)
   {
      for (h = 0; h < k; h++)
      {

         array[count] = 0;
         count += 1;
      }
   }
   for (j = i - 1; j >= 0; j--)
   {
      array[count] = binaryNum[j];
      count += 1;
   }
   return count;
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
int main(int argc, char **argv)
{
   int size;
   unsigned char *contents;
   unsigned char *encodes;
   int j = 0;
   int stdFlag;
   /*
   if (argc == 1)
   {
      stdFlag = 1;
      fd = open(0, O_RDONLY);
      fd = 0;
      fd2 = open(0, O_WRONLY | O_CREAT | O_TRUNC, 0666);
      fd2 = 1;
   }
   else */
   if (argc == 3)
   {
      fd = open(argv[1], O_RDONLY);
      fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
   }

   else
   {
      fprintf(stderr, "Arguments");
   }
   size = lseek(fd, 0, SEEK_END);
   lseek(fd, 0, SEEK_SET);

   int *freqs;
   unsigned char *buffer = malloc(sizeof(unsigned char) * (size + 1));
   unsigned char *buffer2 = malloc(sizeof(unsigned char) * (size + 1));
   unsigned char *buffer3 = malloc(sizeof(unsigned char) * (size + 1));
   unsigned int buffer4;

   unsigned char buffer5[441000];
   read(fd, &buffer, sizeof(uint32_t));

   unsigned uniques = buffer;
   freqs = malloc(sizeof(int) * uniques);

   contents = malloc(sizeof(char) * uniques);
   for (j = 0; j < uniques; j++)
   {
      read(fd, &(buffer2), sizeof(uint8_t));
      contents[j] = buffer2;
      read(fd, &buffer3, sizeof(uint32_t));
      freqs[j] = buffer3;
   }

   int stringCapacity = 10;
   /* freqs[uniques ] = '\0';*/
   int freqSum = 0;
   for (j = 0; j < uniques; j++)
   {
      freqSum += freqs[j];
   }

   struct nodeLL *top;
   top = hencode(contents, freqs, uniques);
   /*char *stringBig = malloc(stringCapacit  y * sizeof(char));*/

   int used = 0;
   int i = 0;
   int buf_len = 0;
   int check;
   int capacity = 30;

   unsigned int *code = calloc(capacity, sizeof(unsigned int));
   int h = 0;

   while ((check = read(fd, &(buffer4), 1) > 0))
   {

      if (used == capacity)
      {
         capacity *= 5;
         code = (unsigned int *)realloc(code, sizeof(unsigned int) * capacity);
      }

      code[used] = buffer4;
      used += 1;
   }

   int count = 0;
   int sum = 0;
   int count2 = 10;

   for (j = 0; j < (used); j++)
   {
      /*printf("%d\n", code[j]);*/
      /*decToBinary(code[j]);*/
      count = decToBinary(code[j], buffer5, count);
      sum += count;
   }

   buffer5[count + 1] = '\0';
   int codeCount = 0;
   int uniqueCount = 0;
   int sizeCount = uniques;

   unsigned char *final = (unsigned char *)calloc(freqSum, sizeof(unsigned char));

   /* printCodesDecode(top, top, buffer5, codeCount, count, uniqueCount, final, uniques);*/
   if (uniques == 1)
   {

      for (j = 0; j < freqSum; j++)
      {
         final[j] = contents[0];
         write(fd2, &final[j], 1);
      }

      return 0;
   }
   printCodesDecode(top, buffer5, final, count, freqSum);

   /*write(fd2, "HI", 1);*/

   for (i = 0; i < freqSum; i++)
   {
      write(fd2, &final[i], 1);
   }

   /*
   int_to_bin(encodes, buffer5, i);*/
   /*
   for (j = 0; j < used * 8; j++)
   {
      printf("%d", buffer5[j]);
   }
   */

   return 0;
}
