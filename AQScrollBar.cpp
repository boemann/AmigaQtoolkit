#include "AQScrollBar.h"

#include <graphics/gfxbase.h>
#include <stdio.h>

#ifdef __GNUC__
#include <proto/graphics.h>
#else
#include <pragma/graphics_lib.h>
#endif

AQScrollBar::AQScrollBar(bool horiz, AQWidget *parent)
   : AQWidget(parent)
   , m_horiz(horiz)
   , m_value(0)
   , m_pageStep(10)
   , m_singleStep(1)
   , m_wheelStep(3)
   , m_minimum(0)
   , m_maximum(90)
   , m_botH(7)
{
   setBgPen(-1); // no bg but we draw ourself

   if (horiz) {
      setExpanding(true, false);
      m_lessIcon = AQIcon("left");
      m_moreIcon = AQIcon("right");
   }
   else {
      setExpanding(false, true);
      setMinimumSize(AQPoint(7, 30));
      setPreferredSize(AQPoint(7, 30));
      m_lessIcon = AQIcon("up");
      m_moreIcon = AQIcon("down");
   }
   calculateKnob();
}

AQScrollBar::~AQScrollBar()
{

}

int AQScrollBar::value() const
{
   return m_value;
}

void AQScrollBar::setValue(int v)
{
   if (v > m_maximum - m_pageStep) {
      v = m_maximum - m_pageStep;
   }

   if (v < m_minimum)
      v = m_minimum;

   if (m_value == v)
      return;

   m_value = v;
   emit("valueChanged", m_value);
   calculateKnob();
   update();
}

void AQScrollBar::setMaximum(int m)
{
   if (m == m_maximum)
     return;

   m_maximum = m;

   if (m_value > m_maximum - m_pageStep)
      setValue(m_maximum - m_pageStep);

   calculateKnob();
   update();
}

void AQScrollBar::setPageStep(int step)
{
   if (step == m_pageStep)
     return;

   m_pageStep = step;
   if (m_value > m_maximum - m_pageStep)
      setValue(m_maximum - m_pageStep);

   calculateKnob();
   update();
}

void AQScrollBar::setSingleStep(int step)
{
   m_singleStep = step;
}

void AQScrollBar::setWheelStep(int step)
{
   m_wheelStep = step;
}

void AQScrollBar::calculateKnob()
{
   if (m_maximum - m_minimum > 0)
      m_knobHeight = m_pageStep * (size().y - m_botH - m_botH)
                    / (m_maximum - m_minimum);
   else
      m_knobHeight = size().y - m_botH - m_botH;

   if (m_knobHeight < 5)
      m_knobHeight = 5;
   if (m_knobHeight > size().y - m_botH - m_botH)
      m_knobHeight = size().y - m_botH - m_botH;

   if (m_maximum - m_minimum  - m_pageStep > 0)
      m_knobPos = (m_value - m_minimum) * (size().y - m_botH - m_botH - m_knobHeight)
                             / (m_maximum - m_minimum - m_pageStep);
   else
      m_knobPos = 0;

}

void AQScrollBar::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define DARK 1
#define SHINE 2
   if (m_horiz) {
   } else {
      LONG bottom = s.y - 1;
      LONG right = s.x - 1;

      SetAPen(rp, DARK);
      // arrow down
      m_moreIcon.paint(rp, AQPoint(0, bottom - m_botH + 2), AQIcon::Small);

      // arrow up
      bottom -= m_botH;
      m_lessIcon.paint(rp, AQPoint(0, bottom - m_botH + 2), AQIcon::Small);

      // slider background
      bottom -= m_botH;
      SetAPen(rp, SHINE);
      Move(rp, right, 0);
      Draw(rp, right, bottom);
      Draw(rp, 0, bottom);
      SetAPen(rp, DARK);
      Draw(rp, 0, 0);
      Draw(rp, right, 0);

      // slider knob
      SetAPen(rp, DARK);
      Move(rp, right, m_knobPos);
      Draw(rp, right, m_knobPos + m_knobHeight - 1);
      Draw(rp, 0, m_knobPos + m_knobHeight - 1);
      Draw(rp, 0, m_knobPos);
      if (m_knobPos) {
         SetAPen(rp, SHINE);
         Move(rp, 1, m_knobPos);
         Draw(rp, right, m_knobPos);
      }
   }
}

bool AQScrollBar::wheelEvent(bool up)
{
   if (up)
      setValue(m_value - m_wheelStep);
   else
      setValue(m_value + m_wheelStep);

   return true;
}

bool AQScrollBar::mousePressEvent(const IntuiMessage &msg)
{
   LONG pos = msg.MouseY;

   if (pos < m_knobPos)
      m_pressOffset = -1; // page up
   else if (pos < m_knobPos + m_knobHeight)
      m_pressOffset = pos - m_knobPos;
   else if (pos < size().y - m_botH - m_botH)
      m_pressOffset = -2; // page down
   else if (pos < size().y - m_botH)
      m_pressOffset = -3; // line up
   else
      m_pressOffset = -4; //l line down

   return true;
}

bool AQScrollBar::mouseMoveEvent(const IntuiMessage &msg)
{
   if (m_pressOffset < 0)
      return true;

   if (size().y - m_botH - m_botH - m_knobHeight > 0)
      setValue((msg.MouseY - m_pressOffset)
          * (m_maximum - m_minimum - m_pageStep)
          / (size().y - m_botH - m_botH - m_knobHeight) + m_minimum);
   else
      setValue(0);


   return true;
}

bool AQScrollBar::mouseReleaseEvent(const IntuiMessage &msg)
{
   switch (m_pressOffset) {
   case -1:
      setValue(m_value - m_pageStep);
      break;
   case -2:
      setValue(m_value + m_pageStep);
      break;
   case -3:
      setValue(m_value - m_singleStep);
      break;
   case -4:
      setValue(m_value + m_singleStep);
      break;
   default:
      return true;
   }

   return true;
}

void AQScrollBar::resizeEvent(const AQPoint &oldSize)
{
   calculateKnob();
}
