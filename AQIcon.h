#ifndef AQICON_H
#define AQICON_H

#include <dos/dos.h>

#include <AQKernel.h>

class AQString;

class AQIconPrivate;

class AQIcon
{
public:
   enum Size {
      Small = 0,
      Large = 1,
   };

   AQIcon();
   AQIcon(const AQString &infoFileName);
   AQIcon(const AQIcon &other);
   ~AQIcon();
    
   bool isNull() const;
   AQPoint size() const;

   void paint(RastPort *rp, AQPoint pos, Size size, int state = 1);

   AQIcon &operator=(const AQIcon &other);

private:
   AQIconPrivate *m_d;
};
   
#endif
