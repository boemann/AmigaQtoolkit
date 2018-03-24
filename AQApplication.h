#ifndef AQAPPLICATION_H
#define AQAPPLICATION_H

#include <vector.h>

#include <AQObject.h>

#include "AQKernel.h"
    
#define aqApp AQApplication::s_aqApp

class AQWindow;
class AQWidget;
struct MsgPort;
struct RastPort;
class AQString;
class AQClipboard;
class AQAction;

typedef long BPTR;

struct Screen;
struct DrawInfo;

class AQScreen {
public:
   AQScreen();
   ~AQScreen();
   AQPoint size() const { return AQPoint(640, 512);}
   AQPoint menubarSize() const;
   AQRect availableGeometry() const { return AQRect(0,10,640,502);}
   void drawAmigaKey(RastPort *rp, int x, int y, bool enabled = true);

   friend class AQWindow;
   friend class AQIcon;

private:
   Screen *m_screen;
   DrawInfo *m_drawInfo;
   void *m_visualInfo;
};

class AQApplication : public AQObject
{
public:
   AQApplication();
   ~AQApplication();

   void startAsyncRead(BPTR file, char *buffer, int size);
   
   void exec();
   void processEvents(bool &stayAlive);

   void exit(int code = 0);

   void addAction(AQAction *a);

   AQClipboard *clipboard() const {return m_clipboard; }
   AQWidget *focusWidget() const;
   MsgPort *userPort() const;
   bool isWindowBlocked(AQWindow *window) const;

   AQWidget *hoveredWidget() const;

   AQScreen *screen(AQWidget *w = nullptr) const;

   static AQApplication *s_aqApp;

   friend class AQWidget;
   friend class AQWindow;

protected:
   void setFocus(AQWidget *w);
   void registerWindow(AQWindow *window);
   void registerModalWindow(AQWindow *window);
   void unregisterWindow(AQWindow *window);
   void setHoveredWidget(AQWidget *w);

private:
   vector<AQWindow *>m_windows;
   vector<AQWindow *>m_modalWindows;
   vector<AQAction *>m_actions;
   bool m_closing;
   AQWidget *m_focusWidget;
   AQWidget *m_hoveredWidget;
   MsgPort *m_userPort;
   MsgPort *m_asyncFilePort;
   int m_loopDepth;
   AQClipboard *m_clipboard;
   AQScreen *m_defaultScreen;
};

#endif
