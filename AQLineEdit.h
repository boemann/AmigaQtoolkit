#ifndef AQLINEEDIT_H
#define AQLINEEDIT_H

#include <AQString.h>
#include <AQTextEdit.h>

class AQLineEdit : public AQTextEdit
{
public:
   AQLineEdit(AQWidget *parent = nullptr);
   ~AQLineEdit();

   AQString text() const;
   void setText(const AQString &text);

protected:
   virtual bool keyEvent(const IntuiMessage &msg);
 
private:
};
   
#endif
