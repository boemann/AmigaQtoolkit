
#include <graphics/gfxbase.h>

#include <proto/graphics.h>

#include <AQLayout.h>

#include "AQSplitter.h"

class SplitLayout : public AQLayout
{
public:
   SplitLayout(bool horiz, AQWidget *w, AQWidget *o)
      : AQLayout(horiz)
      , m_widget(w)
      , m_otherWidget(o)
      , m_horiz(horiz)
   {
      if (horiz)
         m_val = w->preferredSize().x;
      else
         m_val = w->preferredSize().y;
      
      addWidget(m_widget);
      addItem(new AQSpacerItem(AQPoint(0,0), !m_horiz, m_horiz));
      addWidget(m_otherWidget);
   }
   ~SplitLayout() {}

   void layout(const AQPoint &size) {
      int val = m_val;
      const int space = 3;
      if (m_horiz) {
         if (val < m_widget->minimumSize().x)
            val = m_widget->minimumSize().x;

         if (val > size.x - space - m_otherWidget->minimumSize().x)
            val = size.x - space - m_otherWidget->minimumSize().x;

         m_widget->setPos(AQPoint());
         m_widget->setSize(AQPoint(val, size.y));

         m_otherWidget->setPos(AQPoint(val + space, 0));
         m_otherWidget->setSize(AQPoint(size.x - val - space, size.y));
      } else {
         if (val < m_widget->minimumSize().y)
            val = m_widget->minimumSize().y;

         if (val > size.y - space - m_otherWidget->minimumSize().y)
            val = size.y - space - m_otherWidget->minimumSize().y;

         m_widget->setPos(AQPoint());
         m_widget->setSize(AQPoint(size.x, val));

         m_otherWidget->setPos(AQPoint(0, val + space));
         m_otherWidget->setSize(AQPoint(size.x, size.y - val - space));
      }
   }

   void setSplitValue(int v) {
      if (v < 2)
         v = 2;
      if (v == m_val)
         return;
      m_val=v;
      m_widget->updateGeometry();
   }

private:
   AQWidget *m_widget;
   AQWidget *m_otherWidget;
   bool m_horiz;
   int m_val;
};

AQSplitter::AQSplitter(bool horizontal, AQWidget *parent)
   : AQWidget(parent)
   , m_first(nullptr)
   , m_second(nullptr)
   , m_horiz(horizontal)
   , m_pressed(false)
   , m_splitLayout(nullptr)
{
   setBgPen(-1); // no bg but we draw ourself
}

AQSplitter::~AQSplitter()
{
}

void AQSplitter::setWidgets(AQWidget *first, AQWidget *second)
{
   m_first = first;
   m_second = second;
   first->setParent(this);
   second->setParent(this);
   m_splitLayout = new SplitLayout(m_horiz, first, second);
   setLayout(m_splitLayout);
}

void AQSplitter::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   SetAPen(rp, 1);
   if (m_horiz) {
      int y = bottom / 2;
      WritePixel(rp, m_first->size().x+1, y-4);
      WritePixel(rp, m_first->size().x+1, y);
      WritePixel(rp, m_first->size().x+1, y+4);
   } else {
      int x = right / 2;
      WritePixel(rp, x - 4, m_first->size().y+1);
      WritePixel(rp, x,     m_first->size().y+1);
      WritePixel(rp, x + 4, m_first->size().y+1);
   }
}

bool AQSplitter::mousePressEvent(const IntuiMessage &msg)
{
   if (m_horiz) {
      if (msg.MouseX >= m_first->size().x && msg.MouseX < m_second->pos().x) {
         m_pressed = true;
         return true;
      }
   } else {
      if (msg.MouseY >= m_first->size().y && msg.MouseY < m_second->pos().y) {
         m_pressed = true;
         return true;
      }
   }
   return false;
}

bool AQSplitter::mouseMoveEvent(const IntuiMessage &msg)
{
   if (m_pressed) {
      if (m_horiz)
         m_splitLayout->setSplitValue(msg.MouseX);
      else
         m_splitLayout->setSplitValue(msg.MouseY);
   }

   return true;
}

bool AQSplitter::mouseReleaseEvent(const IntuiMessage &msg)
{
   m_pressed = false;
   update();
   return true;
}




