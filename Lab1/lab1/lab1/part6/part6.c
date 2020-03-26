#include "part6.h"

struct Point create_point(double x, double y)
{
   struct Point p;
   /*struct Point p= {x,y};*/
   p.x = x;
   p.y = y;
   return p;
}

int is_a_square(struct Rectangle r)
{
   int xDist;
   int yDist;
   xDist = r.topLeft.x - r.bottomRight.x;
   yDist = r.bottomRight.y - r.topLeft.y;

   if (xDist == yDist)
   {
      return 1;
   }
   return 0;
}
