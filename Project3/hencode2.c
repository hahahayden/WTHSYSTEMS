
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

int fd;
int fd2;
struct node
{
    char character;
    int count;
};
struct MinHeapNode
{

    char data;

    unsigned freq;

    struct MinHeapNode *left, *right;
};
struct codes
{
    char *character;
    int *code;
};
struct MinHeap
{

    unsigned size;

    unsigned capacity;

    struct MinHeapNode **array;
};
struct MinHeapNode *newNode(char data, unsigned freq)
{
    struct MinHeapNode *temp = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

struct MinHeap *createMinHeap(unsigned capacity)

{

    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct MinHeapNode **)malloc(minHeap->capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}
void swapMinHeapNode(struct MinHeapNode **a,
                     struct MinHeapNode **b)

{

    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}
void minHeapify(struct MinHeap *minHeap, int idx)

{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    /*if (minHeap->array[left]->freq == minHeap->array[smallest]->freq)
    {
        if (minHeap->array[left]->char < minHeap->array[smallest]->char)
        {
            smallest = left;
        }
        else
        {
            smallest = right;
        }
    }
    */

    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
int isSizeOne(struct MinHeap *minHeap)
{

    return (minHeap->size == 1);
}
struct MinHeapNode *extractMin(struct MinHeap *minHeap)

{

    struct MinHeapNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}
void insertMinHeap(struct MinHeap *minHeap,
                   struct MinHeapNode *minHeapNode)

{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}
void buildMinHeap(struct MinHeap *minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
int isLeaf(struct MinHeapNode *root)

{

    return !(root->left) && !(root->right);
}
struct MinHeap *createAndBuildMinHeap(char data[], int freq[], int size)

{
    int i = 0;
    struct MinHeap *minHeap = createMinHeap(size);

    for (i = 0; i < size; ++i)
    {
        minHeap->array[i] = newNode(data[i], freq[i]);
    }
    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}
struct MinHeapNode *buildHuffmanTree(char data[], int freq[], int size)

{
    char *s1;
    char *s2;
    struct MinHeapNode *left, *right, *top;

    struct MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap))
    {

        left = extractMin(minHeap);
        right = extractMin(minHeap);

        s1 = &(left->data);
        s2 = &(right->data);

        if (left->freq == right->freq)
        {
            if (strcmp(s1, s2) < 0)
            {
                top = newNode(left->data, left->freq + right->freq);
            }
            else
            {
                top = newNode(right->data, left->freq + right->freq);
            }
        }
        else if (strcmp(s1, s2) < 0)
        {
            top = newNode(left->data, left->freq + right->freq);
        }

        else
        {
            top = newNode(right->data, left->freq + right->freq);
        }

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}
int cmpNodes(const void *a, const void *b) /* comparator for quick sort*/
{
    if (((struct node *)a)->count > ((struct node *)b)->count)
        return 1;
    else if (((struct node *)a)->count == ((struct node *)b)->count)
    {
        if ((((struct node *)a)->character, ((struct node *)b)->character) < 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if (((struct node *)a)->count < ((struct node *)b)->count)
    {
        return -1;
    }
    return 0;
}

void makeNodeList(char *arrayPtr, struct node *nodeArray)
{
    int sizePtr = 5; /* dynamic*/
    int used = 0;
    int i = 0;
    nodeArray = malloc(sizeof(struct node) * sizePtr);
    int counting = 0;

    for (counting = 0; counting < 128; counting++)
    {
        if (arrayPtr[counting] != '\0')
        {

            if (used == sizePtr)
            {
                sizePtr *= 2;

                nodeArray = realloc(nodeArray, sizeof(struct node) * sizePtr);
            }

            struct node *new = malloc(sizeof(struct node));
            new->character = counting;
            new->count = arrayPtr[counting];
            printf("%d,%c", new->count, new->character);
            nodeArray[used] = *new;
            used += 1;
        }
    }

    for (counting = 0; counting < used; counting++)
    {
        printf("\nCount%d, Character %c", nodeArray[counting].count, nodeArray[counting].character);
    }
    qsort(nodeArray, used, sizeof(struct node), cmpNodes);
    printf("Sort");
    for (counting = 0; counting < used; counting++)
    {
        printf("\nCount%d, Character %c", nodeArray[counting].count, nodeArray[counting].character);
    }
    char characterArray[used];
    int frequencyArray[used];
    for (counting = 0; counting < used; counting++)
    {

        characterArray[counting] = nodeArray[counting].character;

        frequencyArray[counting] = nodeArray[counting].count;
    }

    struct MinHeapNode *root = buildHuffmanTree(characterArray, frequencyArray, used);
    printf("here");
    printf("%c", root->data);
    printf("%d", root->freq);
    struct codes *codes = malloc(sizeof(codes) * 256);
    HuffmanCodes(root, codes);
    printf("%c", codes[97].character);
    for (i = 0; i < used; ++i)
    {

        printf("%d", codes[root->data].code[i]);
    }

    /*printHeader(characterArray, frequencyArray, used);*/
}

void printHeader(char *characterArray, int *frequencyArray, int used)
{

    int i = 0;
    write(fd2, "fdedfs", 36);
    for (i = 0; i < used; i++)
    {
        write(fd2, characterArray[i], 1);
        write(fd2, frequencyArray[i], 4);
    }
}

void HuffmanCodes(struct MinHeapNode *root, struct codes *codes)

{
    int arr[100]; /* MAKE DYANMIC*/
    int top = 0;

    codes = printCodes(codes, root, arr, top);
}
void printCodes(struct codes *codes, struct MinHeapNode *root, int arr[], int top)

{
    int i = 0;
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

        codes[root->data].character = root->data;

        codes[root->data].code = malloc(sizeof(int *) * top);

        printf("%c: ", root->data);
        for (i = 0; i < top; ++i)
        {
            codes[root->data].code[i] = arr[i];
        }

        printArr(arr, top);
    }
}
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)

        printf("%d", arr[i]);

    printf("\n");
}

void read_file(int fd, char *buffer, char *arrayPtr, int size, struct node *nodeArray)
{
    /* rn fix size*/
    int count = 0;
    int bytes_read;
    int i = 0;
    int var;

    do
    {

        bytes_read = read(fd, buffer, size);

    } while (bytes_read != 0);

    /*read the file*/
    /* go through it and add into the array*/
    for (i = 0; i < size; i++)
    {
        printf("%c", buffer[i]);
    }
    for (i = 0; i < size; i++)
    {
        var = buffer[i];
        printf("%c", buffer[i]);

        if (arrayPtr[var] == '\0')
        {
            arrayPtr[var] = 1;
        }
        else
        {
            arrayPtr[(int)buffer[i]] += 1;
        }
    }
    for (i = 0; i < 255; i++)
    {
        printf("%d", arrayPtr[i]);
    }

    makeNodeList(arrayPtr, nodeArray);
    /*printBits(5, buffer);*/
}
void printBits(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = 0; i <= size; i++)
    {
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

int main(int argc, char *argv[])
{
    int i = 0;
    int size = 10;
    char *buffer;
    /*unsigned char buffer[size];*/

    char array[256] = "";
    char *arrayPtr;
    struct node *nodeArray;

    arrayPtr = array;

    fd = open(argv[1], O_RDONLY);
    fd2 = open(argv[2], O_WRONLY);
    if (fd2 == NULL)
    {
        printf("NO");
    }
    else
    {
        printf("YESSS");
    }
    write(fd2, "fdedfs", 36);
    struct stat st;
    stat("test.txt", &st);
    size = st.st_size;
    printf("%dSIZEEE", size);
    buffer = (char *)malloc(sizeof(char) * size);
    read_file(fd, buffer, arrayPtr, size, nodeArray);

    return 0;
}