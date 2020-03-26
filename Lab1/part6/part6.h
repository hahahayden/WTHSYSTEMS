#ifndef PART5_H
#define PART5_H

struct Point
{
   double x;
   double y;
};

struct Point create_point(double x, double y);

struct Rectangle
{
   struct Point topLeft;
   struct Point bottomRight;
};

int is_a_square(struct Rectangle r);
#endif
