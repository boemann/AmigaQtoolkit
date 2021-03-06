 #include "AQWindow.h"

#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include <devices/inputevent.h>
#include <utility/hooks.h>

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/layers.h>

#include <AQApplication.h>
#include <AQWidget.h>
#include <AQMenu.h>

void backfill()
{

}

Hook refreshHook;

AQWindow::AQWindow(AQWidget *widget, int modality, ULONG flags)
   : m_window(nullptr)
   , m_active((flags & ToolTip) != ToolTip)
   , m_winControl(WidgetArea)
   , m_flags(Flags(flags & Normal))
   , m_widget(widget)
   , m_buttonDownWidget(nullptr)
   , m_dirtyRegion(NewRegion())
   , m_screen(aqApp->screen(widget))
{
   refreshHook.h_Entry = (ULONG (*)() )backfill;

   if (flags & Popup)
      aqApp->registerPopupWindow(this);
   else if (modality > 0)
      aqApp->registerModalWindow(this);
   else
      aqApp->registerWindow(this);

   if (m_flags & (TitleBar | CloseButton)) {
      m_border = AQPoint(4,2);//(4, 3);
      m_titleHeight = m_screen->m_drawInfo->dri_Font->tf_YSize + 2;
   } else  {
      m_border = AQPoint(0,0);
      m_titleHeight = 0;
   }

   AQPoint prefSize = widget->preferredSize() + m_border + m_border;
   prefSize.y += m_titleHeight;

   ULONG borderflag = 0;
   if (m_flags & CloseButton)
      borderflag |= WFLG_CLOSEGADGET;
   if ((m_flags & TitleBar) && m_widget->m_windowModality == 0)
      borderflag |= WFLG_DEPTHGADGET;
   if ((m_flags & TitleBar) == 0 && !borderflag)
      borderflag = WFLG_BORDERLESS;

   m_window = OpenWindowTags(NULL,
   WA_Width, prefSize.x,
   WA_Height, prefSize.y,
   WA_Left, widget->pos().x,
   WA_Top, widget->pos().y,
   WA_Activate, m_active,
   WA_Flags, WFLG_RMBTRAP | borderflag,
   (flags & AQWindow::TitleBar ? WA_Title : TAG_SKIP), (ULONG)(char *)m_widget->m_title,
   WA_SimpleRefresh, TRUE,
   WA_MinWidth, 15,
   WA_MinHeight, 15,
   WA_ReportMouse, TRUE,
   WA_NewLookMenus, TRUE,
   WA_CustomScreen, (ULONG)m_screen->m_screen,
   WA_BackFill, (ULONG)&refreshHook,
   TAG_DONE, 0L);
   
   m_window->UserPort = aqApp->userPort();

   ModifyIDCMP(m_window, CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_RAWKEY
    | IDCMP_REFRESHWINDOW | IDCMP_MOUSEBUTTONS | IDCMP_MOUSEMOVE
    | IDCMP_ACTIVEWINDOW | IDCMP_INACTIVEWINDOW
       | IDCMP_INTUITICKS | IDCMP_CHANGEWINDOW);

   SetFont(m_window->RPort, m_screen->m_drawInfo->dri_Font);

//   m_widget->m_palette = new AQPalette();
//   m_widget->m_palette->ShadowPen = m_drawInfo->dri_Pens[SHADOWPEN]);
//   m_widget->m_palette->ShinePen = m_drawInfo->dri_Pens[SHINEPEN]);
//   m_widget->m_palette->DarkPen = m_drawInfo->dri_Pens[SHADOWPEN]);
//   m_widget->m_palette->BasePen = m_drawInfo->dri_Pens[SHINEPEN]);
//   m_widget->m_palette->TextPen = m_drawInfo->dri_Pens[TEXTPEN]);


   m_widget->m_window = this;

   m_clientPos = m_border + AQPoint(0, m_titleHeight);
   m_widget->m_pos = AQPoint(m_window->LeftEdge, m_window->TopEdge);
   AQPoint size(AQPoint(m_window->Width,  m_window->Height) - m_clientPos - m_border);
   m_widget->setSize(size);
   paintAll();
}

AQWindow::~AQWindow()
{
   hide();
}

AQWidget *AQWindow::widget() const
{
   return m_widget;
}

void AQWindow::hide()
{
   if (m_window == nullptr)
      return;

   Forbid();

   struct IntuiMessage *imsg;
   struct Node *succ;
   imsg = (IntuiMessage *)m_window->UserPort->mp_MsgList.lh_Head;
   while ((succ = imsg->ExecMessage.mn_Node.ln_Succ) != nullptr) {
      if (imsg->IDCMPWindow == m_window) {
         Remove((Node *)imsg);
         ReplyMsg((Message *)imsg);
      }
      imsg = (IntuiMessage *)succ;
   }

   m_window->UserPort = nullptr;
   ModifyIDCMP(m_window, 0L);

   aqApp->unregisterWindow(this);

   Permit();

   CloseWindow(m_window);

   m_window = nullptr;
}

void AQWindow::setPos(const AQPoint &p)
{
   ChangeWindowBox(m_window, p.x, p.y, m_window->Width, m_window->Height);
}

AQPoint AQWindow::clientOffset() const
{
   return m_clientPos;
}

AQWidget *AQWindow::s_mouseGrabWidget = nullptr;

AQWidget *AQWindow::pickMouseReceiver(IntuiMessage &msg)
{
   if (s_mouseGrabWidget) {
      AQPoint pos(msg.MouseX, msg.MouseY);
      pos += m_widget->pos();
      pos = s_mouseGrabWidget->mapFromGlobal(pos);
      msg.MouseX = pos.x;
      msg.MouseY = pos.y;
      return s_mouseGrabWidget;
   }

   if (aqApp->isWindowBlocked(this))
      return nullptr;

   if (m_buttonDownWidget) {
      AQPoint pos(msg.MouseX, msg.MouseY);
      pos += m_widget->pos();
      pos = m_buttonDownWidget->mapFromGlobal(pos);
      msg.MouseX = pos.x;
      msg.MouseY = pos.y;
      return m_buttonDownWidget;
   }

   if (msg.Class == IDCMP_MOUSEMOVE || msg.Code == SELECTUP || msg.Code == MENUUP)
      return nullptr;

   m_x = msg.MouseX;
   m_y = msg.MouseY;

   hoverTest(msg, true);

   if (m_winControl == Hover || m_winControl == 0) {
      msg.MouseX -= m_clientPos.x;
      msg.MouseY -= m_clientPos.y;
      return m_widget->widgetAt(msg.MouseX, msg.MouseY);
   }

   return nullptr;
}

void AQWindow::event(IntuiMessage &msg)
{
   if (m_window->WLayer->Flags & LAYERREFRESH) {
      LockLayerInfo(&m_window->WScreen->LayerInfo);

      OrRegionRegion(m_window->WLayer->DamageList, m_dirtyRegion);

      BeginRefresh(m_window);
      EndRefresh(m_window, true);

      Region *oldClip = InstallClipRegion(m_window->WLayer, m_dirtyRegion);

      paintAll(false);

      InstallClipRegion(m_window->WLayer, oldClip);
      UnlockLayerInfo(&m_window->WScreen->LayerInfo);
      paintBorder();
   }
   
   switch (msg.Class) {
   case MENUPICK:
      // transform and make menuitems emit;
      break;

   case IDCMP_REFRESHWINDOW:
      // we handle it (and more) above
      break;

   case IDCMP_RAWKEY:
      if (msg.Code == AQWidget::Key_WheelUp || msg.Code == AQWidget::Key_WheelDown) {
         AQWidget *receiver = pickMouseReceiver(msg);

         if (receiver) {
            if (!receiver->wheelEvent(msg.Code == AQWidget::Key_WheelUp)) {
               receiver = aqApp->focusWidget();
               if (receiver)
                  receiver->wheelEvent(msg.Code == AQWidget::Key_WheelUp);
            }
         }
         break;
      }
      // intentional fallthrough
   case IDCMP_VANILLAKEY:
     if (!aqApp->isWindowBlocked(this)) {
         AQWidget *receiver = aqApp->focusWidget();
         while (receiver && !receiver->event(msg)) {
            if (receiver->isTopLevel())
               receiver = nullptr;
            else {
               receiver = receiver->parent();
            }
         }
      }
      break;

   case IDCMP_MOUSEBUTTONS: {
      AQWidget *receiver = pickMouseReceiver(msg);

      if (receiver) {
         while (receiver && !receiver->event(msg)) {
            if (receiver->isTopLevel())
               receiver = nullptr;
            else {
               receiver = receiver->parent();
               msg.MouseX += receiver->pos().x;
               msg.MouseY += receiver->pos().y;
            }
         }

         // receiver is valid if we broke loop because event() gave true
         if (msg.Code == SELECTDOWN || msg.Code == MIDDLEDOWN || msg.Code == MENUUP)
            m_buttonDownWidget = receiver;
         else
            m_buttonDownWidget = nullptr;
      } else
         testFrameClick(msg);
      if (!receiver && msg.Code == MENUDOWN && m_widget->m_menu) {
         m_widget->m_menu->exec(AQPoint());
         m_buttonDownWidget = nullptr;
      }

      break;
   }
   case IDCMP_MOUSEMOVE: {
      AQWidget *receiver = pickMouseReceiver(msg);

      if (receiver) {
         while (receiver && !receiver->event(msg)) {
            if (receiver->isTopLevel())
               receiver = nullptr;
            else {
               receiver = receiver->parent();
               msg.MouseX += receiver->pos().x;
               msg.MouseY += receiver->pos().y;
            }
         }
      }
      break;
   }
   case IDCMP_INTUITICKS:
      if (!aqApp->isWindowBlocked(this))
         if (m_buttonDownWidget == nullptr && s_mouseGrabWidget == nullptr) {
            hoverTest(msg, !msg.Qualifier & IEQUALIFIER_LEFTBUTTON);

            if (m_winControl == Hover || m_winControl == 0) {
               msg.MouseX -= m_clientPos.x;
               msg.MouseY -= m_clientPos.y;

               aqApp->setHoveredWidget(m_widget->widgetAt(msg.MouseX, msg.MouseY));
            }
         }
      break;

   case IDCMP_CHANGEWINDOW: {
      AQPoint newPos(m_window->LeftEdge, m_window->TopEdge);
      if (m_widget->pos() != newPos)
         m_widget->m_pos = AQPoint(m_window->LeftEdge, m_window->TopEdge);
      AQPoint size(m_window->Width,  m_window->Height);
      size = size - m_border - m_clientPos;
      if (m_widget->size() != size) {
         m_widget->setSize(size);
         paintAll();
      }
      break;
   }
            
   case IDCMP_ACTIVEWINDOW:
      paintBorder();
      if (m_active)
         break;
      m_active = true;
      m_widget->changeEvent(AQWidget::Activation);
      paintAll();
      break;
   case IDCMP_INACTIVEWINDOW:
      if (!m_active)
         break;
      m_active = false;
      m_widget->changeEvent(AQWidget::InActivation);
      paintAll();
      break;
    
   case CLOSEWINDOW:
      if (aqApp->isWindowBlocked(this))
         return;

      m_widget->closeEvent();
      break;
   }
}

void AQWindow::paintWidget(AQWidget *w, RastPort *rp, AQRect rect, int winBg)
{
   if (!w->m_visible)
      return;

   AQRect clip(rect.intersected(w->geometry()));

   if (!clip.isValid()) // nothing to paint for this widget within cliprect
      return;

   AQPoint pos = w->pos();

   // offset painting by calling ScrollLayer (misnomer but it works as an offset for Gfx)
   ScrollLayer(0, rp->Layer, -pos.x, -pos.y);
   clip.translate(-pos);    // make clip as seen by the widget

   winBg = w->bgPen() >= 0 ? w->bgPen() : winBg;

   // Let's see if a child can handle all of the clip so we can defer
   for (int i = 0; i < w->children().size(); ++i) {
      AQWidget *child = w->children()[i];

      if (child->isTopLevel())
         continue;
      if (child->m_visible && child->geometry().contains(clip)) {
         // child can handle all of the clip
         paintWidget(child, rp, clip, winBg);
         ScrollLayer(0, rp->Layer, pos.x, pos.y);  // undo the painting offset
         return;
      }
   }

   if (winBg >= 0) {
      SetAPen(rp, winBg);
      RectFill(rp, clip.topLeft.x, clip.topLeft.y, clip.bottomRight.x, clip.bottomRight.y);
      winBg = -1; // mark we have painted the background
   }
   
   w->paintEvent(rp, clip);

   // now paint children
   for (int i = 0; i < w->children().size(); ++i) {
      AQWidget *child = w->children()[i];
      if (child->isTopLevel())
         continue;
      paintWidget(child, rp, clip, winBg);
   }

   ScrollLayer(0, rp->Layer, pos.x, pos.y);  // undo the painting offset
}

void AQWindow::paintDirty()
{
   if (!m_window)
      return;

   RastPort *rp = m_window->RPort;

   AQPoint pos(m_window->LeftEdge, m_window->TopEdge);
   pos -= m_clientPos;
   // offset painting by calling ScrollLayer (misnomer but it works as an offset for Gfx)
   ScrollLayer(0, rp->Layer, pos.x, pos.y);

   RegionRectangle *reg = m_dirtyRegion->RegionRectangle;

   AQWidget *w = m_widget; 
   while (reg) {
      AQRect rect(m_dirtyRegion->bounds.MinX + reg->bounds.MinX,
                  m_dirtyRegion->bounds.MinY + reg->bounds.MinY,
                  reg->bounds.MaxX - reg->bounds.MinX + 1,
                  reg->bounds.MaxY - reg->bounds.MinY + 1);

      rect.translate(pos);    // make rect as seen globally

      int winBg = m_active ? m_screen->m_drawInfo->dri_Pens[FILLPEN] : 0;
      winBg = 0;
      paintWidget(m_widget, rp, rect, winBg);

      reg = reg->Next;
   }

   ClearRegion(m_dirtyRegion);
   ScrollLayer(0, rp->Layer, -pos.x, -pos.y);
}

void AQWindow::scroll(const AQPoint &delta, const AQRect &rect)
{
   AQRect r(rect.topLeft + m_clientPos, rect.bottomRight + m_clientPos);

   if (m_window) {
      paintDirty(); // make sure any previous scrolls have been dealt with
      ScrollWindowRaster(m_window, delta.x, delta.y, r.topLeft.x, r.topLeft.y,
                         r.bottomRight.x, r.bottomRight.y);
      // should also update the exposed
      if (delta.y > 0)
         markDirty(AQRect(
                    AQPoint(rect.topLeft.x, rect.bottomRight.y - delta.y)
                  , rect.bottomRight));

      if (delta.y < 0)
         markDirty(AQRect(rect.topLeft,
                     AQPoint(rect.bottomRight.x, rect.topLeft.y - delta.y)));

   }
}


void AQWindow::markDirty(const AQRect &rect)
{
   Rectangle rectangle = {m_clientPos.x + rect.topLeft.x,
                          m_clientPos.x + rect.topLeft.y,
                          m_clientPos.x + rect.bottomRight.x,
                          m_clientPos.y + rect.bottomRight.y};

   OrRectRegion(m_dirtyRegion, &rectangle);
}

void AQWindow::paintBorder()
{
   if (m_border.x == 0)
      return;

   ULONG old = m_window->Flags;

   m_window->Flags = (m_window->Flags & ~WFLG_WINDOWACTIVE);
   m_window->Flags |= m_active ? WFLG_WINDOWACTIVE : 0;
   RefreshWindowFrame(m_window);

   m_window->Flags = old;


   RastPort *rp = m_window->RPort;
   WORD h = m_window->Height - 1;
   WORD w = m_window->Width - 1;

   //SetRast(rp, 0);
   SetAPen(rp, m_screen->m_drawInfo->dri_Pens[BACKGROUNDPEN]);

   int top = m_border.y + m_titleHeight - 1;
   RectFill(rp, m_border.x-1, top, w-m_border.x+1, top);
   RectFill(rp, m_border.x-1, top, m_border.x-1, h-1);
   RectFill(rp, w-m_border.x, top, w - m_border.x+2, h-2);

   if (m_active)
      SetAPen(rp, m_screen->m_drawInfo->dri_Pens[FILLPEN]);

   RectFill(rp, m_border.x, h-1,  w - m_border.x+2, h - 1);
}

void AQWindow::paintAll(bool trulyAll)
{
   if (!m_window)
      return;

   paintBorder();

   // Finally draw the widget
   Rectangle rectangle = {m_clientPos.x, m_clientPos.y
                       , m_clientPos.x + m_widget->size().x - 1
                       , m_clientPos.y + m_widget->size().y - 1};

   // anything OR full rect => full rect
   if (trulyAll)
      OrRectRegion(m_dirtyRegion, &rectangle);

   paintDirty();
}

void AQWindow::setMouseGrabber(AQWidget *w)
{
   s_mouseGrabWidget = w;
}

void AQWindow::testFrameClick(const IntuiMessage &msg)
{
   if (msg.Code == SELECTDOWN) {
      switch (m_winControl) {
      case Flags(WidgetArea | Hover): // user area
      case WidgetArea:
         break;
      default: // title and size should raise
         WindowToFront(m_window);
         break;
      }
      return;
   }

   m_winControl = WidgetArea; // do this or we will keep size / move
}

void AQWindow::hoverTest(const IntuiMessage &msg, bool forceSet)
{
   if (m_titleHeight == 0) {
      m_winControl = WidgetArea;
      return;
   }
   if (forceSet) {
      WORD x = msg.MouseX;
      WORD y = msg.MouseY;
             
      UWORD winControl = 0x0;
      
      if (x >= 0 && y >= 0 && x < m_window->Width && y < m_window->Height) {
         // first check for size controls
         if ((m_flags & LeftSizer) && x <= m_border.x)
            winControl = LeftSizer;
         else {
            if ((m_flags & RightSizer) && x >= m_window->Width - m_border.x)
               winControl = RightSizer;
         }
       
        if ((m_flags & BottomSizer) && y >= m_window->Height - m_border.y)
           winControl |= BottomSizer;
       
        if (!winControl && y < m_border.y + m_titleHeight) {          
           if ((m_flags & TitleBar))
              winControl = TitleBar;
        }
     }    
     if (forceSet) {
        m_winControl = Flags(winControl | Hover); // mark that we hover it
     } else if (Flags(m_winControl & ~Hover) == winControl) {
        m_winControl = Flags(m_winControl | Hover); // we hover the original again
     } else
        m_winControl = Flags(m_winControl & ~Hover); // we no longer hover the original
   }
   else if (m_winControl == Flags(Hover | TitleBar)) {
      if (msg.MouseX != m_x || msg.MouseY != m_y)
         ChangeWindowBox(m_window, msg.MouseX + m_window->LeftEdge - m_x,
                                   msg.MouseY + m_window->TopEdge - m_y,
                                   m_window->Width, m_window->Height);
   } else {
      WORD dw = 0;
      WORD dh = 0;
      WORD dx = 0;
      WORD dy = 0;
      AQPoint calculatedAllowance(m_widget->size());
      calculatedAllowance -= m_widget->minimumSize();

      if (m_winControl & LeftSizer) {
         dx = msg.MouseX - m_x;
         if(calculatedAllowance.x - dx < 0)
            dx = calculatedAllowance.x;
      }
      if (m_winControl & RightSizer) {
         dw = msg.MouseX - m_x;
         if(calculatedAllowance.x + dw < 0)
            dw = - calculatedAllowance.x;
         m_x += dw;         
      }
      if (m_winControl & BottomSizer) {
         dh = msg.MouseY - m_y;
         if(calculatedAllowance.y + dh < 0)
            dh = - calculatedAllowance.y;
         m_y += dh;
      }
      dw -= dx;
      dh -= dy;
      if (dx || dy || dw | dh )
         ChangeWindowBox(m_window, m_window->LeftEdge + dx,
                                   m_window->TopEdge +dy,
                                   m_window->Width + dw,
                                   m_window->Height + dh);
   }
}

