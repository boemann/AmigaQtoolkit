#ifndef AQDIALOG_H
#define AQDIALOG_H

#include <AQWidget.h>
#include <AQString.h>

class AQListView;
class AQListItem;
class AQLineEdit;

class AQDialog : public AQWidget
{
public:
   enum Buttons {
      SaveButton =0x0001
    , OpenButton=0x0002
    , CancelButton=0x0004
    , SelectionName=0x0008
    , DrawerMode=0x0010
   };

   AQDialog(UWORD buttons, AQWidget *parent = nullptr);
   ~AQDialog();

   int exec();

   AQString drawer() const;
   void setDrawer(const AQString &drawerPath);
   AQString selectedPath() const;

   void accept();
   void dismiss();

   void onCdUp();
   void onItemExpanded(AQObject *obj);
   void onItemActivated(AQObject *obj);
   void onFileItemActivated(AQObject *obj);
   void onFileItemDoubleClicked(AQObject *obj);

protected:
   void closeEvent();

private:
   void populateDrawer(AQListItem *drawerItem, bool alsoFiles);
   void populateDrawerView();

   AQListView *m_drawerView;
   AQListView *m_filesView;
   AQLineEdit *m_selectName;
   AQLineEdit *m_locationName;
   bool m_drawerMode;
   bool m_drawerIsSet;
   bool m_visible;
   int m_return;
};
   
#endif
