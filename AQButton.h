#ifndef AQBUTTON_H
#define AQBUTTON_H

#include <AQWidget.h>
#include <AQIcon.h>

class AQButton : public AQWidget
{
public:
   AQButton(bool toolMode, AQWidget *parent = nullptr);
   ~AQButton();

   void setCheckable(bool c);
   void setText(const AQString &text);
   void setIcon(const AQIcon &icon);

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual void enterEvent();
   virtual void leaveEvent();
   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);

private:
   void recalcSizes();
   AQString m_text;
   AQIcon m_icon;
   bool m_pressed;
   bool m_toolMode;
   bool m_hovered;
   bool m_checkable;
};
   
#endif
