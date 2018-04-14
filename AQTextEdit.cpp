#include "AQTextEdit.h"

#include <graphics/gfxbase.h>

#include <proto/graphics.h>
#include <proto/layers.h>

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
   , m_auxColumnWidth(0)
{
   setBgPen(2);
   setExpanding(true, true);
   setMinimumSize(AQPoint(9, 10)
       + (withScroll ? m_scrollBar->minimumSize() : AQPoint()));
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
   Connect<AQAction>(aqApp->clipboard(), "changed", m_pasteAction, &AQAction::setEnabled);
   addAction(m_pasteAction);

   if (m_scrollBar) {
      Connect<AQTextEdit>(m_scrollBar, "valueChanged", this, &AQTextEdit::scrollUpdate);
      m_scrollBar->setSingleStep(font()->tf_YSize);
      m_scrollBar->setWheelStep(3*font()->tf_YSize);
   }

   Connect<AQTextEdit>(m_doc, "documentChanged", this, &AQTextEdit::onDocumentChanged);
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
   if (m_doc)
      Disconnect<AQTextEdit>(m_doc, "documentChanged", this, &AQTextEdit::onDocumentChanged);
   m_doc = doc;
   m_cursor = cursor;
   if (m_doc) {
      if (m_scrollBar)
         m_scrollBar->setMaximum(m_doc->height());
      Connect<AQTextEdit>(m_doc, "documentChanged", this, &AQTextEdit::onDocumentChanged);
   }
   update();
}

AQTextCursor *AQTextEdit::mainCursor() const
{
   return m_cursor;
}

AQScrollBar *AQTextEdit::verticalScrollBar() const
{
   return m_scrollBar;
}

void AQTextEdit::scrollUpdate(int delta)
{
   if (!m_doc)
      return;

   AQRect area(2, 2, size().x - 4, size().y - 4);

   if (!m_scrollBar) {
      update(area);
      return;
   }

   area.bottomRight.x -= m_scrollBar->size().x + 2;

   int offBefore = ((m_scrollBar->value() - delta)/ m_doc->lineHeight()) * m_doc->lineHeight();
   int offAfter = (m_scrollBar->value() / m_doc->lineHeight()) * m_doc->lineHeight();

   int dy = offAfter - offBefore;

   scroll(AQPoint(0, dy), area); // also serves pending updates

   m_docOffset.y = offAfter; // now it is safe

   if (dy < 0)
      update(AQRect(AQPoint(2, area.bottomRight.y-10), area.bottomRight));
}

void AQTextEdit::cut()
{
   if (!m_doc)
      return;

   aqApp->clipboard()->setText(m_cursor->selectedText());
   m_cursor->deleteSelection();
   update();
}

void AQTextEdit::copy()
{
   if (!m_doc)
      return;

   aqApp->clipboard()->setText(m_cursor->selectedText());
}

void AQTextEdit::paste()
{
   if (!m_doc)
      return;

   m_cursor->deleteSelection();
   m_cursor->insertText(aqApp->clipboard()->text());
   update();
}

void AQTextEdit::setAuxColumnWidth(int w)
{
   m_auxColumnWidth = w;
   update();
}

void AQTextEdit::ensureCursorVisible(EnsureType type)
{
   if (!m_doc)
      return;

   AQPoint p = m_doc->pointOfPosition(m_cursor->position());

   int v = m_docOffset.x;


   // First we do horizontal against left side
   int width = size().x - 4 - m_auxColumnWidth;
   if (m_scrollBar)
      width -= m_scrollBar->size().x + 2;

   if (v > p.x)
      v = p.x - width/4;
   if (v < 0)
      v = 0;

   // Next we do horizontal against right side
   p.x += 2;
   p.x -= width;

   if (v < p.x)
      v = p.x + width/4;

   if (v != m_docOffset.x) {
      m_docOffset.x = v;
      update();
   }

   // Next we do vertical against top
   if (!m_scrollBar)
      return;

   p.y -= m_doc->defaultFont()->tf_Baseline;
   v = m_docOffset.y;

   if (v > p.y)
      v = p.y;

   // Next we do verticalagainstbottom
   p.y += m_doc->defaultFont()->tf_Baseline;
   p.y -= size().y - 4 - m_doc->lineHeight();
   if (v < p.y)
      v = p.y;

   m_scrollBar->setValue(v);
}

void AQTextEdit::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define SHADOW 1

   LONG bottom = s.y - 1;
   LONG right = s.x - 1 ;

   if (m_scrollBar)
      right -= m_scrollBar->size().x + 2;

   AQRect clipRect(rect);
   clipRect.bottomRight.x = aqMin(right - 2, rect.bottomRight.x);
   clipRect.topLeft.x = aqMax(2 + m_auxColumnWidth, rect.topLeft.x);

   if (rect.bottomRight.x > right) {
      SetAPen(rp, 0);
      RectFill(rp, right, 0, right+2, bottom);
   }

   if (m_auxColumnWidth && rect.topLeft.x < 2 + m_auxColumnWidth) {
      SetAPen(rp, 0);
      RectFill(rp, 1, 0, m_auxColumnWidth, bottom-1);
      SetAPen(rp, 1);
      SetFont(rp, m_doc->defaultFont());
      int b = m_docOffset.y / rp->TxHeight + 1; // +1 to show 1 based
      int y = rp->TxBaseline + 2;
      while (y < bottom + rp->TxBaseline - rp->TxHeight) {
         AQString t = AQString::number(b);
         Move(rp,m_auxColumnWidth - TextLength(rp, t, t.size()), y);
         Text(rp, t, t.size());
         ++b;
         y += rp->TxHeight;
      }

   }

   SetAPen(rp, SHADOW);
   Move(rp, right, 0);
   Draw(rp, 0, 0);
   Draw(rp, 0, bottom);

   if (!m_doc)
      return;


   pushClipRect(rp, clipRect);

   ScrollLayer(0, rp->Layer, -2 - m_auxColumnWidth, -2); //offset painting in widget

   m_doc->render(rp, m_docOffset, AQPoint(right-4-m_auxColumnWidth, bottom-4));

   if (hasFocus())
      m_doc->renderCursor(rp, m_cursor, m_docOffset, AQPoint(right-4, bottom-4));

   ScrollLayer(0, rp->Layer, 2 + m_auxColumnWidth, 2); // restore offset
   restoreClipping(rp);
}

bool AQTextEdit::wheelEvent(bool up)
{
   if (m_scrollBar)
      m_scrollBar->wheelEvent(up);

   return true;
}

AQRect AQTextEdit::cursorRect(bool fullLineWidth) const
{   
   AQPoint point = m_doc->pointOfPosition(m_cursor->selectionStart());
   point.y -= m_doc->defaultFont()->tf_Baseline;
   point -= m_docOffset;
   point += AQPoint(2 + m_auxColumnWidth, 2); // we have a frame around the actual doc
   AQPoint bottom = point;

   if (fullLineWidth || m_cursor->hasSelection()) {
      bottom = m_doc->pointOfPosition(m_cursor->selectionEnd());
      bottom.y -= m_doc->defaultFont()->tf_Baseline;
      bottom.y += m_doc->defaultFont()->tf_YSize + 1;
      bottom -= m_docOffset;
      bottom += AQPoint(2 + m_auxColumnWidth, 2); // we have a frame around the actual doc

      point.x = 2;
      bottom.x = size().x - 4;
      if (m_scrollBar)
         bottom.x -= m_scrollBar->size().x + 2;
   } else
      bottom += AQPoint(2, m_doc->lineHeight() + 1);


   return AQRect(point, bottom);
}

bool AQTextEdit::keyEvent(const IntuiMessage &msg)
{
   if (!m_doc)
      return false;

   AQRect cursorPreRect = cursorRect();
   bool hadSelection = m_cursor->hasSelection();

   if (msg.Class ==IDCMP_RAWKEY) {
      switch (msg.Code) {
      case Key_Left:
         if (msg.Qualifier & IEQUALIFIER_CONTROL)
            m_cursor->movePrevWord(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         else
            m_cursor->movePrevChar(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_Right:
         if (msg.Qualifier & IEQUALIFIER_CONTROL)
            m_cursor->moveNextWord(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         else
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
         m_cursor->movePageUp((size().y -4) / m_doc->lineHeight()-1, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      case Key_PageDown:
      case Key_Numeric3:
         m_cursor->movePageDown((size().y -4) / m_doc->lineHeight()-1, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));
         break;
      default:
         return false;
      }
   } else { // IDMCP_VANILLAKEY
      if (msg.Code == '\r') {
         m_cursor->insertBlock();
         update();
      } else if (msg.Code == 0x08) { // backspace
         if (hadSelection || m_cursor->positionInBlock() == 0)
            update();
         else
            update(cursorRect(true));
         m_cursor->deleteLeft();
      } else if (msg.Code == 0x7F) { // delete
         m_cursor->deleteRight();
         if (hadSelection || m_cursor->positionInBlock() == 0)
            update();
         else
            update(cursorRect(true));
      } else if (msg.Code >31 || msg.Code == '\t'){
         char cstr[2];
         cstr[0] = msg.Code;
         cstr[1] = 0;
         AQString str(cstr);
         m_cursor->insertText(str);
         if (hadSelection)
            update();
         else
            update(cursorRect(true));
      }
   }

   update(cursorPreRect);
   update(cursorRect());
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
   if (!m_doc)
      return false;

   if (msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT))
      return true;

   if (msg.MouseX - 2 - m_auxColumnWidth < -1)
      return true;

   m_cursor->select(AQTextCursor::WordUnderCursor);

   update();
   return true;
}

bool AQTextEdit::mousePressEvent(const IntuiMessage &msg)
{
   if (!m_doc)
      return false;

   setFocus();

   AQPoint clickPoint(msg.MouseX - 2 - m_auxColumnWidth, msg.MouseY - 2);

   if (clickPoint.x < -1)
      return true; // 

   clickPoint += m_docOffset;

   int pos = m_doc->positionOfPoint(clickPoint);   

   if (msg.Code == MENUDOWN) {
      if (pos < m_cursor->selectionStart() || pos > m_cursor->selectionEnd())
         m_cursor->setPosition(pos, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));

      AQMenu *menu = new AQMenu();
      menu->addAction(m_cutAction);
      menu->addAction(m_copyAction);
      menu->addAction(m_pasteAction);
      menu->exec(mapToGlobal(AQPoint(msg.MouseX, msg.MouseY)));
   }
   else
      m_cursor->setPosition(pos, msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT));

   update();
   return true;
}

bool AQTextEdit::mouseMoveEvent(const IntuiMessage &msg)
{
   if (!m_doc)
      return false;

   AQPoint clickPoint(msg.MouseX - 2 - m_auxColumnWidth, msg.MouseY - 2);

   clickPoint += m_docOffset;

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

void AQTextEdit::onDocumentChanged(AQObject *obj)
{
   if (!m_doc)
      return;

   if (m_scrollBar)
      m_scrollBar->setMaximum(m_doc->height());
}
