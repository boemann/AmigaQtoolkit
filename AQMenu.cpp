#include "AQMenu.h"

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

const int SeparatorType = 0;
const int ActionType = 1;
const int MenuType = 2;

AQMenu::AQMenu(const AQString &title, AQWidget *parent)
   : AQWidget(parent)
   , m_hoveredItem(-1)
   , m_execed(false)
   , m_barMode(false)
   , m_title(title)
   , m_shownSubMenu(nullptr)
{
   setBgPen(2);

   setExpanding(false, false);
   setPreferredSize(AQPoint(8, 4));
   setMinimumSize(AQPoint(8, 4));
   setWindowModality(2);
   setWindowFlags(AQWindow::Popup);
}

AQMenu::~AQMenu()
{
}

void AQMenu::setMenubarMode(bool on)
{
   m_barMode = on;
}

void AQMenu::exec(const AQPoint &pos)
{
   updateSize();

   setPos(pos);
   m_hoveredItem -1;
   show();
   sendFakeMouseMove(true); // change to false for non-hold-behaviour

   grabMouse();
   m_execed = true;
   aqApp->processEvents(m_execed);
   releaseMouse();
   effectuate();
}

void AQMenu::popup(const AQPoint &pos)
{
   updateSize();

   setPos(pos);
   m_hoveredItem -1;
   show();
   sendFakeMouseMove(true); // keep true
}

void AQMenu::addSeparator()
{
   m_entryType.push_back(SeparatorType);
   m_entryAction.push_back(nullptr);
   m_entryMenu.push_back(nullptr);
}

void AQMenu::addAction(AQAction *action)
{
   m_entryType.push_back(ActionType);
   m_entryAction.push_back(action);
   m_entryMenu.push_back(nullptr);
}

void AQMenu::addMenu(AQMenu *menu)
{
   menu->setParent(this);
   m_entryType.push_back(MenuType);
   m_entryAction.push_back(nullptr);
   m_entryMenu.push_back(menu);
}

void AQMenu::updateSize()
{
   RastPort rp;
   InitRastPort(&rp);
   SetFont(&rp, font());

   int width = 8;
   int checkWidth = 0;
   int scWidth = 0;
   int height = 4;

   if (m_barMode) {
      setMinimumSize(aqApp->screen()->menubarSize());
      setPreferredSize(minimumSize());
      return;
   }

   for(int i = 0; i <m_entryType.size(); ++i) {
      switch (m_entryType[i]) {
      case SeparatorType:
         height += 6;
         break;
      case ActionType: {
         if (m_entryAction[i]->isCheckable())
            checkWidth = 16;

         AQString text = m_entryAction[i]->text();
         width = aqMax(14 + TextLength(&rp, text, text.size()), width);

         AQString scText = m_entryAction[i]->shortcutSansQualifier();
         int w = TextLength(&rp, scText, scText.size());
         if (m_entryAction[i]->hasAltQualifier())
            w += 14+2;
         if (m_entryAction[i]->hasShiftQualifier())
            w += 11+2;
         if (m_entryAction[i]->hasAmigaQualifier())
            w += 22;
         scWidth = aqMax(w, scWidth);
         
         height += rp.TxHeight + 2;
         break;
      }
      case MenuType: {
         AQString text = m_entryMenu[i]->m_title;
         width = aqMax(14 + TextLength(&rp, text, text.size()), width);

         AQString scText = "»";
         scWidth = aqMax(TextLength(&rp, scText, scText.size()), scWidth);

         height += rp.TxHeight + 2;
         break;
      }
      }
   }

   if (scWidth)
      scWidth += 6;

   setMinimumSize(AQPoint(checkWidth + width + scWidth, height));
   setPreferredSize(minimumSize());
}

void AQMenu::closeEvent()
{
}

void AQMenu::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define DARK 1
#define SHINE 2

   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   SetAPen(rp, DARK);
   if (m_barMode) {
      Move(rp, 0, bottom);
      Draw(rp, right, bottom);
   } else {
      Move(rp, right, 0);
      Draw(rp, right, bottom);
      Draw(rp, 0, bottom);
      Draw(rp, 0, 0);
      Draw(rp, right-1, 0);
      Draw(rp, right-1, bottom);

      Move(rp, 1,0);
      Draw(rp, 1, bottom);
   }

   SetDrMd(rp, JAM1);
   int y = rp->TxBaseline + 3;
   int x = 3;
   for(int i = 0; i <m_entryType.size(); ++i) {
      SetAPen(rp, DARK);
   
      switch (m_entryType[i]) {
      case SeparatorType: {
         USHORT pt[2] = {0xDDDD, 0x7777};
         SetAfPt(rp, pt,1);
         RectFill(rp, 6, y - rp->TxBaseline +1, right - 6, y - rp->TxBaseline +2);
         SetAfPt(rp, nullptr, 0);
         y += 6;
         break;
      }
      case ActionType: {
         int state = m_entryAction[i]->enabled() ? 1 : 0;
         if (state == 0)
            SetAPen(rp, 0);
         
         Move(rp, 6, y);

         if (m_hoveredItem == i && m_entryAction[i]->enabled()) {
            RectFill(rp, 4, y - rp->TxBaseline - 1, right - 4, y + 2);
            SetAPen(rp, 2);
            state = 2;
         }

         if (m_entryAction[i]->isCheckable()) {
            if (m_entryAction[i]->isChecked()) {
               AQIcon i(m_entryAction[i]->isMutexed() ? "bullet" : "checkmark");
               i.paint(rp, AQPoint(6,y -rp->TxBaseline), AQIcon::Small, state);
            }
            Move(rp, 6 + 16, y);
         }

         AQString text = m_entryAction[i]->text();

         Text(rp, text, text.size());

         AQString scText = m_entryAction[i]->shortcutSansQualifier();
         int n = m_entryAction[i]->numQualifiers();
         int tx = right - 6 - TextLength(rp, scText, scText.size());
         if (n) {
            int ttx = tx;
            if (m_entryAction[i]->hasAmigaQualifier()) {
               AQIcon i("amigakey");
               ttx -= i.size().x + 2;
               i.paint(rp, AQPoint(ttx,y -rp->TxBaseline), AQIcon::Small, state);
            }

            if (m_entryAction[i]->hasAltQualifier()) {
               AQIcon i("altkey");
               ttx -= i.size().x + 2;
               i.paint(rp, AQPoint(ttx,y -rp->TxBaseline), AQIcon::Small, state);
            }

            if (m_entryAction[i]->hasShiftQualifier()) {
               AQIcon i("shiftkey");
               ttx -= i.size().x + 2;
               i.paint(rp, AQPoint(ttx,y -rp->TxBaseline), AQIcon::Small, state);
            }

            if (m_entryAction[i]->hasCtrlQualifier()) {
               AQIcon i("ctrlkey");
               ttx -= i.size().x + 2;
               i.paint(rp, AQPoint(ttx,y -rp->TxBaseline), AQIcon::Small, state);
            }

         }
         Move(rp, tx, y);
         Text(rp, scText, scText.size());

         y += rp->TxHeight + 2;
         break;
      }
      case MenuType: {
         AQString text = m_entryMenu[i]->m_title;


         if (m_barMode) {
            Move(rp, x+4, y - 2);
            int w = TextLength(rp, text, text.size());
            if (m_hoveredItem == i) {
               RectFill(rp, x, 0, x + w + 7, y + 2);
               SetAPen(rp, 2);
            }

            Text(rp, text, text.size());

            x += w + 14;
         } else {
            Move(rp, 6, y);
            if (m_hoveredItem == i) {
               RectFill(rp, 4, y - rp->TxBaseline - 1, right - 4, y + 2);
               SetAPen(rp, 2);
            }

            Text(rp, text, text.size());

            Move(rp, right - 6 - TextLength(rp, "»", 1), y);
            Text(rp, "»", 1);

            y += rp->TxHeight + 2;
         }
         break;
      }
      }
   }
}

void AQMenu::sendFakeMouseMove(bool holdBehavior)
{
   IntuiMessage msg;
   AQPoint p;
   p.x = IntuitionBase->MouseX;
   p.y = IntuitionBase->MouseY;
   p = mapFromGlobal(p);
   msg.MouseX = p.x;
   msg.MouseY = p.y;
   mouseMoveEvent(msg);

   m_firstUpReceived = holdBehavior;
}

void AQMenu::effectuate()
{
   if (m_hoveredItem != -1) {
      if (m_entryType[m_hoveredItem] == MenuType)
         m_entryMenu[m_hoveredItem]->effectuate();
      else if (m_entryType[m_hoveredItem] == ActionType)
         m_entryAction[m_hoveredItem]->trigger();
   }
}

bool AQMenu::mousePressEvent(const IntuiMessage &msg)
{
   return true;
}


bool AQMenu::mouseMoveEvent(const IntuiMessage &msg)
{
   if (msg.MouseX > 1 && msg.MouseY > 1)
      m_firstUpReceived = true;


   if (m_shownSubMenu) {
      IntuiMessage m = msg;
      AQPoint p = mapToGlobal(AQPoint(m.MouseX, m.MouseY));
      p = m_shownSubMenu->mapFromGlobal(p);
      m.MouseX = p.x;
      m.MouseY = p.y;
      if (m_shownSubMenu->mouseMoveEvent(m))
         return true;
      m.MouseX = msg.MouseX;
      m.MouseY = msg.MouseY;
   }

   int item = -1;
   int x = 3;
   int y = 2;
   if (m_barMode) {
      RastPort rp;
      InitRastPort(&rp);
      SetFont(&rp, font());
      while (true) {
         if (msg.MouseX < x || item + 1 == m_entryType.size()) {
            item = -1;
            break;
         }
         ++item;

         AQString text = m_entryMenu[item]->m_title;
         int w = TextLength(&rp, text, text.size());

         if (msg.MouseX >= x && msg.MouseX < x + w + 7)
            break;

         x += w + 14;
      }

      if (msg.MouseY < 0 || msg.MouseY > size().y)
         item = -1;

      y = size().y - 2;
   } else {
      for(; item + 1 < m_entryType.size(); ) {
         if (msg.MouseY < y)
            break;
         ++item;
         if (m_entryType[item] == SeparatorType)
            y += 6;
         else
            y += font()->tf_YSize + 2;
      }

      if (msg.MouseX < 0 || msg.MouseX > size().x
                         || msg.MouseY > y)
         item = -1;

      x = size().x - 10;
      y -= font()->tf_YSize + 3;
   }

   if (item == -1 && m_shownSubMenu)
      return false;

   if (item != m_hoveredItem) {
      m_hoveredItem = item;
      update();
      scroll(AQPoint(0,0), AQRect(0,0,1,1)); // forces repaint
      
      if (m_shownSubMenu) {
         m_shownSubMenu->hide();
      }
      m_shownSubMenu = nullptr;
   }

   if (m_hoveredItem != -1 && m_entryType[m_hoveredItem] == MenuType) {
      m_shownSubMenu = m_entryMenu[m_hoveredItem];
      m_shownSubMenu->popup(pos() + AQPoint(x, y));
   }
   return item != -1;
}

bool AQMenu::mouseReleaseEvent(const IntuiMessage &msg)
{
   if (m_shownSubMenu) {
      m_shownSubMenu->mouseReleaseEvent(msg);
      m_shownSubMenu = nullptr;
   }

   if (m_firstUpReceived) {
      hide();
      m_execed = false;
      update();
   }
   return true;
}

