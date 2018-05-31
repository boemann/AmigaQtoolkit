#include "AQWidget.h"
#include "AQLayout.h"

#include <algorithm>

AQLayoutItem::AQLayoutItem(AQWidget *w)
   : m_widget(w)
{
}

AQLayout::AQLayout(bool horiz)
   : AQLayoutItem(nullptr)
   , m_horiz(horiz)
   , m_hasCachedMin(false)
   , m_hasCachedPref(false)
   , m_hasCachedExpandX(false)
   , m_hasCachedExpandY(false)
   , m_spacing(2,1)
   , m_valid(false)
{
}

AQLayout::~AQLayout()
{
   for (int i = 0; i < m_items.size(); ++i) {
      delete m_items[i];
   }
}

void AQLayout::addWidget(AQWidget *w)
{
   m_items.push_back(new AQLayoutItem(w));
   invalidate();
}

void AQLayout::addItem(AQLayoutItem *item)
{
   m_items.push_back(item);
   invalidate();
}

void AQLayout::removeWidget(AQWidget *w)
{
   for (int i = 0; i < m_items.size(); ++i) {
      if (m_items[i]->widget()	 == w) {
         removeItem(m_items[i]);
         break;
      }
   }
}

void AQLayout::removeItem(AQLayoutItem *item)
{
   m_items.erase(std::remove(m_items.begin(), m_items.end(), item), m_items.end());

   delete item;

   invalidate();
}

int AQLayout::count() const
{
   return m_items.size();
}

AQPoint AQLayout::minimumSize() const
{
   if (m_hasCachedMin)
      return m_cachedMin;

   AQPoint s;
   int contributeCount = 0;

   for (int i = 0; i < m_items.size(); ++i) {
      if (!m_items[i]->contributes())
         continue;
      AQPoint itemMin = m_items[i]->minimumSize();

      ++contributeCount;
      if (m_horiz) {
         s.x += itemMin.x;
         if (itemMin.y > s.y)
            s.y = itemMin.y;
      }
      else { 
         s.y += itemMin.y;
         if (itemMin.x > s.x)
            s.x = itemMin.x;
      }  
   }

   if (m_horiz)
      s.x += (contributeCount - 1) * m_spacing.x;
   else
      s.y += (contributeCount - 1) * m_spacing.y;

   const_cast<AQLayout *>(this)->m_cachedMin = s;
   const_cast<AQLayout *>(this)->m_hasCachedMin = true;
   return s;
}

AQPoint AQLayout::preferredSize() const
{
   if (m_hasCachedPref)
      return m_cachedPref;

   AQPoint s;
   int contributeCount = 0;

   for (int i = 0; i < m_items.size(); ++i) {
      if (!m_items[i]->contributes())
         continue;
      AQPoint itemPref = m_items[i]->preferredSize();

      ++contributeCount;
      if (m_horiz) {
         s.x += itemPref.x;
         if (itemPref.y > s.y)
            s.y = itemPref.y;
      }
      else { 
         s.y += itemPref.y;
         if (itemPref.x > s.x)
            s.x = itemPref.x;
      }  
   }

   if (m_horiz)
      s.x += (contributeCount - 1) * m_spacing.x;
   else
      s.y += (contributeCount - 1) * m_spacing.y;


   const_cast<AQLayout *>(this)->m_cachedPref = s;
   const_cast<AQLayout *>(this)->m_hasCachedPref = true;
   return s;
}

bool AQLayout::likeToExpandX() const
{
   if (m_hasCachedExpandX)
      return m_cachedExpandX;

   const_cast<AQLayout *>(this)->m_hasCachedExpandX = true;

   if (m_horiz) {
      for (int i = 0; i < m_items.size(); ++i) {
         if (!m_items[i]->contributes())
            continue;
         if (m_items[i]->likeToExpandX()) {
            const_cast<AQLayout *>(this)->m_cachedExpandX = true;
            return true;
         }
      }
      const_cast<AQLayout *>(this)->m_cachedExpandX = false;
      return false;
   } else { 
      for (int i = 0; i < m_items.size(); ++i) {
         if (!m_items[i]->contributes())
            continue;
         if (!m_items[i]->likeToExpandX()) {
            const_cast<AQLayout *>(this)->m_cachedExpandX = false;
            return false;
          }
      }
      const_cast<AQLayout *>(this)->m_cachedExpandX = true;
      return true;
   }
}

bool AQLayout::likeToExpandY() const
{
   if (m_hasCachedExpandY)
      return m_cachedExpandY;

   const_cast<AQLayout *>(this)->m_hasCachedExpandY = true;

   if (m_horiz) {
      for (int i = 0; i < m_items.size(); ++i) {
         if (!m_items[i]->contributes())
            continue;
         if (!m_items[i]->likeToExpandY()) {
            const_cast<AQLayout *>(this)->m_cachedExpandY = false;
            return false;
         }
      }
      const_cast<AQLayout *>(this)->m_cachedExpandY = true;
      return true;
   } else {
      for (int i = 0; i < m_items.size(); ++i) {
         if (!m_items[i]->contributes())
            continue;
         if (m_items[i]->likeToExpandY()) {
            const_cast<AQLayout *>(this)->m_cachedExpandY = true;
            return true;
         }
      }
      const_cast<AQLayout *>(this)->m_cachedExpandY = false;
      return false;
   }
}

void AQLayout::invalidate()
{
   m_hasCachedMin = false;
   m_hasCachedPref = false;
   m_hasCachedExpandX = false;
   m_hasCachedExpandY = false;

   m_valid = false;
   for (int i = 0; i < m_items.size(); ++i) {
      AQLayout *l = dynamic_cast<AQLayout *>(m_items[i]);
      if (l)
         l->invalidate();
   }
}

void AQLayout::setPos(const AQPoint &pos)
{
   m_ownPos = pos;
}

AQPoint AQLayout::pos() const
{
   return m_ownPos;
}

void AQLayout::layout(const AQPoint &size)
{
   AQPoint itemPos;
   AQPoint totalMin = minimumSize();
   AQPoint totalPreferred = preferredSize();
   WORD expandShare = 0;
   WORD askedShare = 0;

   int numContribs = 0;
   for (int i = 0; i < m_items.size(); ++i) {
      if (m_items[i]->contributes())
         ++numContribs;
   }

   if (numContribs == 0)
      return;
   
   if (m_valid && m_currentSize == size) {
      // Although we don't need to relayout we may need to move our items
      AQPoint offset = m_ownPos - m_items.front()->pos();

      if (offset.x != 0 || offset.y != 0) {
         for (int i = 0; i < m_items.size(); ++i) {
            AQLayoutItem *item = m_items[i];
            item->setPos(item->pos() + offset);
         }
      }
      return;
   }
   m_currentSize = size;
   m_valid = true;

   if (m_horiz) {
      WORD sumSpace = (m_items.size() - 1)* m_spacing.x;
      if (totalMin.x > size.x) {
         // this is the rude case where we need to harm the items
         askedShare = (totalMin.x - size.x) / numContribs;
         for (int i = 0; i < m_items.size(); ++i) {
            AQLayoutItem *item = m_items[i];
            if (!item->contributes())
               continue;
            AQPoint itemSize = item->minimumSize();
      
            itemSize.x -= askedShare;
            if(item->likeToExpandY())
               itemSize.y = size.y;
            else
               itemSize.y = aqMin(item->preferredSize().y, size.y);
            
            item->setPos(itemPos+m_ownPos);
            item->layout(itemSize);
            itemPos.x += itemSize.x;
         }
         return;
      } else if (totalPreferred.x > size.x) {
         // This is where we shrink as little as possible
         // Loop to see what the share to ask for is. It will be
         // more than a fair share if not all can give it.
         WORD distribute = totalPreferred.x - size.x;
         WORD askCorrection = 0;
         WORD remainingGivers;
         askedShare = distribute / numContribs;
         do {
            askedShare += askCorrection;
            distribute = totalPreferred.x - size.x;
            remainingGivers = numContribs;

            for (int i = 0; i < m_items.size(); ++i) {
               if (!m_items[i]->contributes())
                  continue;
               WORD maxShare = m_items[i]->preferredSize().x - m_items[i]->minimumSize().x;
               if (maxShare < askedShare) {
                  --remainingGivers;
                  distribute -= maxShare;
               } else
                  distribute -= askedShare;
            }
            if (remainingGivers)
               askCorrection = distribute / remainingGivers;
         } while (remainingGivers && askCorrection);
         
         // Fall through for assignment below
      } else {
         WORD expandCount = 0;
         // This is the case where we can expand
         for (int i = 0; i < m_items.size(); ++i) {
            if (!m_items[i]->contributes())
               continue;
            if ( m_items[i]->likeToExpandX())
               ++expandCount;
         }
         if (expandCount)
            expandShare = (size.x - totalPreferred.x) / expandCount;
         // Fall through for assignment below
      }
      
      // Time to assign
      for (int i = 0; i < m_items.size(); ++i) {
         AQLayoutItem *item = m_items[i];
         if (!item->contributes())
            continue;
         AQPoint itemSize = item->preferredSize();
      
         // Make sure we don't go below minimum
         if (askedShare) {
            WORD maxShare = itemSize.x - item->minimumSize().x;
            if (maxShare < askedShare)
               itemSize.x -= maxShare;
            else
               itemSize.x -= askedShare;
         }

         if (item->likeToExpandX())
            itemSize.x += expandShare;

         if(item->likeToExpandY())
            itemSize.y = size.y;
         else
            itemSize.y = aqMin(itemSize.y, size.y);
               
         item->setPos(itemPos + m_ownPos);
         item->layout(itemSize);
         itemPos.x += itemSize.x + m_spacing.x;
      }
   } else { // VERTICAL
      WORD sumSpace = (numContribs - 1)* m_spacing.y;
      if (totalMin.y > size.y) {
         // this is the rude case where we need to harm the items
         askedShare = (totalMin.y - size.y) / numContribs;
         for (int i = 0; i < m_items.size(); ++i) {
            AQLayoutItem *item = m_items[i];
            if (!item->contributes())
               continue;
            AQPoint itemSize = item->minimumSize();
      
            itemSize.y -= askedShare;
            if(item->likeToExpandX())
               itemSize.x = size.x;
            else
               itemSize.x = aqMin(item->preferredSize().x, size.x);
            
            item->setPos(itemPos + m_ownPos);
            item->layout(itemSize);
            itemPos.y += itemSize.y;
         }
         return;
      } else if (totalPreferred.y > size.y) {
         // This is where we shrink as little as possible
         // Loop to see what the share to ask for is. It will be
         // more than a fair share if not all can give it.
         WORD distribute = totalPreferred.y - size.y;
         WORD askCorrection = 0;
         WORD remainingGivers;
         askedShare = distribute / numContribs;
         do {
            askedShare += askCorrection;
            distribute = totalPreferred.y - size.y;
            remainingGivers = numContribs;

            for (int i = 0; i < m_items.size(); ++i) {
               if (!m_items[i]->contributes())
                  continue;
               WORD maxShare = m_items[i]->preferredSize().y - m_items[i]->minimumSize().y;
               if (maxShare < askedShare) {
                  --remainingGivers;
                  distribute -= maxShare;
               } else
                  distribute -= askedShare;
            }
            if (remainingGivers)
               askCorrection = distribute / remainingGivers;
         } while (remainingGivers && askCorrection);

         // Fall through for assignment below
      } else {
         WORD expandCount = 0;
         // This is the case where we can expand
         for (int i = 0; i < m_items.size(); ++i) {
            if (!m_items[i]->contributes())
               continue;
            if (m_items[i]->likeToExpandY())
               ++expandCount;
         }
         if (expandCount)
            expandShare = (size.y - totalPreferred.y) / expandCount;
         // Fall through for assignment below
      }
      
      // Time to assign
      for (int i = 0; i < m_items.size(); ++i) {
         AQLayoutItem *item = m_items[i];
         if (!m_items[i]->contributes())
            continue;
         AQPoint itemSize = item->preferredSize();
      
         // Make sure we don't go below minimum
         if (askedShare) {
            WORD maxShare = itemSize.y - item->minimumSize().y;
            if (maxShare < askedShare)
               itemSize.y -= maxShare;
            else
               itemSize.y -= askedShare;
         }
         if (item->likeToExpandY())
            itemSize.y += expandShare;

         if(item->likeToExpandX())
            itemSize.x = size.x;
         else
            itemSize.x = aqMin(itemSize.x, size.x);
               
         item->setPos(itemPos + m_ownPos);
         item->layout(itemSize);
         itemPos.y += itemSize.y + m_spacing.y;
      }
   }
}
