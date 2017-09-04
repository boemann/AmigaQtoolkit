#ifndef AQKERNEL_H
#define AQKERNEL_H

#ifndef __GNUC__
typedef char bool;
#define true 1
#define false 0
#endif

#define nullptr 0
   
class AQPoint
{
public:
   AQPoint() : x(0), y(0) {}
   AQPoint(long int ix, long int iy) : x(ix), y(iy) {}
   AQPoint(const AQPoint &o) : x(o.x), y(o.y) {}

   AQPoint & operator+=(const AQPoint &o) { x += o.x; y += o.y; return *this;}
   AQPoint & operator-=(const AQPoint &o) { x -= o.x; y -= o.y; return *this;}
   AQPoint operator-() const;

   long int x;
   long int y;
};

const AQPoint operator+(const AQPoint &p1, const AQPoint &p2);
const AQPoint operator-(const AQPoint &p1, const AQPoint &p2);
bool operator==(const AQPoint &p1, const AQPoint &p2);
bool operator!=(const AQPoint &p1, const AQPoint &p2);

class AQRect
{
public:
   AQRect() : topLeft(), bottomRight() {}
   AQRect(long int ix, long int iy, long int iw, long int ih)
      : topLeft(ix, iy)
      , bottomRight(ix + iw - 1, iy + ih - 1)
   {}
   AQRect(const AQRect &o)
      : topLeft(o.topLeft)
      , bottomRight(o.bottomRight)
   {}
   AQRect(const AQPoint &otl, const AQPoint &obr)
      : topLeft(otl)
      , bottomRight(obr)
   {}
   
   bool isValid() const {return width() > 0 && height() > 0; }
   long int width() const {return bottomRight.x - topLeft.x + 1;}
   long int height() const {return bottomRight.y - topLeft.y + 1;}

   bool contains(const AQPoint &p) const
   {
     if (p.x < topLeft.x || p.x > bottomRight.x)
        return false;
     if (p.y < topLeft.y || p.y > bottomRight.y)
        return false;
     return true;
   }

   bool contains(const AQRect &o) const
   {
     return contains(o.topLeft) && contains(o.bottomRight);
   }

   AQRect intersected(const AQRect &o) const;

   void translate(const AQPoint &off)
   {
      topLeft += off;
      bottomRight += off;
   }

   AQPoint topLeft;
   AQPoint bottomRight;
};

inline int aqMin(int a, int b) {return a<b ? a : b;}
inline int aqMax(int a, int b) {return a>b ? a : b;}

#endif
