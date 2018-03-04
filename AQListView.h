#ifndef AQLISTVIEW_H
#define AQLISTVIEW_H

#include <AQWidget.h>
#include <AQKernel.h>
#include <AQIcon.h>
#include <vector.h>

class AQScrollBar;
class AQListView;

class AQListItem : public AQObject
{
public:
   AQListItem(AQListView *lv);
   ~AQListItem();

   AQString text(int column) const;
   void setText(int column, const AQString &string);

   AQIcon icon(int column) const;
   void setIcon(int column, const AQIcon &icon);

   void addChild(AQListItem *child);
   AQListItem *parentItem() const;

   AQListView *listView() const;

   AQListItem *child(int i) const;
   int childCount() const;
   int indexOfChild(const AQListItem *item) const;

   void sortChildren(bool(*comp)(const AQListItem *, const AQListItem *));

   bool isExpanded() const;
   void setExpanded(bool b);
   int visibleCount() const;

   void setExpanderPolicy(int p);
   int expanderPolicy() const;

private:
   vector<AQListItem *> m_children;
   AQListItem *m_parentItem;
   AQListView *m_listView;
   bool m_expanded;
   int m_expanderPolicy;
   AQString m_text[10];
   AQIcon m_icon[10];
};

class AQListView : public AQWidget
{
public:
   AQListView(AQWidget *parent = nullptr);
   ~AQListView();
   
   void setTreeMode(bool on);
   void setWordWrap(bool on);

   void addTopLevelItem(AQListItem *item);

   void scrollUpdate(int v);
   void updateScrollBar();

   AQListItem *rootItem();
   AQListItem *itemAt(const AQPoint &p);

   AQListItem *itemAbove(const AQListItem *item) const;
   AQListItem *itemBelow(const AQListItem *item) const;

   vector<AQListItem *> selectedItems() const;
   void clear();

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual bool keyEvent(const IntuiMessage &msg);
   virtual void focusInEvent();
   virtual void focusOutEvent();
   virtual bool wheelEvent(bool up);
   virtual bool mouseDoubleClickEvent(const IntuiMessage &msg);
   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);
   virtual void resizeEvent(const AQPoint &oldSize);

private:
   void paintChildren(RastPort *rp, int &x, int &, AQListItem *parent);
   AQListItem *itemAtRecurse(const AQListItem *root, int h, int &x, int &y, int searchY);

   AQScrollBar *m_scrollBar;
   AQListItem *m_rootItem;
   AQIcon m_rightIcon;
   AQIcon m_downIcon;
   AQListItem *m_selectedItem;
   bool m_treeMode;
   bool m_wordWrap;
   int m_expanderWidth;
};
   
#endif
