#include <stdio.h>

#include "checkit.h"
#include "part3.h"

void test_sum_1()
{
    int array[4] = {1, 2, 3, 4};
    checkit_int(sum(array, 4), 10);
}

void test_sum_2()
{
    int array[4] = {1, 2, 3, 4};
    checkit_int(sum(array, 0), 0);
}
void test_sum()
{
    test_sum_1();
    test_sum_2();
}

int main(void)
{
    test_sum();

    return 0;
}
