#ifndef AQKERNEL_H
#define AQKERNEL_H

#include <sstream>

#define nullptr 0

struct AQDebug
{
   AQDebug() : m_space(true) {}
   ~AQDebug()
   {
      std::cout << m_s.str() << std::endl;
   }

   inline AQDebug &space() { m_space = true; m_s << ' '; return *this; }
   inline AQDebug &nospace() { m_space = false; return *this; }
   inline AQDebug &maybeSpace() { if (m_space) m_s << ' '; return *this; }

   inline AQDebug &operator<<(bool t) { m_s << (t ? "true" : "false"); return maybeSpace(); }
   inline AQDebug &operator<<(char t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(signed short t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(unsigned short t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(signed int t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(unsigned int t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(signed long t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(unsigned long t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(float t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(double t) { m_s << t; return maybeSpace(); }
   inline AQDebug &operator<<(const char *t) { m_s << t; return maybeSpace(); }
//   inline AQDebug &operator<<(const QString &t) { m_s << '\"' << t  << '\"'; return maybeSpace(); }
   inline AQDebug &operator<<(const void * t) { m_s << t; return maybeSpace(); }

private:
   std::ostringstream m_s;
   bool m_space;
};

struct AQNoDebug
{
   AQNoDebug() {}
   ~AQNoDebug() {}

   inline AQNoDebug &space() { return *this; }
   inline AQNoDebug &nospace() { return *this; }
   inline AQNoDebug &maybeSpace() { return *this; }

    template<typename T>
    inline AQNoDebug &operator<<(const T &) { return *this; }
};

#define aqDebug() AQDebug()

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
