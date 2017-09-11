#ifndef AQSPLITTER_H
#define AQSPLITTER_H

#include <AQWidget.h>

class SplitLayout;

class AQSplitter : public AQWidget
{
public:
   AQSplitter(bool horizontal, AQWidget *parent = nullptr);
   ~AQSplitter();

   void setWidgets(AQWidget *first, AQWidget *second);

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);

private:
   AQWidget *m_first;
   AQWidget *m_second;
   bool m_horiz;
   bool m_pressed;
   SplitLayout *m_splitLayout;
};
   
#endif
