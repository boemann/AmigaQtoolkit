#ifndef AQAPPLICATION_H
#define AQAPPLICATION_H

#include <vector.h>

#include <AQObject.h>

#include "AQKernel.h"
    
#define aqApp AQApplication::s_aqApp

class AQWindow;
class AQWidget;
struct MsgPort;
class AQString;
class AQClipboard;
class AQAction;

typedef long BPTR;


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
};

#endif
