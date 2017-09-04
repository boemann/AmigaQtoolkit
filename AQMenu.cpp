#include "AQMenu.h"

#include <graphics/gfxmacros.h>
#include <graphics/gfxbase.h>

#ifdef __GNUC__
#include <proto/graphics.h>
#else
#include <pragma/graphics_lib.h>
#endif

#include <AQApplication.h>
#include <AQString.h>

AQMenu::AQMenu()
   : AQWidget(nullptr)
   , m_hoveredItem(-1)
{
   setBgPen(2);

   setExpanding(false, false);
   setPreferredSize(AQPoint(8, 4));
   setMinimumSize(AQPoint(8, 4));
   setWindowModality(2);
   setWindowFlags(0);
}

AQMenu::~AQMenu()
{
}

void AQMenu::exec(const AQPoint &pos)
{
   m_firstUpReceived = false;
   setPos(pos);
   show();
   grabMouse();
   m_visible = true;
   aqApp->processEvents(m_visible);
   releaseMouse();
}

void AQMenu::addSeparator()
{
   addEntry(AQString(""), 0, AQString(""), nullptr);
}

void AQMenu::addItem(const AQString &text)
{
   addEntry(AQString(text), 1, AQString("Ctrl+X"), nullptr);
}

void AQMenu::addEntry(const AQString &text, int type, const AQString &shortcut, AQMenu *subMenu)
{
   m_entryText.push_back(text);
   m_entryType.push_back(type);
   m_entryShortCut.push_back(shortcut);
   m_entrySubMenu.push_back(subMenu);

   RastPort rp;
   InitRastPort(&rp);
   int width = 14 + TextLength(&rp, text, text.size()) + 50;
   width = aqMax(minimumSize().x, width);
   int height = preferredSize().y;
   if (type)
      height += rp.TxHeight + 2;
   else
      height += 6;

   setMinimumSize(AQPoint(width, height));
   setPreferredSize(minimumSize());
}

void AQMenu::closeEvent()
{
}

void AQMenu::changeEvent(int change)
{
 //  if (change == AQWidget::InActivation)
   //   hide();
}

void AQMenu::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define DARK 1
#define SHINE 2

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
   int y = rp->TxBaseline + 3;
   for(int i = 0; i <m_entryText.size(); ++i) {
      SetAPen(rp, 1);
   
      if (m_entryType[i] == 0) {
         USHORT pt[2] = {0xDDDD, 0x7777};
         SetAfPt(rp, pt,1);
         RectFill(rp, 6, y-4, right - 6, y-3);
         SetAfPt(rp, nullptr, 0);
         y += 6;
      } else {
         Move(rp, 6, y);
         if (m_hoveredItem == i) {
            RectFill(rp, 4, y - rp->TxBaseline - 1, right - 4, y + 2);
            SetAPen(rp, 2);
         }

         Text(rp, m_entryText[i], m_entryText[i].size());

         if (m_entryType[i] == 2) {
            Move(rp, right - 6 - TextLength(rp, "»", 1), y);
            Text(rp, "»", 1);
         } else {
            Move(rp, right - 6 - TextLength(rp, m_entryShortCut[i], m_entryShortCut[i].size()), y);
            Text(rp, m_entryShortCut[i], m_entryShortCut[i].size());
         }
         y += rp->TxHeight + 2;
      }
   }
}

bool AQMenu::mousePressEvent(const IntuiMessage &msg)
{
   return true;
}


bool AQMenu::mouseMoveEvent(const IntuiMessage &msg)
{
   int oldItem = m_hoveredItem;

   m_hoveredItem = (msg.MouseY - 2 )/ 9;

   if (msg.MouseX > 1 && msg.MouseY > 1)
      m_firstUpReceived = true;

   if (msg.MouseX < 0 || msg.MouseX > size().x
                      || m_hoveredItem < 0 || m_hoveredItem >= m_entryText.size())
      m_hoveredItem = -1;

   if (oldItem != m_hoveredItem)
      update();

   return true;
}

bool AQMenu::mouseReleaseEvent(const IntuiMessage &msg)
{
   if (m_firstUpReceived) {
      hide();
      m_visible = false;
      update();
   }
   m_firstUpReceived = true;
   return true;
}

