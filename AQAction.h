#ifndef AQACTION_H
#define AQACTION_H

#include <AQObject.h>
#include <AQString.h>

struct IntuiMessage;


class AQAction : public AQObject
{
public:
   AQAction(AQObject *parent = nullptr);
   ~AQAction();

   void setText(const AQString &text);
   AQString text() const;
   void setShortcut(const AQString &sc);
   bool matchShortcut(const IntuiMessage &msg);
   void trigger();

private:
   AQString m_shortcut;
   AQString m_text;
   bool m_scCtrl;
   bool m_scAmiga;
   bool m_scAlt;
   bool m_scShift;
   char m_scCode;
   ULONG m_scClass;
};
   
#endif