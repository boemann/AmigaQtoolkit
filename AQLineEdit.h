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

protected:
   virtual bool keyEvent(const IntuiMessage &msg);
 
private:
};
   
#endif
