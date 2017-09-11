#include "AQWindow.h"

#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <intuition/intuition.h>
#include <libraries/gadtools.h>
#include <graphics/gfxbase.h>
#include <devices/inputevent.h>

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/layers.h>

#include <AQApplication.h>
#include <AQWidget.h>


AQWindow::AQWindow(AQWidget *widget, int modality, UWORD flags)
   : m_window(nullptr)
   , m_drawInfo(nullptr)
   , m_visualInfo(nullptr)
   , m_pubScreen(nullptr)
   , m_active(true)
   , m_winControl(WidgetArea)
   , m_flags(Flags(flags & Normal))
   , m_widget(widget)
   , m_buttonDownWidget(nullptr)
   , m_dirtyRegion(NewRegion())
{
   m_pubScreen = LockPubScreen(NULL);
   if (m_pubScreen) {
      m_visualInfo = GetVisualInfo(m_pubScreen, TAG_DONE);
      m_drawInfo = GetScreenDrawInfo(m_pubScreen);
   }

   if (m_flags & (TitleBar | CloseButton | MinimizeButton | MaximizeButton)) {
      m_border = AQPoint(4, 3);
      m_titleHeight = m_drawInfo->dri_Font->tf_YSize;
   } else  {
      m_border = AQPoint(0,0);
      m_titleHeight = 0;
   }

   AQPoint prefSize = widget->preferredSize() + m_border + m_border;

   m_window = OpenWindowTags(NULL,
   WA_Width, prefSize.x,
   WA_Height, prefSize.y,
   WA_Left, widget->pos().x,
   WA_Top, widget->pos().y,
   WA_Activate, TRUE,
   WA_Flags, WFLG_BORDERLESS,
   WA_SimpleRefresh, TRUE,
   WA_MinWidth, 5,
   WA_MinHeight, 5,
   WA_ReportMouse, TRUE,
   WA_NewLookMenus, TRUE,
   TAG_DONE, 0L);

   m_window->UserPort = aqApp->userPort();

   ModifyIDCMP(m_window, CLOSEWINDOW | MENUPICK | IDCMP_VANILLAKEY | IDCMP_RAWKEY
    | IDCMP_REFRESHWINDOW | IDCMP_MOUSEBUTTONS | IDCMP_MOUSEMOVE
    | IDCMP_ACTIVEWINDOW | IDCMP_INACTIVEWINDOW
       | IDCMP_INTUITICKS | IDCMP_CHANGEWINDOW);


//   m_widget->m_palette = new AQPalette();
//   m_widget->m_palette->ShadowPen = m_drawInfo->dri_Pens[SHADOWPEN]);
//   m_widget->m_palette->ShinePen = m_drawInfo->dri_Pens[SHINEPEN]);
//   m_widget->m_palette->DarkPen = m_drawInfo->dri_Pens[SHADOWPEN]);
//   m_widget->m_palette->BasePen = m_drawInfo->dri_Pens[SHINEPEN]);
//   m_widget->m_palette->TextPen = m_drawInfo->dri_Pens[TEXTPEN]);


   if(modality > 0)
      aqApp->registerModalWindow(this);
   else
      aqApp->registerWindow(this);

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

   if (m_visualInfo) {
      FreeVisualInfo(m_visualInfo);
   }
   
   if (m_drawInfo) {
      FreeScreenDrawInfo(m_pubScreen, m_drawInfo);
   }

   if (m_pubScreen) {
      UnlockPubScreen(NULL, m_pubScreen);
   }
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
      BeginRefresh(m_window);
      paintAll();
      EndRefresh(m_window, TRUE);
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
         while (receiver && !receiver->event(msg))
            receiver = receiver->parent();
      }
      break;

   case IDCMP_MOUSEBUTTONS: {
      AQWidget *receiver = pickMouseReceiver(msg);

      if (receiver) {
         while (receiver && !receiver->event(msg)) {
            if (receiver->parent()) {
               msg.MouseX += receiver->pos().x;
               msg.MouseY += receiver->pos().y;
            }
            receiver = receiver->parent();
         }
         // receiver is valid if we broke loop because event() gave true
         if (msg.Code == SELECTDOWN || msg.Code == MIDDLEDOWN || msg.Code == MENUUP)
            m_buttonDownWidget = receiver;
         else
            m_buttonDownWidget = nullptr;
      } else
         testFrameClick(msg);

      break;
   }
   case IDCMP_MOUSEMOVE: {
      AQWidget *receiver = pickMouseReceiver(msg);

      if (receiver) {
         while (receiver && !receiver->event(msg)) {
            if (receiver->parent()) {
               msg.MouseX += receiver->pos().x;
               msg.MouseY += receiver->pos().y;
            }
            receiver = receiver->parent();
         }
      }
      break;
   }
   case IDCMP_INTUITICKS:
      if (aqApp->isWindowBlocked(this))
         return;

      if (m_buttonDownWidget == nullptr && s_mouseGrabWidget == nullptr) {
         hoverTest(msg, !msg.Qualifier & IEQUALIFIER_LEFTBUTTON);

         if (m_winControl == Hover || m_winControl == 0) {
            msg.MouseX -= m_clientPos.x;
            msg.MouseY -= m_clientPos.y;

            aqApp->setHoveredWidget(m_widget->widgetAt(msg.MouseX, msg.MouseY));
            if (aqApp->hoveredWidget() && aqApp->hoveredWidget()->contextMenuPolicy() == 2)
               m_window->Flags |= WFLG_RMBTRAP;
            else
               m_window->Flags &= ~WFLG_RMBTRAP;
         }
      }

      paintDirty();
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
      m_active = true;
      m_widget->changeEvent(AQWidget::Activation);
      paintAll();
      break;
   case IDCMP_INACTIVEWINDOW:
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
   AQRect clip(rect.intersected(w->geometry()));

   if (!clip.isValid()) // nothing to paint for this widget within cliprect
      return;

   AQPoint pos = w->pos();

   // offset painting by calling ScrollLayer (misnomer but it works as an offset for Gfx)
   ScrollLayer(0, rp->Layer, -pos.x, -pos.y);
   clip.translate(-pos);    // make clip as seen by the widget

   if (w->bgPen() >= 0) {
      SetAPen(rp, w->bgPen());
      RectFill(rp, clip.topLeft.x, clip.topLeft.y, clip.bottomRight.x, clip.bottomRight.y);
      winBg = -1;
   }

   if (winBg >= 0) { // we need to check if we should handle inherited bg
      int effectiveBg = winBg;
      if (w->bgPen() < 0) {//== -2) {
         // the widget doesn't need to draw so let's see if a child can
         // handle all of the clip so we can defer
         for (int i = 0; i < w->children().size(); ++i) {
            AQWidget *child = w->children()[i];

            if (child->geometry().contains(clip)) {
               effectiveBg = -1;  // yes child can handle all of the clip, so defer
               break;
            }
         }
      }

      if (effectiveBg >= 0) {
         SetAPen(rp, effectiveBg);
         RectFill(rp, clip.topLeft.x, clip.topLeft.y, clip.bottomRight.x, clip.bottomRight.y);
         winBg = -1; // mark we have handled the window background
      }
   }
 
   w->paintEvent(rp, clip);

   // now paint children
   for (int i = 0; i < w->children().size(); ++i) {
      AQWidget *child = w->children()[i];
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

      int winBg = m_active ? m_drawInfo->dri_Pens[FILLPEN] : 0;
      winBg = 0;
      paintWidget(m_widget, rp, rect, winBg);

      reg = reg->Next;
   }
   ClearRegion(m_dirtyRegion);
   ScrollLayer(0, rp->Layer, -pos.x, -pos.y);
}

void AQWindow::markDirty(const AQRect &rect)
{
   Rectangle rectangle = {m_clientPos.x + rect.topLeft.x,
                          m_clientPos.x + rect.topLeft.y,
                          m_clientPos.x + rect.bottomRight.x,
                          m_clientPos.y + rect.bottomRight.y};

   OrRectRegion(m_dirtyRegion, &rectangle);
}

void AQWindow::paintAll()
{
   if (!m_window)
      return;

   RastPort *rp = m_window->RPort;
   WORD h = m_window->Height - 1;
   WORD w = m_window->Width - 1;

   if (m_active) {
      SetRast(rp, 0);//m_drawInfo->dri_Pens[FILLPEN]);
      SetAPen(rp, m_drawInfo->dri_Pens[FILLPEN]);
      SetBPen(rp, m_drawInfo->dri_Pens[FILLPEN]);
   } else {
      SetRast(rp, 0);
      SetAPen(rp, m_drawInfo->dri_Pens[BACKGROUNDPEN]);
      SetBPen(rp, m_drawInfo->dri_Pens[BACKGROUNDPEN]);
   }

   RectFill(rp, 2, 1, w - 1, m_border.y + m_titleHeight - 2);
   RectFill(rp, 0, 1, 2, h-1);
   RectFill(rp, w-2, 1, w, h-1);
   RectFill(rp, 1, h, w-1, h);

   SetAPen(rp, m_drawInfo->dri_Pens[SHADOWPEN]);
   Move(rp, w-1, 1); // inner line right side
   Draw(rp, w-1, h - 1);
   Draw(rp, 1, h - 1); // bottom line
   SetAPen(rp, m_drawInfo->dri_Pens[SHINEPEN]);
   Move(rp, 1, h - 1);
   Draw(rp, 1, 0); // inner line left side
   Draw(rp, w - 1, 0); // top line 
   
   // Draw the title
   if (m_flags & TitleBar) {
      SetAPen(rp, m_drawInfo->dri_Pens[TEXTPEN]);
      Move(rp, m_border.x + 10 + m_window->IFont->tf_XSize
                             , m_border.y - 2 + m_window->IFont->tf_Baseline);	
      Text(rp, m_widget->m_title, m_widget->m_title.size());
   }
      
   // Draw the close button
   if (m_flags & CloseButton) {
      SetAPen(rp, m_drawInfo->dri_Pens[SHADOWPEN]);
      RectFill(rp, m_border.x + 1 , m_border.y, m_border.x + 5, m_border.y + 3);
      SetAPen(rp, m_drawInfo->dri_Pens[SHINEPEN]);
      RectFill(rp, m_border.x + 2, m_border.y + 1, m_border.x + 4, m_border.y + 2);
   }
 
   // Finally draw the widget
   Rectangle rectangle = {m_clientPos.x, m_clientPos.y
                       , m_clientPos.x + m_widget->size().x - 1
                       , m_clientPos.y + m_widget->size().y - 1};

   // anything OR full rect => full rect
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
      case Flags(CloseButton | Hover): // close
      case CloseButton:
      case Flags(MaximizeButton | Hover): // maximize
      case MaximizeButton:
      case Flags(MinimizeButton | Hover): // minimize
      case MinimizeButton:
         break;
      default: // title and size should raise
         WindowToFront(m_window);
         break;
      }
   }

   if (msg.Code == SELECTUP) {
      switch (m_winControl & (CloseButton | MaximizeButton | MinimizeButton)) {
      case CloseButton:
         m_widget->closeEvent();
         break;
      case MaximizeButton:
         break;
      case MinimizeButton:
         break;
      }
      m_winControl = WidgetArea; // do this or we will keep size / move
   }
}

void AQWindow::hoverTest(const IntuiMessage &msg, bool forceSet)
{
   if (m_titleHeight == 0) {
      m_winControl = WidgetArea;
      return;
   }
   if (forceSet || (m_winControl & (CloseButton|MaximizeButton|MinimizeButton))) {
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
       
        if ((m_flags & TopSizer) && y < m_border.y - 1)
           winControl |= TopSizer;
        else if ((m_flags & BottomSizer) && y >= m_window->Height - m_border.y)
           winControl |= BottomSizer;
       
        if (!winControl && y < m_border.y + m_titleHeight) {          
           if ((m_flags & CloseButton) && x < m_border.x + 10)
              winControl = CloseButton; // close
           else {
              x += m_border.x - m_window->Width; // rel to inner right
              if (x > - 20) //but top right
                 winControl = MaximizeButton;
              else if (x > - 40) // but 2 top right
                 winControl = MinimizeButton;
              else if ((m_flags & TitleBar))
                 winControl = TitleBar;
           }
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
      if (m_winControl & TopSizer) {
         dy = msg.MouseY - m_y;
         if(calculatedAllowance.y - dy < 0)
            dy = calculatedAllowance.y;
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

