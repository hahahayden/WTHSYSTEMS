#include <stdio.h>

#include "checkit.h"
#include "swap.h"

void test_swap_1()
{
    int x = 11;
    int y = 12;
    int *ptr = &x;
    int *ptr2 = &y;
    swap(ptr, ptr2);
    checkit_int(*ptr, 12);
    checkit_int(*ptr2, 11);
}

void test_swap()
{
    test_swap_1();
}

int main(void)
{
    test_swap();

    return 0;
}
