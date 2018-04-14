#ifndef AQCLIPBOARD_H
#define AQCLIPBOARD_H

#include <AQKernel.h>
#include <AQObject.h>

class AQString;
class AQApplication;
class AQClipboardPrivate;

class AQClipboard : public AQObject
{
public:
   void setText(const AQString &t);
   AQString text() const;

   friend class AQApplication;

private:
   AQClipboard();
   ~AQClipboard();

   void checkUpdates();

   AQClipboardPrivate *m_d;
};
   
#endif
