#ifndef AQLAYOUT_H
#define AQLAYOUT_H

#include <graphics/gfx.h>

#include <AQKernel.h>
#include <AQWidget.h>

class AQLayoutItem
{
public:
   AQLayoutItem(AQWidget *w);

   virtual AQPoint minimumSize() const {return m_widget->minimumSize();}
   virtual AQPoint preferredSize() const {return m_widget->preferredSize();}
   virtual void layout(const AQPoint &size) {m_widget->setSize(size);}
   virtual bool likeToExpandX() const {return m_widget->likeToExpandX();}
   virtual bool likeToExpandY() const {return m_widget->likeToExpandY();}
   virtual void setPos(const AQPoint &pos) {m_widget->setPos(pos); }
   virtual AQPoint pos() const { return m_widget->pos(); }

private:
   AQWidget *m_widget;
};

class AQSpacerItem : public AQLayoutItem
{
public:
   AQSpacerItem(const AQPoint &s, bool eX, bool eY) : AQLayoutItem(nullptr), m_size(s),  m_expX(eX), m_expY(eY) {}

   AQPoint minimumSize() const {return m_size;}
   AQPoint preferredSize() const {return m_size;}
   void layout(const AQPoint &size) {}
   bool likeToExpandX() const {return m_expX;}
   bool likeToExpandY() const {return m_expY;}
   void setPos(const AQPoint &pos) { m_pos = pos;}
   AQPoint pos() const { return m_pos; }

private:
   AQPoint m_size;
   AQPoint m_pos;
   bool m_expX;
   bool m_expY;
};

class AQLayout : public AQLayoutItem
{
public:
   AQLayout(bool horiz);
   ~AQLayout();
   
   void addWidget(AQWidget *w);
   void addItem(AQLayoutItem *item);

   int count() const;

   AQPoint minimumSize() const;
   AQPoint preferredSize() const;
   void layout(const AQPoint &size);
   void setPos(const AQPoint &pos);
   AQPoint pos() const;

   bool likeToExpandX() const;
   bool likeToExpandY() const;

   void invalidate();

private:
   vector<AQLayoutItem *> m_items;
   bool m_horiz;
   bool m_hasCachedMin;
   bool m_hasCachedPref;
   bool m_hasCachedExpandX;
   bool m_hasCachedExpandY;
   volatile bool m_cachedExpandX;
   volatile bool m_cachedExpandY;
   AQPoint m_ownPos;
   AQPoint m_cachedMin;
   AQPoint m_cachedMax;
   AQPoint m_cachedPref;
   AQPoint m_spacing;
   bool m_valid;
   AQPoint m_currentSize;
};
   
#endif
