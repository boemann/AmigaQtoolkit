#ifndef AQSCROLLBAR_H
#define AQSCROLLBAR_H

#include <AQWidget.h>
#include <AQIcon.h>

class AQScrollBar : public AQWidget
{
public:
   AQScrollBar(bool horiz, AQWidget *parent = nullptr);
   ~AQScrollBar();

   int value() const;
   void setValue(int v);
   void setMaximum(int m);
   void setPageStep(int step);

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);
   virtual void resizeEvent(const AQPoint &oldSize);
 
private:
   void calculateKnob();

   bool m_horiz;
   int m_value;
   int m_pageStep;
   int m_singleStep;
   int m_minimum;
   int m_maximum;
   int m_pressOffset;
   LONG m_botH;
   LONG m_knobHeight;
   LONG m_knobPos;
   AQIcon m_lessIcon;
   AQIcon m_moreIcon;
};
   
#endif
