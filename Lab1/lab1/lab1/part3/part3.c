#include <stdio.h>
#include <math.h>
#include "part3.h"
int sum(int array1[], int size)
{
    int sum = 0;
    int a;
    for (a = 0; a < size; a = a + 1)
    {
        sum += array1[a];
    }
    return sum;
}