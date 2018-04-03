#ifndef AQTOOLTIP_H
#define AQTOOLTIP_H

#include <AQWidget.h>

#include <AQString.h>

class AQAction;
class AQApplication;

class AQToolTip : public AQWidget
{
public:
   AQToolTip(const AQString &text = AQString(), AQWidget*parent = nullptr);
   ~AQToolTip();

   void exec(const AQPoint &pos);
   void popdown();

   friend class AQApplication;

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);

private:
   void updateSize();

   bool m_execed;
   AQString m_text;

   static void filterIntuiMessage(IntuiMessage &msg);
   static void checkShow(AQWidget *hoveredWidget);

   static AQToolTip *s_toolTip;
   static AQPoint s_stillMousePos;
   static int s_timeCounter;
   static bool s_avaitReactivation;

};
   
#endif
