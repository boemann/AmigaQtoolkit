#include "AQToolTip.h"

#include <graphics/gfxmacros.h>
#include <graphics/gfxbase.h>
#include <intuition/intuitionbase.h>

#include <proto/graphics.h>

#include <AQApplication.h>
#include <AQString.h>
#include <AQAction.h>
#include <AQWindow.h>
#include <AQIcon.h>

#include <stdio.h>

extern struct IntuitionBase *IntuitionBase;


AQToolTip::AQToolTip(const AQString &text, AQWidget *parent)
   : AQWidget(parent)
   , m_execed(false)
   , m_text(text)
{
   setBgPen(2);

   setExpanding(false, false);
   setPreferredSize(AQPoint(50, 15));
   setMinimumSize(preferredSize());
   setWindowFlags(AQWindow::ToolTip);
}

AQToolTip::~AQToolTip()
{
}

void AQToolTip::popdown()
{
   hide();
   m_execed = false;
}

void AQToolTip::exec(const AQPoint &pos)
{
   updateSize();

   setPos(pos);
   show();

   m_execed = true;
   aqApp->processEvents(m_execed);
}


void AQToolTip::updateSize()
{
   RastPort rp;
   InitRastPort(&rp);
   SetFont(&rp, font());

   int width = 14 + TextLength(&rp, m_text, m_text.size());        
   int height = 6 + rp.TxHeight;

   setMinimumSize(AQPoint(width, height));
   setPreferredSize(minimumSize());
}

void AQToolTip::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define DARK 1

   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   SetAPen(rp, DARK);

   Move(rp, right, 0);
   Draw(rp, right, bottom);
   Draw(rp, 0, bottom);
   Draw(rp, 0, 0);
   Draw(rp, right-1, 0);
   Draw(rp, right-1, bottom);

   Move(rp, 1,0);
   Draw(rp, 1, bottom);

   SetDrMd(rp, JAM1);
   Move(rp, 7, 3 +rp->TxBaseline);
   Text(rp, m_text, m_text.size());

}

AQToolTip *AQToolTip::s_toolTip = nullptr;
bool AQToolTip::s_avaitReactivation = false;
int AQToolTip::s_timeCounter = 0;
AQPoint AQToolTip::s_stillMousePos = AQPoint();

void AQToolTip::filterIntuiMessage(IntuiMessage &msg)
{
   if (msg.Class == IDCMP_INTUITICKS) {
      AQPoint gPos(IntuitionBase->MouseX, IntuitionBase->MouseY);
      if (gPos == s_stillMousePos || s_timeCounter <= 0)
         ++s_timeCounter;
      else {
         s_timeCounter = 0;
         s_stillMousePos = gPos;
      }
      if (s_toolTip) {
         AQPoint pPos = s_toolTip->m_parent->mapFromGlobal(gPos);
         if (!AQRect(AQPoint(),s_toolTip->m_parent->size()).contains(pPos) ) {
            s_toolTip->popdown();
            s_timeCounter = -5;
            s_avaitReactivation = true;
         }
      }
   }

   if (msg.Class == IDCMP_ACTIVEWINDOW) {
      if (s_toolTip && s_toolTip->m_window->m_window != msg.IDCMPWindow) {
         s_toolTip->popdown();
         s_avaitReactivation = true;
         s_timeCounter = 0;
      }
   }

   if (msg.Class == IDCMP_MOUSEBUTTONS || msg.Class == IDCMP_VANILLAKEY || msg.Class == IDCMP_RAWKEY) {
      if (s_toolTip)
         s_toolTip->popdown();

      s_avaitReactivation = true;
      s_timeCounter = 0;
   }
}

void AQToolTip::checkShow(AQWidget *hoveredWidget)
{
   if ((s_timeCounter > 5 || s_timeCounter < 0)
         && hoveredWidget && !s_avaitReactivation
         && !s_toolTip && !hoveredWidget->toolTip().isEmpty()) {
      s_toolTip = new AQToolTip(hoveredWidget->toolTip(), hoveredWidget);
      AQPoint p(IntuitionBase->MouseX +16, IntuitionBase->MouseY+16);
      s_toolTip->exec(p);
      delete s_toolTip;
      s_toolTip = nullptr;
   }
}
