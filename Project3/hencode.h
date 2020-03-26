#ifndef HENCODE_H
#define HENCODE_H

struct node
{
    unsigned ch;
    unsigned freq;
    struct node *left, *right;
};

struct nodeLL
{
    unsigned char ch;
    unsigned int freq;
    int size;
    struct nodeLL *next;
    struct nodeLL *left, *right;
};

#endif