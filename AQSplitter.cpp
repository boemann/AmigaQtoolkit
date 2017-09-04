
#include <graphics/gfxbase.h>

#ifdef __GNUC__
#include <proto/graphics.h>
#else
#include <pragma/graphics_lib.h>
#endif

#include <AQLayout.h>

#include "AQSplitter.h"

class LimitingItem : public AQLayoutItem
{
public:
   LimitingItem(bool horiz, AQWidget *w)
      : AQLayoutItem(nullptr)
      , m_widget(w)
      , m_horiz(horiz)
   {
      if (horiz)
         m_val = w->preferredSize().x;
      else
         m_val = w->preferredSize().y;
   }
   ~LimitingItem() {}

   AQPoint minimumSize() const {
      if (m_horiz)
         return AQPoint(aqMax(m_val, m_widget->minimumSize().x), m_widget->minimumSize().y);
      else
         return AQPoint(m_widget->minimumSize().x, aqMax(m_val, m_widget->minimumSize().y));
   }

   AQPoint preferredSize() const  {
      if (m_horiz)
         return AQPoint(aqMax(m_val, m_widget->minimumSize().x), m_widget->preferredSize().y);
      else
         return AQPoint(m_widget->preferredSize().x, aqMax(m_val, m_widget->minimumSize().y));
   }
   void layout(const AQPoint &size) {m_widget->setSize(size);}
   bool likeToExpandX() const {if (m_horiz) return false; else return m_widget->likeToExpandX();}
   bool likeToExpandY() const {if (!m_horiz) return false; else return m_widget->likeToExpandY();}
   void setPos(const AQPoint &pos) {m_widget->setPos(pos);}
   void setSplitValue(int v) {m_val=v; m_widget->updateGeometry();}

private:
  AQWidget *m_widget;
  bool m_horiz;
  int m_val;
};

AQSplitter::AQSplitter(bool horizontal, AQWidget *parent)
   : AQWidget(parent)
   , m_first(nullptr)
   , m_second(nullptr)
   , m_horiz(horizontal)
   , m_pressed(false)
   , m_limitingItem(nullptr)
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
   AQLayout *m_sLayout = new AQLayout(m_horiz);
   m_limitingItem = new LimitingItem(m_horiz, first);
   m_sLayout->addItem(m_limitingItem);
   m_sLayout->addItem(new AQSpacerItem(AQPoint(0,0), !m_horiz, m_horiz));
   m_sLayout->addWidget(second);
   setLayout(m_sLayout);
}

void AQSplitter::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   SetAPen(rp, 1);
   int y = bottom / 2;
   WritePixel(rp, m_first->size().x+2, y-4);
   WritePixel(rp, m_first->size().x+2, y);
   WritePixel(rp, m_first->size().x+2, y+4);
}

bool AQSplitter::mousePressEvent(const IntuiMessage &msg)
{
   if (msg.MouseX >= m_first->size().x && msg.MouseX < m_second->pos().x) {
      m_pressed = true;
      return true;
   }
   return false;
}

bool AQSplitter::mouseMoveEvent(const IntuiMessage &msg)
{
   if (m_pressed) {
      m_limitingItem->setSplitValue(msg.MouseX);
   }
   return true;
}

bool AQSplitter::mouseReleaseEvent(const IntuiMessage &msg)
{
   m_pressed = false;
   update();
   return true;
}

