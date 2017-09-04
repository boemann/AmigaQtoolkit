
#include <AQTextDoc.h>

#include <AQLineEdit.h>

#include <stdio.h>
#include <string.h>

AQLineEdit::AQLineEdit(AQWidget *parent)
   : AQTextEdit(parent, false)
{
   setExpanding(true, true);
   setMinimumSize(AQPoint(9, 11));
   setPreferredSize(AQPoint(70, 11));
}

AQLineEdit::~AQLineEdit()
{
}

AQString AQLineEdit::text() const
{
   return document()->toString();
}

bool AQLineEdit::keyEvent(const IntuiMessage &msg)
{
   if(msg.Class ==IDCMP_RAWKEY) {
      switch (msg.Code) {
      case Key_Left:
      case Key_Right:
      case Key_Home:
      case Key_End:
      case Key_Numeric1:
      case Key_Numeric7:
         return AQTextEdit::keyEvent(msg);
      default:
         return true;
      }
   } else { // IDMCP_VANILLAKEY
      if (msg.Code == '\r')
         return true;  //TODO emit some kind of signal
      else
         AQTextEdit::keyEvent(msg);
   }
   return true;
}

