#include "AQButton.h"

#include <graphics/gfxbase.h>

#include <proto/graphics.h>

#include <stdio.h>

AQButton::AQButton(bool toolMode, AQWidget *parent)
   : AQWidget(parent)
   , m_pressed(false)
   , m_toolMode(toolMode)
   , m_hovered(false)
   , m_checkable(false)
   , m_subButton(nullptr)
{
   setBgPen(-1); // no bg but we draw ourself

   setExpanding(false, false);

   recalcSizes();
}

AQButton::~AQButton()
{
}

void AQButton::setCheckable(bool c)
{
   m_checkable = c;
}

bool AQButton::isChecked() const
{
  return m_checkable && m_pressed;
}

void AQButton::setChecked(bool c)
{
   if (!m_checkable)
      return;

   m_pressed = c;
   update();   
}

void AQButton::setShowSubButton()
{
   m_subButton = new AQButton(true, this);
   m_subButton->setText("x");
   Connect<AQButton>(m_subButton, "clicked", this, &AQButton::onSubClicked);
   recalcSizes();
}

void AQButton::setText(const AQString &text)
{
   m_text = text;

   recalcSizes();
}

void AQButton::setIcon(const AQIcon &icon)
{
   m_icon = icon;

   recalcSizes();
}

void AQButton::recalcSizes()
{
   RastPort rp;
   InitRastPort(&rp);
   SetFont(&rp, font());

   int w = m_icon.isNull() ? 0 : m_icon.size().x;
   if (!m_text.isEmpty())
      w += TextLength(&rp, m_text, m_text.size());

   w += m_toolMode ? 2 : 10;

   if (m_subButton) {
      w += 4;
      m_subButton->setPos(AQPoint(w, 1));
      m_subButton->setSize(m_subButton->preferredSize()-AQPoint(2,2));
      w += m_subButton->size().x;
   }

   w += m_toolMode ? 2 : 10;

   setMinimumSize(AQPoint(w, 4 + rp.TxHeight));
   setPreferredSize(minimumSize());
   updateGeometry();
}

void AQButton::enterEvent()
{
   m_hovered = true;
   update();
}

void AQButton::leaveEvent()
{
   m_hovered = false;
   update();
}

void AQButton::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define DARK 1
#define SHINE 2

   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   if (m_pressed) {
      SetAPen(rp, SHINE);
      Move(rp, right, 0);
      Draw(rp, right, bottom);
      Draw(rp, 0, bottom);

      SetAPen(rp, DARK);
      Draw(rp, 0, 0);
      Draw(rp, right, 0);
   } else if (m_toolMode == false) {
      SetAPen(rp, DARK);
      Move(rp, right, 0);
      Draw(rp, right, bottom);
      Draw(rp, 0, bottom);
      SetAPen(rp, SHINE);
      Draw(rp, 0, 0);
      Draw(rp, right, 0);
   } else if (m_hovered == true) {
      SetAPen(rp, 3);
      Move(rp, right, 0);
      Draw(rp, right, bottom);
      Draw(rp, 0, bottom);
      Draw(rp, 0, 0);
      Draw(rp, right, 0);
   }

   if (!m_text.isEmpty()) {
      SetAPen(rp, DARK);
      SetDrMd(rp, JAM1);
      if (m_toolMode)
         Move(rp, 2, (s.y - rp->TxHeight)/2 + rp->TxBaseline);
      else
         Move(rp, (s.x - TextLength(rp, m_text, m_text.size())) / 2
                , (s.y - rp->TxHeight)/2 + rp->TxBaseline);
      Text(rp, m_text, m_text.size());
   }
   if (!m_icon.isNull()) {
      m_icon.paint(rp, AQPoint(2, (s.y - m_icon.size().y) / 2), AQIcon::Small);
   }
}

bool AQButton::mousePressEvent(const IntuiMessage &msg)
{
   if (m_checkable)
      m_pressed = !m_pressed;
   else
      m_pressed = true;
   update();

   return true;
}

bool AQButton::mouseMoveEvent(const IntuiMessage &msg)
{
   return true;
}

bool AQButton::mouseReleaseEvent(const IntuiMessage &msg)
{
   if (!m_checkable)
      m_pressed = false;

   update();
   emit("clicked", m_checkable ? m_pressed : true);
   return true;
}

void AQButton::onSubClicked()
{
   emit("subClicked");
}