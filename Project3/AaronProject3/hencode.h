#ifndef HENCODE_H
#define HENCODE_H

struct node
{
    unsigned ch;
    int freq;
    struct node *left, *right;
};

struct nodeLL
{
    unsigned ch;
    int freq;
    int size;
    struct nodeLL *next;
    struct nodeLL *left, *right;
};

struct huffNode
{
    unsigned ch;
    int *code;
    int freq;
    int size;
};

#endif