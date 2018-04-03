#include "AQWidget.h"

#include <algorithm>

#include <graphics/gfxbase.h>

#include <proto/graphics.h>
#include <proto/layers.h>
#include <proto/intuition.h>

#include <AQLayout.h>
#include <AQApplication.h>
#include <AQWindow.h>
#include <AQAction.h>
#include <AQMenu.h>

AQWidget::AQWidget(AQWidget *parent)
   : AQObject(parent)
   , m_parent(nullptr)
   , m_pos(0, 0)
   , m_size(0, 0)
   , m_maximumSize(1000, 1000)
   , m_preferredSize(50, 25)
   , m_bgPen(-2) // no bg and no painting of our own
   , m_expandX(true)
   , m_expandY(false)
   , m_enabled(true)
   , m_visible(true)
   , m_layout(nullptr)
   , m_oldClip(nullptr)
   , m_window(nullptr)
   , m_contextMenuPolicy(0)
   , m_windowModality(0)
   , m_windowFlags(AQWindow::Normal)
   , m_firstClickSeconds(0)
   , m_firstClickMicros(0)
   , m_menu(nullptr)
{
   setParent(parent);
}

AQWidget::~AQWidget()
{
   if (aqApp->m_focusWidget == this)
      aqApp->m_focusWidget = nullptr;

   if (aqApp->m_hoveredWidget == this)
      aqApp->m_hoveredWidget = nullptr;

   if (!m_parent)
      delete m_window;

   if (m_layout)
      delete m_layout;
}

void AQWidget::setParent(AQWidget *pw)
{
   if (m_parent)      
      m_parent->m_children.erase(std::remove(m_parent->m_children.begin(), m_parent->m_children.end(), this), m_parent->m_children.end());

   if (pw)
      pw->m_children.push_back(this);

   m_parent = pw;

   AQObject::setParent(pw);
}

void AQWidget::show()
{
   setVisible(true);
}

void AQWidget::hide()
{
   setVisible(false);
}

bool AQWidget::isVisible() const
{
   if (!m_visible)
      return  false;

   if (isTopLevel())
      return m_window;
   else
      return m_parent->isVisible();
}

void AQWidget::setVisible(bool visible)
{
   if (m_visible != visible) {
      m_visible = visible;
      updateGeometry();
   } else
      m_visible = visible;

   if (visible) {
      if (isTopLevel() && m_window == nullptr)
          m_window = new AQWindow(this, m_windowModality, m_windowFlags);
   } else {
      if (m_window) {
          m_window->hide();
          delete m_window;
          m_window = nullptr;
      }
   }
}

bool AQWidget::isTopLevel() const
{
   return (!m_parent) || (m_windowFlags & AQWindow::Popup);
}


void AQWidget::setMenu(AQMenu *menu)
{
   m_menu = menu;
   m_menu->setMenubarMode(true);
   m_menu->setParent(this);
}


AQString AQWidget::toolTip() const
{
   return m_toolTip;
}

void AQWidget::setToolTip(const AQString &text)
{
   m_toolTip = text;
}


void AQWidget::setWindowTitle(const AQString &title)
{
   m_title = title;
}

void AQWidget::setWindowModality(int modal)
{
   m_windowModality = modal;
}

void AQWidget::setWindowFlags(UWORD flags)
{
   m_windowFlags = flags;
}

TextFont *AQWidget::font() const
{
   return aqApp->screen(this)->font();
}

void AQWidget::grabMouse()
{
   AQWindow::setMouseGrabber(this);
}

void AQWidget::releaseMouse()
{
   AQWindow::setMouseGrabber(nullptr);
}

void AQWidget::setBgPen(int p)
{
   m_bgPen = p;
}

int AQWidget::bgPen() const
{
   return m_bgPen;
}

AQPoint AQWidget::size() const
{
   return m_size;
}

void AQWidget::setSize(const AQPoint &s)
{
   AQPoint oldSize(m_size);

   bool diff = (m_size == s);

   m_size = s;

   if (m_layout)
      m_layout->layout(s);

   if (diff)
      return;

   resizeEvent(oldSize);
}

AQPoint AQWidget::pos() const
{
   return m_pos;
}

void AQWidget::setPos(const AQPoint &pos)
{
   if (m_window)
      m_window->setPos(pos);

   m_pos = pos;
}

AQRect AQWidget::geometry() const
{
   if (m_parent == nullptr)
      ;
   AQRect r(m_pos.x, m_pos.y, m_size.x, m_size.y);
   return r;
}

AQPoint AQWidget::minimumSize() const
{
   if (m_layout && m_minimumSize.x == 0 && m_minimumSize.y == 0)
      return m_layout->minimumSize();

   return m_minimumSize;
}

void AQWidget::setMinimumSize(const AQPoint &s)
{
   m_minimumSize = s;
}

AQPoint AQWidget::preferredSize() const
{
   if (m_layout && m_preferredSize.x == -1 && m_preferredSize.y == -1)
      return m_layout->preferredSize();

   return m_preferredSize;
}

void AQWidget::setPreferredSize(const AQPoint &s)
{
   m_preferredSize = s;
}

bool AQWidget::likeToExpandX() const
{
   if (m_layout)
      return m_layout->likeToExpandX();

   return m_expandX;
}

bool AQWidget::likeToExpandY() const
{
   if (m_layout)
      return m_layout->likeToExpandY();

   return m_expandY;
}

void AQWidget::setExpanding(bool x, bool y)
{
   m_expandX = x;
   m_expandY = y;
}

AQLayout *AQWidget::layout() const
{
   return m_layout;
}

void AQWidget::setLayout(AQLayout *layout)
{
   m_layout = layout;
   if (m_layout) {
      m_preferredSize = AQPoint(-1, -1);
      m_layout->layout(m_size);
      update();
   }
}

void AQWidget::updateGeometry()
{
   if (!isTopLevel()) {
      AQLayout *pl = m_parent->layout();
      if (pl) {
         pl->invalidate();
         // the parent might need to do the same
         if (!m_parent->isTopLevel() && m_parent->m_parent->m_layout)
            m_parent->updateGeometry();
         else {
            pl->layout(m_parent->size());
            m_parent->update();
         }
      }
   }
}

void AQWidget::setFocus()
{
   aqApp->setFocus(this);
}

bool AQWidget::hasFocus()
{
   return aqApp->focusWidget() == this;
}

void AQWidget::addAction(AQAction *a)
{
   m_actions.push_back(a);
}

int AQWidget::contextMenuPolicy() const
{
   return m_contextMenuPolicy;
}

void AQWidget::setContextMenuPolicy(int val)
{
   m_contextMenuPolicy = val;
}

void AQWidget::update()
{
   AQRect r(0, 0, m_size.x, m_size.y);

   update(r);
}

void AQWidget::update(const AQRect &rect)
{
   if (!m_visible)
      return;

   AQRect r(rect.topLeft + m_pos, rect.bottomRight + m_pos);

   if (!isTopLevel())
      m_parent->update(r);
   else if (m_window)
      m_window->markDirty(rect);
}

void AQWidget::scroll(const AQPoint &delta, const AQRect &rect)
{
   AQRect r(rect.topLeft + m_pos, rect.bottomRight + m_pos);

   if (delta.y < -r.height() || delta.y > r.height()
     || delta.x < -r.width() || delta.x > r.width()) {
      update(rect);
      return;
   }

   if (!isTopLevel())
      m_parent->scroll(delta, r);
   else if (m_window) {
      m_window->scroll(delta, rect);
   }
}

AQWidget *AQWidget::parent()
{
   return m_parent;
}

const vector<class AQWidget *> &AQWidget::children() const
{
   return m_children;
}

void AQWidget::pushClipRect(RastPort *rp, const AQRect &r)
{
   // unfortunately clip regions work in plain bitmap coords
   // so we need to subtract the scroll offset ourselves
   WORD sx = rp->Layer->Scroll_X;
   WORD sy = rp->Layer->Scroll_Y;

   Rectangle rectangle = {r.topLeft.x - sx, r.topLeft.y - sy,
                             r.bottomRight.x - sx, r.bottomRight.y - sy};
   

   Region *newClip = NewRegion();

   if (newClip) {
      // nothing OR rect => rect
      if (! OrRectRegion(newClip, &rectangle))
      {
         DisposeRegion(newClip);
         newClip = nullptr;
      }
   }

   if (rp->Layer->Flags & LAYERREFRESH)
      EndUpdate(rp->Layer, false);

   if (m_oldClip) {
      Region *prePushedClip = InstallClipRegion(rp->Layer, newClip);
      DisposeRegion(prePushedClip);
   } else
      m_oldClip = InstallClipRegion(rp->Layer, newClip);

   if (rp->Layer->Flags & LAYERREFRESH)
      BeginUpdate(rp->Layer);
}

void AQWidget::restoreClipping(RastPort *rp)
{
   if (rp->Layer->Flags & LAYERREFRESH)
      EndUpdate(rp->Layer, false);

   struct Region *pushedRegion = InstallClipRegion(rp->Layer, m_oldClip);

   if (rp->Layer->Flags & LAYERREFRESH)
      BeginUpdate(rp->Layer);

   if (pushedRegion)
      DisposeRegion(pushedRegion);
   m_oldClip = nullptr;
}

AQWidget *AQWidget::widgetAt(WORD &x, WORD &y)
{
   AQPoint p(x, y);

   for (int i = 0; i < m_children.size(); ++i) {
      AQWidget *child = m_children[i];
      if (child->isTopLevel())
         continue;
      if (child->m_visible && child->geometry().contains(p)) {
         x -= child->pos().x;
         y -= child->pos().y;

         return child->widgetAt(x, y);
      }
   }
   return this;
}

AQPoint AQWidget::mapToGlobal(const AQPoint &p) const
{
   if (!isTopLevel())
      return m_parent->mapToGlobal(p + pos());

   return p + pos() + m_window->clientOffset();;
}

AQPoint AQWidget::mapFromGlobal(const AQPoint &p) const
{
   if (!isTopLevel())
      return m_parent->mapFromGlobal(p - pos());

   return p - pos() - m_window->clientOffset();
}

bool AQWidget::event(IntuiMessage &msg)
{
   switch (msg.Class) {
   case IDCMP_RAWKEY:
   case IDCMP_VANILLAKEY: {
      for (int i= 0; i < m_actions.size(); ++i)
         if (m_actions[i]->matchShortcut(msg)) {
            return true;
         }
      return keyEvent(msg);
   }

   case IDCMP_MOUSEBUTTONS:
      switch (msg.Code) {
      case SELECTUP:
      case MENUUP:
         return mouseReleaseEvent(msg);

      case MENUDOWN:
         if (contextMenuPolicy() == 0) {
            return false;
         }

      case SELECTDOWN: {
         bool res = mousePressEvent(msg);
         if (DoubleClick(m_firstClickSeconds, m_firstClickMicros, msg.Seconds, msg.Micros))
            res |= mouseDoubleClickEvent(msg);
         else {
            m_firstClickSeconds = msg.Seconds;
            m_firstClickMicros = msg.Micros;
         }
         return res;
      }
      default:
         return false;
      }

   case IDCMP_MOUSEMOVE:
      m_firstClickSeconds = 0;
      m_firstClickMicros = 0;
      return mouseMoveEvent(msg);
   }
   return false;
}

void AQWidget::closeEvent()
{
   aqApp->exit();
}

bool AQWidget::keyEvent(const IntuiMessage &msg)
{
   return false;
}

void AQWidget::paintEvent(RastPort *rp, const AQRect &rect)
{
}

void AQWidget::changeEvent(int change)
{
}

void AQWidget::enterEvent()
{
}

void AQWidget::leaveEvent()
{
}

void AQWidget::focusInEvent()
{
}

void AQWidget::focusOutEvent()
{
}

bool AQWidget::wheelEvent(bool up)
{
   return false;
}

bool AQWidget::mouseDoubleClickEvent(const IntuiMessage &msg)
{
   return false;
}

bool AQWidget::mousePressEvent(const IntuiMessage &msg)
{
   return false;
}

bool AQWidget::mouseMoveEvent(const IntuiMessage &msg)
{
   return false;
}

bool AQWidget::mouseReleaseEvent(const IntuiMessage &msg)
{
   return false;
}

void AQWidget::resizeEvent(const AQPoint &oldSize)
{
}

