#include <stdio.h>

#include "checkit.h"
#include "part6.h"

/* define testing functions */
void test_create_point1(void)
{
   struct Point p = create_point(1.9, -2.7);

   checkit_double(p.x, 1.9);
   checkit_double(p.y, -2.7);
}

void test_create_point2(void)
{
   struct Point p = create_point(0.2, 12.1);

   checkit_double(p.x, 0.2);
   checkit_double(p.y, 12.1);
}

void test_create_point(void)
{
   test_create_point1();
   test_create_point2();
}

void test_is_a_square(void)
{
   struct Point p = create_point(0, 1);
   struct Point p1 = create_point(1, 0);
   struct Rectangle r;
   r.topLeft = p;
   r.bottomRight = p1;
   checkit_int(is_a_square(r), 1);
}
void test_is_a_square2(void)
{
   struct Point p = create_point(0, 1);
   struct Point p1 = create_point(2, 0);
   struct Rectangle r;
   r.topLeft = p;
   r.bottomRight = p1;
   checkit_int(is_a_square(r), 0);
}

void test_isSquare(void)
{
   test_is_a_square();
   test_is_a_square2();
}

int main(int arg, char *argv[])
{
   /* call testing function(s) here */
   test_create_point();
   test_isSquare();

   return 0;
}
