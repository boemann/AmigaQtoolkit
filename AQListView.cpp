#include "AQListView.h"

#include <string.h>
#include <algorithm>

#include <graphics/gfxbase.h>

#include <proto/graphics.h>
#include <proto/layers.h>

#include <AQScrollBar.h>

#define IconWidth 18

AQListItem::AQListItem(AQListView *lv)
   : m_parentItem(nullptr)
   , m_expanded(false)
   , m_expanderPolicy(1)
   , m_listView(lv)
{
}

AQListItem::~AQListItem()
{
   for (int i=0; i < m_children.size(); ++i)
      delete m_children[i];
}

void AQListItem::setExpanderPolicy(int p)
{
   m_expanderPolicy = p;
}

int AQListItem::expanderPolicy() const
{
   return m_expanderPolicy;
}

void AQListItem::setText(int column, const AQString &string)
{
   if (column > 9)
      column = 9;

   m_text[column] = string;

   if (m_listView && m_listView->wordWrap())
      m_listView->updateScrollBar();

//   if (m_listView)
//      m_listView->emit("dataChanged", this);
}

AQString AQListItem::text(int column) const
{
   if (column > 9)
      column = 9;

   return m_text[column];
}

void AQListItem::setIcon(int column, const AQIcon &icon)
{
   if (column > 9)
      column = 9;

   m_icon[column] = icon;

//   if (m_listView)
//      m_listView->emit("dataChanged", this);
}

AQIcon AQListItem::icon(int column) const
{
   if (column > 9)
      column = 9;

   return m_icon[column];
}

void AQListItem::addChild(AQListItem *child)
{
   m_children.push_back(child);
   child->m_parentItem = this;

   if (m_listView)
      m_listView->updateScrollBar();
//      m_listView->emit("dataChanged", this);
}

AQListItem *AQListItem::parentItem() const
{
   return m_parentItem;
}

AQListItem *AQListItem::child(int i) const
{
   return m_children[i];
}

int AQListItem::childCount() const
{
   return m_children.size();
}

int AQListItem::indexOfChild(const AQListItem *item) const
{
   for(int i = 0; i < m_children.size(); ++i)
      if (m_children[i] == item)
         return i;

   return -1;
}

int AQListItem::visibleCount() const
{
   if (!isExpanded())
      return 0;

   int sum = 0;
   for(int i = 0; i < m_children.size(); ++i)
      sum += m_children[i]->visibleCount() + 1;

   return sum;
}

void AQListItem::sortChildren(bool(*comp)(const AQListItem *, const AQListItem *))
{
   sort(m_children.begin(), m_children.end(), comp);
}

bool AQListItem::isExpanded() const
{
   return m_expanded;
}

void AQListItem::setExpanded(bool e)
{
   if (m_expanded == e)
      return;

   m_expanded = e;
   if (m_listView)
      m_listView->updateScrollBar();
}

AQListView::AQListView(AQWidget *parent)
   : AQWidget(parent)
   , m_rootItem(new AQListItem(this))
   , m_scrollBar(new AQScrollBar(false, this))
   , m_rightIcon(AQIcon("right"))
   , m_downIcon(AQIcon("down"))
   , m_selectedItem(nullptr)
   , m_treeMode(true)
   , m_wordWrap(false)
   , m_expanderWidth(10)
{
   setBgPen(0); // no bg but we draw ourself
   setExpanding(true, true);
   setMinimumSize(AQPoint(20, 0) + m_scrollBar->minimumSize());
   setPreferredSize(AQPoint(150, 150));
   m_rootItem->setExpanded(true);

   Connect<AQListView>(m_scrollBar, "valueChanged", this, &AQListView::scrollUpdate);
   Connect<AQListView>(this, "dataChanged", this, &AQListView::updateScrollBar);
}

AQListView::~AQListView()
{
}

void AQListView::setTreeMode(bool on)
{
   m_treeMode = on;
}

bool AQListView::treeMode() const
{
   return m_treeMode;
}

void AQListView::setWordWrap(bool on)
{
   m_wordWrap = on;
}

bool AQListView::wordWrap() const
{
   return m_wordWrap;
}

void AQListView::addTopLevelItem(AQListItem *item)
{
   m_rootItem->addChild(item);
}

AQListItem *AQListView::itemAtRecurse(const AQListItem *root, int &x, int &y, int searchY)
{
      RastPort rp;
      InitRastPort(&rp);
   for (int i = 0; i < root->childCount(); i++) {
      AQListItem *item = root->child(i);

      if (m_treeMode)
         x += m_expanderWidth;

      int len = item->text(0).size();
      int availLen = len;

      if (m_wordWrap) {
         availLen = (size().x - m_scrollBar->size().x - IconWidth - x - 4) /
                    rp.TxWidth;
      }

      char *str = item->text(0);
      do {
         len -= availLen;
         str += availLen;
         y += fontHeight() + 2;
      } while (len>0);


      if (searchY < y) {
         if (m_treeMode)
            x -= m_expanderWidth;

         return item;
      }

      if (m_treeMode) {
          if(item->isExpanded() && (item = itemAtRecurse(item, x, y, searchY))) {
             return item;
          }
          x -= m_expanderWidth;
      }
   }

   return nullptr;
}

AQListItem *AQListView::rootItem()
{
   return m_rootItem;
}

AQListItem *AQListView::itemAt(const AQPoint &point)
{
   int x = 0;
   int y = 0;
   return itemAtRecurse(m_rootItem, x, y, point.y);
}

AQListItem *AQListView::itemAbove(const AQListItem *item) const
{
   AQListItem *p = item->parentItem();

   if (!p)
      return nullptr;

   int index = p->indexOfChild(item);

   if (index > 0) {
      AQListItem *bottomMost =  p->child(index - 1);

      while (bottomMost->isExpanded() && bottomMost->childCount() > 0)
         bottomMost = bottomMost->child(bottomMost->childCount() - 1);

      return bottomMost;
   } else
      if (p->parentItem())
         return item->parentItem();
      else
         return nullptr;
}

AQListItem *AQListView::itemBelow(const AQListItem *item) const
{
   if (item->isExpanded() && item->childCount() > 0) {
      return item->child(0);
   }

   if (!item->parentItem())
      return nullptr;

   AQListItem *p = item->parentItem();

   int index = p->indexOfChild(item);

   if (index < p->childCount() - 1)
      return p->child(index + 1);
   else {
      // need to find the next sibling to parent
      if (!p->parentItem())
         return nullptr;

      index = p->parentItem()->indexOfChild(p);
      
      if (index < p->parentItem()->childCount() - 1)
         return p->parentItem()->child(index + 1);
   }
   return nullptr;
}

void AQListView::selectItem(AQListItem *item)
{
   m_selectedItem = item;
   emit("itemActivated", item);
   update();
}

vector<AQListItem *> AQListView::selectedItems() const
{
   vector<AQListItem*> res;

   if (m_selectedItem)
      res.push_back(m_selectedItem);

   return res;
}

void AQListView::scrollUpdate(int delta)
{
   update();
}

void AQListView::updateScrollBar()
{
   if (m_wordWrap) {
      int x=0;
      RastPort rp;
      InitRastPort(&rp);
      m_scrollBar->setMaximum(countHeight(&rp, x, m_rootItem));
   } else
      m_scrollBar->setMaximum(m_rootItem->visibleCount());
}

void AQListView::clear()
{
   delete m_rootItem;

   m_rootItem = new AQListItem(this);
   m_rootItem->setExpanded(true);
   update();
}

int AQListView::countHeight(RastPort *rp, int &x, AQListItem *parent) const
{
   int lines;
   for (int i = 0; i < parent->childCount(); ++i) {
      AQListItem *item = parent->child(i);

      int len = item->text(0).size();

      if (m_treeMode)
         x += m_expanderWidth;

      int availLen = len;
      if (m_wordWrap)
         availLen = (size().x - m_scrollBar->size().x - IconWidth - x - 4)
                     / rp->TxWidth;

      char *str = item->text(0);
      do {
         len -= availLen;
         str += availLen;
         ++lines;
      } while (len>0);

      if (m_treeMode) {
         if (item->childCount() && item->isExpanded()) {
            lines += countHeight(rp, x, item);
         }

         x -= m_expanderWidth;
      }
   }
   return lines;
}

void AQListView::paintChildren(RastPort *rp, int &x, int &y, AQListItem *parent)
{
   for (int i = 0; i < parent->childCount(); ++i) {
      AQListItem *item = parent->child(i);

      int top = y - rp->TxBaseline;
      int len = item->text(0).size();

      if (m_treeMode)
         x += m_expanderWidth;

      int availLen = len;
      if (m_wordWrap)
         availLen = (size().x - m_scrollBar->size().x - IconWidth - x - 4)
                     / rp->TxWidth;

      int bottom = top + (rp->TxHeight + 2) * ((len - 1)/ availLen + 1) - 2;
      if (item == m_selectedItem) {
         SetAPen(rp, 3);
         if (hasFocus())
            RectFill(rp, 0, top -1, size().x - m_scrollBar->size().x - 7, bottom);
         else {
            Move(rp, 0, top - 1);
            Draw(rp, size().x - m_scrollBar->size().x - 7, top - 1);
            Draw(rp, size().x - m_scrollBar->size().x - 7, bottom);
            Draw(rp, 0, bottom);
            Draw(rp, 0, top -1);
         }
      }


      if (!item->icon(0).isNull())
         item->icon(0).paint(rp, AQPoint(x, top), AQIcon::Small);

      SetAPen(rp, 1);
      char *str = item->text(0);
      do {
         Move(rp, x + IconWidth, y);
         Text(rp, str, aqMin(availLen, len));
         len -= availLen;
         str += availLen;
         y += fontHeight() + 2;
      } while (len>0);

      if (m_treeMode) {
         if (item->childCount() && item->isExpanded()) {
            paintChildren(rp, x, y, item);
         }

         x -= m_expanderWidth;

         if(item->expanderPolicy() > 0) {
            if (item->childCount() || item->expanderPolicy() == 2) {
               if (item->isExpanded())
                  m_downIcon.paint(rp, AQPoint(x, top), AQIcon::Small);
               else
                  m_rightIcon.paint(rp, AQPoint(x , top), AQIcon::Small);
            }
         }
      }
   }
}

void AQListView::paintEvent(RastPort *rp, const AQRect &rect)
{
   AQPoint s = size();
#define SHADOW 1
#define SHINE 2

   LONG bottom = s.y - 1;
   LONG right = s.x - 1 - m_scrollBar->size().x - 2;

   AQRect clipRect(rect);
   clipRect.bottomRight.x = aqMin(right, rect.bottomRight.x);
   pushClipRect(rp, clipRect);

   int y = 0;
   if (m_scrollBar)
      y -= m_scrollBar->value() * (rp->TxHeight + 2);

   ScrollLayer(0, rp->Layer, -2, -2); //offset painting in widget

   int x = 0;
   y += rp->TxBaseline;
   SetAPen(rp, 1);
   SetDrMd(rp, JAM1);
   paintChildren(rp, x, y, m_rootItem);

   ScrollLayer(0, rp->Layer, 2, 2); // restore offset

   SetAPen(rp, SHADOW);
   Move(rp, right, 0);
   Draw(rp, 0, 0);
   Draw(rp, 0, bottom);
   SetAPen(rp, SHINE);
   Draw(rp, right, bottom);
   Draw(rp, right, 1);

   restoreClipping(rp);
}

bool AQListView::keyEvent(const IntuiMessage &msg)
{
   if(msg.Class ==IDCMP_RAWKEY) {
      switch (msg.Code) {
      case Key_Left:
         if (!m_selectedItem)
             return true;
         if (m_selectedItem->parentItem()) {
            if (m_selectedItem->isExpanded() && m_selectedItem->childCount() > 0) {
               m_selectedItem->setExpanded(false);
               update();
               return true;
            } else
               if (m_selectedItem->parentItem()->parentItem()) {
                  selectItem(m_selectedItem->parentItem());
               }
         }
         break;
      case Key_Right:
         if (!m_selectedItem)
             return true;
         if (m_selectedItem->parentItem()) {
            if (m_selectedItem->isExpanded() && m_selectedItem->childCount() > 0) {
               selectItem(m_selectedItem->child(0));
               return true;
            } else {
               m_selectedItem->setExpanded(true);
               emit("itemExpanded", m_selectedItem);
               update();
            }
         }
         break;
      case Key_Up: {
         AQListItem *item = itemAbove(m_selectedItem);
         if (item) {
            selectItem(item);
         }
         break;
      }
      case Key_Down: {
         AQListItem *item = itemBelow(m_selectedItem);
         if (item) {
            selectItem(item);
         }
         break;
      }
      case Key_Home:
         break;
      case Key_End:
         break;
      case Key_PageUp:
         break;
      case Key_PageDown:
         break;
      default:
         break;
      }
   }
   return true;
}

void AQListView::focusInEvent()
{
   update();
}

void AQListView::focusOutEvent()
{
   update();
}

bool AQListView::wheelEvent(bool up)
{
   m_scrollBar->wheelEvent(up);

   return true;
}


bool AQListView::mouseDoubleClickEvent(const IntuiMessage &msg)
{
   AQPoint clickPoint(msg.MouseX - 2, msg.MouseY - 2);

   int x = 0;
   int y = 0;

   clickPoint.y += m_scrollBar->value() * (fontHeight() + 2);
   AQListItem *item = itemAtRecurse(m_rootItem, x, y, clickPoint.y);

   if (item) {
      if (!m_treeMode || clickPoint.x >= x + m_expanderWidth) {
         emit("itemDoubleClicked", item);
      }
   }

   return true;
}

bool AQListView::mousePressEvent(const IntuiMessage &msg)
{
   setFocus();

   AQPoint clickPoint(msg.MouseX - 2, msg.MouseY - 2);

   int x = 0;
   int y = 0;

   clickPoint.y += m_scrollBar->value() * (fontHeight() + 2);
   AQListItem *item = itemAtRecurse(m_rootItem, x, y, clickPoint.y);

   if (item) {
      if (m_treeMode && clickPoint.x < x + m_expanderWidth) {
         item->setExpanded(!item->isExpanded());
         if (item->isExpanded())
            emit("itemExpanded", item);
      } else {
            selectItem(item);
      }
   }
   update();

   return true;
}

bool AQListView::mouseMoveEvent(const IntuiMessage &msg)
{
   return true;
}

bool AQListView::mouseReleaseEvent(const IntuiMessage &msg)
{
   return true;
}

void AQListView::resizeEvent(const AQPoint &oldSize)
{
   m_scrollBar->setPageStep((size().y - 2) / (fontHeight() + 2));
   m_scrollBar->setSize(AQPoint(m_scrollBar->preferredSize().x, size().y));
   m_scrollBar->setPos(AQPoint(size().x - m_scrollBar->size().x, 0));
}

