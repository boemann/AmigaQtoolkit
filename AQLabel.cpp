#include "AQLabel.h"

#include <graphics/gfxmacros.h>
#include <graphics/gfxbase.h>

#ifdef __GNUC__
#include <proto/graphics.h>
#else
#include <pragma/graphics_lib.h>
#endif

AQLabel::AQLabel(AQWidget *parent)
   : AQWidget(parent)
{
   setBgPen(-1); // no bg but we draw ourself

   setExpanding(false, false);

   setMinimumSize(AQPoint(20, 11));
   setPreferredSize(AQPoint(20, 11));
}

AQLabel::~AQLabel()
{
}

void AQLabel::setText(const AQString &text)
{
   RastPort rp;
   InitRastPort(&rp);
   setMinimumSize(AQPoint(TextLength(&rp, text, text.size()), 11));
   setPreferredSize(minimumSize());

   m_text = text;
   
   update();
}

void AQLabel::setIcon(const AQIcon &icon)
{
   m_icon = icon;
   update();
}

void AQLabel::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();

   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   if (!m_text.isEmpty()) {
      SetAPen(rp, 1);
      SetDrMd(rp, JAM1);
      Move(rp, (s.x - TextLength(rp, m_text, m_text.size())) / 2
             , (s.y - rp->TxHeight)/2 + rp->TxBaseline);
      Text(rp, m_text, m_text.size());
   }
   if (!m_icon.isNull()) {
      m_icon.paint(rp, AQPoint(2, 1), AQIcon::Small);
   }
}

