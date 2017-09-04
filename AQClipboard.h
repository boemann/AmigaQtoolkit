#ifndef AQCLIPBOARD_H
#define AQCLIPBOARD_H

#include <AQKernel.h>

class AQString;
class AQApplication;
class AQClipboardPrivate;

class AQClipboard
{
public:
   void setText(const AQString &t);
   AQString text() const;

   friend class AQApplication;

private:
   AQClipboard();
   ~AQClipboard();

   AQClipboardPrivate *m_d;
};
   
#endif
