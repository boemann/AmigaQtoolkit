#ifndef AQTEXTEDIT_H
#define AQTEXTEDIT_H

#include <AQWidget.h>

class AQTextDoc;
class AQTextCursor;
class AQScrollBar;
class AQAction;

class AQTextEdit : public AQWidget
{
public:
   AQTextEdit(AQWidget *parent = nullptr, bool withScroll = true);
   ~AQTextEdit();

   AQTextDoc *document() const;
   void setDocument(AQTextDoc *doc, AQTextCursor *cursor);
   AQTextCursor *mainCursor() const;


   AQScrollBar *verticalScrollBar() const;

   void setXOffset(int offX);
   void setYOffset(int ooXY);

   void scrollUpdate(int v);

   enum EnsureType {
      Nearest,
      AtTop,  //not implemented yet
      MiddleHalf,  //not implemented yet
      AtBottom  //not implemented yet
   };
   void ensureCursorVisible(EnsureType type = Nearest);

   void cut();
   void copy();
   void paste();

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

   void onDocumentChanged(AQObject *obj);

private:
   AQRect cursorRect(bool fullLineWidth = false) const;

   AQTextDoc *m_doc;
   AQTextCursor *m_cursor;
   AQPoint m_docOffset;
   bool m_ownsDoc;
   AQScrollBar *m_scrollBar;
   AQAction *m_cutAction;
   AQAction *m_copyAction;
   AQAction *m_pasteAction;
};
   
#endif
