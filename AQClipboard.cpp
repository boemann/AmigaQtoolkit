
#include <AQString.h>

#include "AQClipboard.h"

class AQClipboardPrivate {
public:
   AQString text;
};


AQClipboard::AQClipboard()
   : m_d(new AQClipboardPrivate)
{
}

AQClipboard::~AQClipboard()
{
   delete m_d;
}

void AQClipboard::setText(const AQString &t)
{
   m_d->text = t;
}

AQString AQClipboard::text() const
{
   return m_d->text;
}

