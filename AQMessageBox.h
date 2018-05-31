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
      OkButton =0x0001
    , SaveButton=0x0002
    , CancelButton=0x0004
   };

   AQMessageBox(UWORD buttons, AQWidget *parent = nullptr);
   ~AQMessageBox();

   int exec();

   void accept();
   void dismiss();

protected:
   void closeEvent();

private:
   int m_return;
   bool m_visible;
};
   
#endif
