#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hencode.h"
int fd1,fd2;
struct nodeLL* createNodeLL(unsigned ch, int freq)
{
   struct nodeLL* node = (struct nodeLL*)calloc(1,sizeof(struct nodeLL));
   /*printf("ch: %i",ch);*/
   node->ch = ch;
   node->freq = freq;
   node->next = node-> left = node->right = NULL;
   return node;
}

int isLeaf(struct nodeLL* root) 
{ 
   return !(root->left) && !(root->right); 
} 

struct nodeLL* pop(struct nodeLL** head)
{
   struct nodeLL* temp = calloc(1,sizeof(struct nodeLL));
   temp = *head;
   if (temp->next)
   {
      (*head) = temp->next;
      (*head)->size = temp->size;
   }
   (*head)->size--;
   return temp;
}

void insertSorted(struct nodeLL** head, struct nodeLL* node) 
{ 
   struct nodeLL* current; 
   if (*head == NULL || (*head)->freq >= node->freq) 
   { 
      node->next = *head;
      node->size = (*head)->size; 
      *head = node; 
   } 
   else
   {  
      current = *head; 
      while (current->next!=NULL && current->next->freq < node->freq) 
      { 
         current = current->next; 
      } 
      node->next = current->next; 
      current->next = node; 
   }
   (*head)->size++; 
   current = *head;
} 
void insert(struct nodeLL** head, struct nodeLL* node) 
{ 
   struct nodeLL* current; 
   if (*head == NULL || (*head)->freq > node->freq) 
   { 
      node->next = *head;
      node->size = (*head)->size; 
      *head = node; 
   } 
   else
   {  
      current = *head; 
      while (current->next!=NULL && current->next->freq <= node->freq) 
      { 
         current = current->next; 
      } 
      node->next = current->next; 
      current->next = node; 
   }
   (*head)->size++; 
} 

struct nodeLL* head;
struct nodeLL* createLinkedList(unsigned char* data, int* freq, int size)
{
   struct nodeLL *temp = calloc(1,sizeof(struct nodeLL));
   int i;
   temp->ch = data[0];
   temp->freq = freq[0];
   temp->next = temp-> left = temp->right = NULL;
   head = temp;
   head->size = 1;
   for (i = 1; i < size; i++)
   {
      insert(&head,createNodeLL(data[i],freq[i]));
   }
   return head;
}

unsigned char* huffCodes(struct nodeLL *root,int* codes, unsigned char* string, int size, int total)
{
   struct nodeLL *node = root;
   int i;
   int j = 0;
   for (i = 0;j < total; i++)
   {
      if (node->left == NULL && node->right == NULL)
      {
         /*printf("Char: %i",node->ch);*/
         string[j] = node->ch;
         node = root;
         j++;
      }
      if (codes[i] == 1)
      {
         node = node->right;
      }
      else if(codes[i] == 0)
      {
         node = node->left;
      }
   }
   return string;
}


struct nodeLL* buildHuffTree(unsigned char* data, int freq[],int size)
{
   struct nodeLL *left,*right,*top;
   head = createLinkedList(data,freq,size);
   while(head->size > 1)
   {
      left = pop(&head);
      if (head->size != 1)
      {
         right = pop(&head);
         top = createNodeLL('^',left->freq + right->freq);
         top->left = left;
         top->right = right;
         insertSorted(&head, top);
      }
      else
      {
         top = createNodeLL('^',left->freq + head->freq);
         top->left = left;
         top->right = head;
         head = top;
      }
   }
   return head;
}

int* dec_to_bin(int contents)
{
   int bin[8] = {128,64,32,16,8,4,2,1};
   int i;
   int size = 0;
   int* result = calloc(9,sizeof(int)); 
   for (i = 0; i < 8; i++)
   {
      if (contents/bin[i] > 0)
      {
         result[size] = 1;
         contents -= bin[i]; 
         size++;
      }
      else
      {
         result[size] = 0;
         size++;
      }
   }
   return result;
}

int bin_to_dec(char* binary)
{
   int bin[8] = {128,64,32,16,8,4,2,1};
   int i;
   int sum = 0;
   for (i = 0; i < strlen(binary);i++)
   {
      if (binary[i] == '1')
      {
         sum += bin[i % 8];
      }
   }
   return sum;
}

unsigned char* createCodes(unsigned char* data,int* freq, int size, int* codes, int chars)
{
   unsigned char* string = calloc(size+1,sizeof(unsigned char));
   struct nodeLL* root = buildHuffTree(data,freq,chars);
   string = huffCodes(root,codes,string,chars,size);
   string[size] = '\0';
   return string;
}

unsigned* readfile(int fd, int size)
{
   unsigned* buffer = calloc(size+1,sizeof(unsigned));
   int i = 0;
   unsigned c = 0;
   int bytes_read;
   do
   {
      bytes_read = read(fd,&c,1);
      buffer[i] = c;
      i++;
   }
   while (bytes_read != 0);
   buffer[size] = '\0';
   return buffer;
}

int main(int argc, char** argv)
{
   fd1 = open(argv[1],O_RDONLY);
   if (fd1 < 0)
   {
      printf("No such file or file is empty.\n");
      exit(1);
   }
   fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
   int fsize,i,j,unique;
   int size = 0;
   unsigned int* contents;
   int* codes;
   int* c = calloc(8,sizeof(int));
   unsigned char* data;
   int* freqs;
   int sum = 0;
   unsigned char* string;

   fsize = lseek(fd1,0,SEEK_END);
   lseek(fd1,0,SEEK_SET);
   unsigned chars;
   uint8_t ch;
   uint32_t f;
   read(fd1,&chars,sizeof(uint32_t));
   fsize -= sizeof(uint32_t);
   unique = chars;
   data = calloc(unique,sizeof(unsigned));
   freqs = calloc(unique,sizeof(int));
   for (i = 0;i < unique;i++)
   {
      read(fd1,&ch,sizeof(uint8_t));
      fsize -= sizeof(uint8_t);
      data[i] = ch;
      read(fd1,&f,sizeof(uint32_t));
      fsize-= sizeof(uint32_t);
      freqs[i] = f;
      sum+= freqs[i];
   }
   contents = readfile(fd1,fsize);
   codes = calloc(fsize*8,sizeof(int));
   if (fsize != 0)
   {
      for (i = 0; i < fsize; i++)
      {
         c = dec_to_bin(contents[i]);
         for (j = 0; j < 8; j++)
         {
            codes[size] = c[j];
            size++;
         } 
      }
      string = createCodes(data,freqs,sum,codes,unique);
   }
   if (fd2 < 0)
   {
      for (i = 0;i < sum;i++)
      {
         putchar(string[i]);
      }
   }
   else if (fsize == 0)
   {
      string = malloc(freqs[0]);
      for (i = 0; i < freqs[0];i++)
      {
         string[i] = data[0];
      }
      write(fd2,string,sum);
   }
   else
      write(fd2,string,sum);
   free(contents);
   free(codes);
   free(freqs);
   free(data);
   free(c);
   free(string);
   return 0;
}