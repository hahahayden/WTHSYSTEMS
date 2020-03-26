#ifndef FW_H
#define FW_H

struct word
{
    char *string;
    int count;

    struct word *left;
    struct word *right;
};

struct listNode
{
    char *string;
    int count;
};
int cmpfunc(const void *a, const void *b);
int toArray(struct listNode words[], struct word *wordie, int count);
struct word *newWord(char *key);
struct word *insert(struct word *word, char *key);
int search(struct word *root, char *key);
int counting(struct word *node, int i);
void freeFunc(struct word *node);

#endif