#include "swap.h"

void swap(int *point1, int *point2)
{

    int temp = *point1;
    *point1 = *point2;
    *point2 = temp;
}