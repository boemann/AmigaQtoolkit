#ifndef AQWINDOW_H
#define AQWINDOW_H
#include <exec/types.h>

#include "AQKernel.h"
#include "AQString.h"
#include "AQLayout.h"

struct Menu;
struct Window;
struct IntuiMessage;
struct Screen;
struct DrawInfo;
struct RastPort;
struct Region;

class AQWidget;
class AQScreen;

class AQWindow {
public:
   enum Flags {
      WidgetArea = 0x00,  // always present
      LeftSizer = 0x01,
      RightSizer = 0x02,
      TopSizer = 0x04 ,
      BottomSizer = 0x08,
      CloseButton = 0x10,
      TitleBar = 0x20,
      MaximizeButton = 0x40,
      MinimizeButton = 0x80,
      Normal = 0xFF,
      Hover = 0x8000,
      Popup = 0x100,
      ToolTip = 0x300 // A popup that doesn't activate
   };

   AQWindow(AQWidget *widget, int modality, UWORD flags);
   ~AQWindow();

   void hide();

   void scroll(const AQPoint &delta, const AQRect &rect);

   void markDirty(const AQRect &rect);
   static void setMouseGrabber(AQWidget *w);

   void setPos(const AQPoint &p);

   AQPoint clientOffset() const;

   void paintDirty();

   void event(IntuiMessage &msg);   
   struct Window *m_window;

   AQWidget *widget() const;

private:
   void paintAll();
   void testFrameClick(const IntuiMessage &msg);
   void hoverTest(const IntuiMessage &msg, bool forceCheck);
   void paintWidget(AQWidget *w, RastPort *rp, AQRect rect, int winBg);
   AQWidget *pickMouseReceiver(IntuiMessage &msg);
   struct Menu *m_menu;
   bool m_active;
   WORD m_x;
   WORD m_y;
   Flags m_winControl;
   Flags m_flags;
   AQPoint m_border;
   AQPoint m_clientPos;
   WORD m_titleHeight;
   AQWidget *m_widget;
   AQWidget *m_buttonDownWidget;
   static AQWidget *s_mouseGrabWidget;
   Region *m_dirtyRegion;
   AQScreen *m_screen;
};

#endif
