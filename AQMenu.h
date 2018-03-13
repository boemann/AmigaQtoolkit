#ifndef AQMENU_H
#define AQMENU_H

#include <AQWidget.h>

#include <AQString.h>

class AQAction;
struct NewMenu;

class AQMenu : public AQWidget
{
public:
   AQMenu(const AQString &title = AQString());
   ~AQMenu();

   void setMenubarMode(bool on);

   void addSeparator();
   void addAction(AQAction *action);
   void addMenu(AQMenu *menu);

   void exec(const AQPoint &pos);
   void popup(const AQPoint &pos);

   NewMenu *buildGadMenu(NewMenu *gaditem);

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual void closeEvent();
   virtual void changeEvent(int change);

   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);

private:
   void updateSize();

   vector<int> m_entryType;
   vector<AQMenu *> m_entryMenu;
   vector<AQAction *> m_entryAction;
   int m_hoveredItem;
   bool m_execed;
   bool m_firstUpReceived;
   bool m_barMode;
   AQString m_title;
   AQMenu *m_shownSubMenu;
};
   
#endif
