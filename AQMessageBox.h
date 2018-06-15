#ifndef AQMESSAGEBOX_H
#define AQMESSAGEBOX_H

#include <AQWidget.h>
#include <AQString.h>

class AQListView;
class AQListItem;
class AQLineEdit;

class AQMessageBox : public AQWidget
{
public:
   enum Buttons {
      Ok =0x0001
    , Save=0x0002
    , DontSave=0x0002
    , Cancel=0x0004
   };

   AQMessageBox(const AQString &title, const AQString &text, UWORD buttons, AQWidget *parent = nullptr);
   ~AQMessageBox();

   int exec();

   void accept();
   void dismiss();

protected:
   void closeEvent();

private:
   void destroy();

   int m_return;
   bool m_visible;
};
   
#endif
