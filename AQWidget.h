#ifndef AQWIDGET_H
#define AQWIDGET_H

#include <vector.h>

#include <graphics/gfx.h>
#include <intuition/intuition.h>

#include <AQKernel.h>
#include <AQObject.h>

struct IntuiMessage;
struct RastPort;
struct Region;
class AQWindow;
class AQLayout;
class AQAction;
class AQMenu;

class AQWidget : public AQObject
{
public:
   AQWidget(AQWidget *parent = nullptr);
   ~AQWidget();
   
   AQWidget *parent();

   void setParent(AQWidget *parentW);

   bool isEnabled() const { return m_enabled;}
   void setEnabled(bool enabled) {m_enabled = enabled;}

   void show();
   void hide();

   bool isVisible() const;
   void setVisible(bool visible);

   bool isTopLevel() const;

   void setMenu(AQMenu *menu);

   void setTooltip(const AQString &tooltip);

   void setWindowTitle(const AQString &title);
   void setWindowModality(int modal); // 0=not (default) 1 = application modal
   void setWindowFlags(UWORD flags); // see AQWindow.h for flags
   int fontHeight() const;
   void grabMouse();
   void releaseMouse();

   const vector<class AQWidget *> &children() const;

   int bgPen() const;
   void setBgPen(int pen);
   
   AQPoint size() const;
   void setSize(const AQPoint &s);
   AQPoint pos() const;
   void setPos(const AQPoint &pos);
   AQRect geometry() const;

   AQPoint minimumSize() const;
   void setMinimumSize(const AQPoint &s);
   AQPoint preferredSize() const;
   void setPreferredSize(const AQPoint &s);
   bool likeToExpandX() const;
   bool likeToExpandY() const;
   void setExpanding(bool x, bool y);
   AQLayout *layout() const;
   void setLayout(AQLayout *layout);
   void updateGeometry();


   void scroll(const AQPoint &delta, const AQRect &rect);

   void update();
   void update(const AQRect &rect);

   void setFocus();
   bool hasFocus();

   void addAction(AQAction *a);

   int contextMenuPolicy() const;
   void setContextMenuPolicy(int val); // dont use 0 as it's behaviour will change

   AQWidget *widgetAt(WORD &x, WORD &y); // returns this or child

   AQPoint mapToGlobal(const AQPoint &p) const;
   AQPoint mapFromGlobal(const AQPoint &p) const;

   enum {
      Key_Numeric0 = 0x0F,
      Key_Numeric1 = 0x1D,
      Key_Numeric2,
      Key_Numeric3,
      Key_Numeric4 = 0x2D,
      Key_Numeric5,
      Key_Numeric6,
      Key_Numeric7 = 0x3D,
      Key_Numeric8,
      Key_Numeric9,
      Key_Space,
      Key_Backspace,
      Key_Tab,
      Key_Enter,  // The key on the numeric keypad
      Key_Return,
      Key_Esc,
      Key_Delete,
      Key_Insert,
      Key_NumericSomething1,
      Key_PageUp = 0x48,
      Key_PageDown = 0x49,
      Key_NemericMinus,
      Key_Up = 0x4C,
      Key_Down,
      Key_Right,
      Key_Left,
      Key_F1,
      Key_F2,
      Key_F3,
      Key_F4,
      Key_F5,
      Key_F6,
      Key_F7,
      Key_F8,
      Key_F9,
      Key_F10,
      Key_NumericLeftBracket,
      Key_NumericRightBracket,
      Key_NumericSlash,
      Key_NumericStar,
      Key_NumericPlus,
      Key_Help,
      Key_Home = 0x70,
      Key_End = 0x71,
      Key_WheelUp = 0x7A, // passed on to widget as  wheelEvent()
      Key_WheelDown = 0x7B, // assed on to widget as wheelEvent()
   } Key;

   enum {
      Activation,
      InActivation
   } Change;

   friend class AQWindow;
   friend class AQApplication;

protected:
   virtual bool event(IntuiMessage &msg);
   virtual void closeEvent();
   virtual void changeEvent(int change);
   virtual bool keyEvent(const IntuiMessage &msg);
   virtual void paintEvent(RastPort *rp, const AQRect &rect);
   virtual void enterEvent();
   virtual void leaveEvent();
   virtual void focusInEvent();
   virtual void focusOutEvent();
   virtual bool wheelEvent(bool up);
   virtual bool mouseDoubleClickEvent(const IntuiMessage &msg);
   virtual bool mousePressEvent(const IntuiMessage &msg);
   virtual bool mouseMoveEvent(const IntuiMessage &msg);
   virtual bool mouseReleaseEvent(const IntuiMessage &msg);
   virtual void resizeEvent(const AQPoint &oldSize);
   void pushClipRect(RastPort *rp, const AQRect &r);
   void restoreClipping(RastPort *rp);

private:
   void takeFocus(AQWidget *child);
   bool relayEventToChild(IntuiMessage &msg);   
   AQPoint m_pos;
   AQPoint m_size;
   AQPoint m_minimumSize;
   AQPoint m_maximumSize;
   AQPoint m_preferredSize;
   int m_bgPen;
   bool m_expandX;
   bool m_expandY;
   bool m_enabled;
   bool m_visible;
   AQString m_title;
   AQString m_tooltip;

   AQLayout *m_layout;
   vector<AQWidget *> m_children;
   vector<AQAction *> m_actions;
   AQWidget *m_parent;
   AQWidget *m_focusChild;
   AQWindow *m_window;
   Region *m_oldClip;
   ULONG m_firstClickSeconds;
   ULONG m_firstClickMicros;
   int m_contextMenuPolicy;
   bool m_frameless;
   int m_windowModality;
   UWORD m_windowFlags;
   AQMenu *m_menu;
};
   
#endif
