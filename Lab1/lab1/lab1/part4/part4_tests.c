#include <stdio.h>

#include "checkit.h"
#include "part4.h"

void test_lower_1()
{
    char str[] = "SUP";
    char newStr[] = "";
    checkit_string(lower(str, newStr), "sup");
}

void test_lower_2()
{
    char str[] = "HELLO WORLD";
    char newStr[] = "";
    checkit_string(lower(str, newStr), "hello world");
}
void test_lower_3()
{
    char str[] = "HELL";
    char newStr[] = "";
    checkit_string(lower(str, newStr), "hell");
}

void test_lower()
{
    test_lower_1();
    test_lower_2();
    test_lower_3();
}

void test_lowermutate_1()
{
    char str[] = "SUP";

    checkit_string(lowermutate(str), "sup");
}

void test_lowermutate_2()
{
    char str[] = "HELLO WORLD";

    checkit_string(lowermutate(str), "hello world");
}
void test_lowermutate_3()
{
    char str[] = "HELL";

    checkit_string(lowermutate(str), "hell");
}

void test_lowermutate()
{
    test_lowermutate_1();
    test_lowermutate_2();
    test_lowermutate_3();
}
int main(void)
{
    test_lower();
    test_lowermutate();

    return 0;
}
