#include "AQTextEdit.h"

#include <graphics/gfxbase.h>

#ifdef __GNUC__
#include <proto/graphics.h>
#include <proto/layers.h>
#else
#include <pragma/graphics_lib.h>
#include <pragma/layers_lib.h>
#endif

#include <AQTextDoc.h>
#include <AQScrollBar.h>
#include <AQMenu.h>
#include <AQApplication.h>
#include <AQClipboard.h>
#include <AQAction.h>

#include <stdio.h>
#include <string.h>

AQTextEdit::AQTextEdit(AQWidget *parent, bool withScroll)
   : AQWidget(parent)
   , m_doc(new AQTextDoc(this))
   , m_cursor(new AQTextCursor(*m_doc))
   , m_scrollBar(withScroll ? new AQScrollBar(false, this) : 0)
{
   setBgPen(2);
   setExpanding(true, true);
   setMinimumSize(AQPoint(9, 10));
   setPreferredSize(AQPoint(400, 100));
   setFocus();
   setContextMenuPolicy(2); // we want right mouse button event

   m_cutAction = new AQAction(this);
   m_cutAction->setText("Cut");
   m_cutAction->setShortcut("Amiga+X");
   Connect<AQTextEdit>(m_cutAction, "triggered", this, &AQTextEdit::cut);
   addAction(m_cutAction);
   m_copyAction = new AQAction(this);
   m_copyAction->setText("Copy");
   m_copyAction->setShortcut("Amiga+C");
   Connect<AQTextEdit>(m_copyAction, "triggered", this, &AQTextEdit::copy);
   addAction(m_copyAction);
   m_pasteAction = new AQAction(this);
   m_pasteAction->setText("Paste");
   m_pasteAction->setShortcut("Amiga+V");
   Connect<AQTextEdit>(m_pasteAction, "triggered", this, &AQTextEdit::paste);
   addAction(m_pasteAction);

   if (m_scrollBar) {
      Connect<AQTextEdit>(m_scrollBar, "valueChanged", this, &AQTextEdit::scrollUpdate);
      m_scrollBar->setSingleStep(7);
      m_scrollBar->setWheelStep(3*7);
   }
}

AQTextEdit::~AQTextEdit()
{
}

AQTextDoc *AQTextEdit::document() const
{
   return m_doc;
}

void AQTextEdit::setDocument(AQTextDoc *doc, AQTextCursor *cursor)
{
   m_doc = doc;
   m_cursor = cursor;
   if (m_scrollBar)
      m_scrollBar->setMaximum(m_doc->height());
   update();
}

AQScrollBar *AQTextEdit::verticalScrollBar() const
{
   return m_scrollBar;
}

void AQTextEdit::scrollUpdate(int v)
{
   update();
}

void AQTextEdit::cut()
{
   aqApp->clipboard()->setText(m_cursor->selectedText());
   m_cursor->deleteSelection();
   update();
}

void AQTextEdit::copy()
{
   aqApp->clipboard()->setText(m_cursor->selectedText());
}

void AQTextEdit::paste()
{
   m_cursor->deleteSelection();
   m_cursor->insertText(aqApp->clipboard()->text());
   update();
}

void AQTextEdit::ensureCursorVisible()
{
   if (!m_scrollBar)
      return;

   LONG y = m_doc->blockNumber(m_cursor->position()) * m_doc->lineHeight();

   int v = m_scrollBar->value();

   if (v > y)
      v = y;
   y += m_doc->lineHeight();
   y -= size().y - 4;
   if (v < y)
      v = y;

   m_scrollBar->setValue(v);
}

void AQTextEdit::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define SHADOW 1

   LONG bottom = s.y - 1;
   LONG right = s.x - 1;

   if (m_scrollBar)
    right -= m_scrollBar->size().x + 2;

   // norm backgorund for the scrollbar
   SetAPen(rp, 0);
   RectFill(rp, right + 1, 0, s.x, s.y);

   AQRect clipRect(rect);
   clipRect.bottomRight.x = aqMin(right - 2, rect.bottomRight.x);

   SetAPen(rp, SHADOW);
   Move(rp, right, 0);
   Draw(rp, 0, 0);
   Draw(rp, 0, bottom);

   pushClipRect(rp, clipRect);

   AQPoint docOffset(0,0);
   if (m_scrollBar)
      docOffset.y = (m_scrollBar->value() / m_doc->lineHeight()) * m_doc->lineHeight();

   ScrollLayer(0, rp->Layer, -2, -2); //offset painting in widget

   m_doc->render(rp, docOffset, AQPoint(right-4, bottom-4));

   if (hasFocus() && !m_cursor->hasSelection()) {
      SetAPen(rp, 3);
      LONG x = m_cursor->positionInBlock() * rp->TxWidth-1;
      LONG y = m_doc->blockNumber(m_cursor->position()) * m_doc->lineHeight();
      x -= docOffset.x;
      y -= docOffset.y;
      RectFill(rp, x, y, x + 1, y + rp->TxHeight);
   }

   ScrollLayer(0, rp->Layer, 2, 2); // restore offset
   restoreClipping(rp);
}

bool AQTextEdit::wheelEvent(bool up)
{
   if (m_scrollBar)
      m_scrollBar->wheelEvent(up);

   return false;
}

bool AQTextEdit::keyEvent(const IntuiMessage &msg)
{
   if(msg.Class ==IDCMP_RAWKEY) {
      switch (msg.Code) {
      case Key_Left:
         m_cursor->movePrevChar(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_Right:
         m_cursor->moveNextChar(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_Up:
         m_cursor->movePrevLine(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_Down:
         m_cursor->moveNextLine(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_Home:
      case Key_Numeric7:
         m_cursor->moveStartOfLine(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_End:
      case Key_Numeric1:
         m_cursor->moveEndOfLine(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_PageUp:
      case Key_Numeric9:
         m_cursor->movePageUp((size().y -4) / 7, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_PageDown:
      case Key_Numeric3:
         m_cursor->movePageDown((size().y -4) / 7, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      default:
         return 0;
      }
   } else { // IDMCP_VANILLAKEY
      if (msg.Code == '\r')
         m_cursor->insertBlock();
      else if (msg.Code == 0x08) // backspace
         m_cursor->deleteLeft();
      else if (msg.Code == 0x7F) // delete
         m_cursor->deleteRight();
      else if (msg.Code >31){
         char cstr[2];
         cstr[0] = msg.Code;
         cstr[1] = 0;
         AQString str(cstr);
         m_cursor->insertText(str);
      }
      if (m_scrollBar)
         m_scrollBar->setMaximum(m_doc->height());
   }
   update();
   ensureCursorVisible();
   return true;
}

void AQTextEdit::focusInEvent()
{
   update();
}

void AQTextEdit::focusOutEvent()
{
   update();
}

bool AQTextEdit::mouseDoubleClickEvent(const IntuiMessage &msg)
{
   if (msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT))
      return true;

   m_cursor->select(AQTextCursor::WordUnderCursor);

   update();
   return true;
}

bool AQTextEdit::mousePressEvent(const IntuiMessage &msg)
{
   setFocus();

   AQPoint clickPoint(msg.MouseX - 2, msg.MouseY - 2);

   if (m_scrollBar)
      clickPoint.y += (m_scrollBar->value() / m_doc->lineHeight()) * m_doc->lineHeight();

   int pos = m_doc->positionOfPoint(clickPoint);
   

   if (msg.Code == MENUDOWN) {
      if (pos < m_cursor->selectionStart() || pos > m_cursor->selectionEnd())
         m_cursor->setPosition(pos, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));

      AQMenu *menu = new AQMenu();
      menu->addItem(m_cutAction->text());
      menu->addItem(m_copyAction->text());
      menu->addItem(m_pasteAction->text());
      menu->exec(mapToGlobal(AQPoint(msg.MouseX, msg.MouseY)));
   }
   else
      m_cursor->setPosition(pos, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));

   update();
   return true;
}

bool AQTextEdit::mouseMoveEvent(const IntuiMessage &msg)
{
   AQPoint clickPoint(msg.MouseX - 2, msg.MouseY - 2);
   if (m_scrollBar)
      clickPoint.y += (m_scrollBar->value() / m_doc->lineHeight()) * m_doc->lineHeight();

   int pos = m_doc->positionOfPoint(clickPoint);
   
   if (pos != m_cursor->position())
      update();

   m_cursor->setPosition(pos, true);
   return true;
}

bool AQTextEdit::mouseReleaseEvent(const IntuiMessage &msg)
{
   return true;
}

void AQTextEdit::resizeEvent(const AQPoint &oldSize)
{
   if (m_scrollBar) {
      m_scrollBar->setPageStep(size().y - 4);
      m_scrollBar->setSize(AQPoint(m_scrollBar->preferredSize().x, size().y));
      m_scrollBar->setPos(AQPoint(size().x - m_scrollBar->size().x, 0));
   }
}
