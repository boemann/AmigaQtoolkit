#ifndef AQMENU_H
#define AQMENU_H

#include <AQWidget.h>

#include <AQString.h>

class AQMenu : public AQWidget
{
public:
   AQMenu();
   ~AQMenu();

   void addItem(const AQString &text);
   void addSeparator();

   void exec(const AQPoint &pos);

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual void closeEvent();
   virtual void changeEvent(int change);

   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);

private:
   void addEntry(const AQString &text, int type, const AQString &shortcut, AQMenu *subMenu);

   vector<AQString> m_entryText;
   vector<int> m_entryType;
   vector<AQString> m_entryShortCut;
   vector<AQMenu *> m_entrySubMenu;
   int m_hoveredItem;
   bool m_visible;
   bool m_firstUpReceived;
};
   
#endif
