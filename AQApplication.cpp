#include "AQApplication.h"
#include <stdio.h>
#include <algorithm>

#include <exec/types.h>
#include <intuition/intuition.h>
#include <libraries/gadtools.h>

#ifdef __GNUC__
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/layers.h>
#else
#include <pragma/exec_lib.h>
#include <pragma/graphics_lib.h>
#include <pragma/intuition_lib.h>
#include <pragma/gadtools_lib.h>
#include <pragma/layers_lib.h>
#endif

#include <AQWindow.h>
#include <AQObject.h>
#include <AQString.h>
#include <AQClipboard.h>
#include <AQAction.h>

using std::vector;

extern struct ExecBase *SysBase;
struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;
struct Library *GadToolsBase = nullptr;
struct Library *LayersBase = nullptr;
extern struct Library *DOSBase;
struct Library *IconBase = nullptr;

const AQPoint operator+(const AQPoint &p1, const AQPoint &p2)
{
   return AQPoint(p1.x + p2.x, p1.y + p2.y);
}

const AQPoint operator-(const AQPoint &p1, const AQPoint &p2)
{
   return AQPoint(p1.x - p2.x, p1.y - p2.y);
}

bool operator==(const AQPoint &p1, const AQPoint &p2)
{
   return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const AQPoint &p1, const AQPoint &p2)
{
   return p1.x != p2.x || p1.y != p2.y;
}

AQPoint AQPoint::operator-() const
{
   return AQPoint(-x, -y);
}

AQRect AQRect::intersected(const AQRect &o) const
{
   AQRect res(topLeft.x > o.topLeft.x ? topLeft.x : o.topLeft.x,
              topLeft.y > o.topLeft.y ? topLeft.y : o.topLeft.y,
                 0, 0); // size but we owerwrite bottomRight on next two lines
   res.bottomRight.x = bottomRight.x > o.bottomRight.x ? o.bottomRight.x : bottomRight.x;
   res.bottomRight.y = bottomRight.y > o.bottomRight.y ? o.bottomRight.y : bottomRight.y;
   return res;
}

ConnectionBase::ConnectionBase(const AQString &signalName)
   : m_signalName(signalName)
{
}

void ConnectionBase::registerConnection(AQObject *sender, ConnectionBase *c)
{
   sender->registerConnection(c);
}

AQObject::AQObject(AQObject *parent)
{
   if (parent)
      parent->m_children.push_back(this);

   m_parent = parent;
}

AQObject::~AQObject()
{
   if (m_parent)
      m_parent->m_children.erase(std::remove(m_parent->m_children.begin(), m_parent->m_children.end(), this), m_parent->m_children.end());

   // delete our own children in a safe way as the child destructor will manipulate our vector
   while (m_children.size())
      delete m_children.back();
}

void AQObject::setParent(AQObject *p)
{
   if (m_parent)      
      m_parent->m_children.erase(std::remove(m_parent->m_children.begin(), m_parent->m_children.end(), this), m_parent->m_children.end());

   if (p)
      p->m_children.push_back(this);

   m_parent = p;
}


void AQObject::emit(const AQString &signalName) {
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *c = m_connections[i];
      if (c->m_signalName == signalName)
         c->invoke();
   }
}
void AQObject::emit(const AQString &signalName, int arg) {
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *c = m_connections[i];
      if (c->m_signalName == signalName)
         c->invoke(arg);
   }
}
void AQObject::emit(const AQString &signalName, AQObject *arg) {
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *c = m_connections[i];
      if (c->m_signalName == signalName)
         c->invoke(arg);
   }
}

void AQObject::registerConnection(ConnectionBase *cb)
{
   m_connections.push_back(cb);
}

AQApplication *AQApplication::s_aqApp = nullptr;

AQApplication::AQApplication()
   : m_closing(false)
   , m_focusWidget(nullptr)
   , m_hoveredWidget(nullptr)
   , m_loopDepth(0)
   , m_clipboard(new AQClipboard)
{
   // Open all libraries:
   GfxBase = (struct GfxBase *) OpenLibrary((UBYTE*)"graphics.library", 39L);
   IntuitionBase = (struct IntuitionBase *) OpenLibrary((UBYTE*)"intuition.library", 39L);
   LayersBase = OpenLibrary((UBYTE*)"layers.library", 39L);
   GadToolsBase = OpenLibrary((UBYTE*)"gadtools.library", 39L);
   DOSBase = OpenLibrary((UBYTE*)"dos.library", 39L);
   IconBase = OpenLibrary((UBYTE*)"icon.library", 39L);

   if (LayersBase == nullptr)
      exit(20);
	s_aqApp = this;

   m_userPort = CreateMsgPort();
}

void AQApplication::setFocus(AQWidget *w)
{
   if (m_focusWidget)
      m_focusWidget->focusOutEvent();

   m_focusWidget = w;

   if (m_focusWidget)
      m_focusWidget->focusInEvent();
}

bool AQApplication::isWindowBlocked(AQWindow *window) const
{
   return m_modalWindows.size() > 0 && (m_modalWindows.front() != window);
}

void AQApplication::exec()
{
   bool stayAlive = true;
   processEvents(stayAlive);
}

void AQApplication::processEvents(bool &stayAlive)
{
   IntuiMessage *imsg;

   while (stayAlive & !m_closing) {
      Wait (1 << m_userPort->mp_SigBit);
      /* NOTE:  If you use GadTools gadgets, you must use GT_GetIMsg()
       * and GT_ReplyIMsg() instead of GetMsg() and ReplyMsg().
       * Regular GetMsg() and ReplyMsg() are safe if the only part
       * of GadTools you use are menus...
       */
      while (stayAlive && (!m_closing) && (imsg = (IntuiMessage *)GetMsg(m_userPort))) {
         IntuiMessage imsgCopy = *imsg;
         ReplyMsg((Message *)imsg);

         if (m_modalWindows.size() && imsgCopy.Class == IDCMP_ACTIVEWINDOW
                && imsgCopy.IDCMPWindow != m_modalWindows[m_modalWindows.size()-1]->m_window) {
            ActivateWindow(m_modalWindows[m_modalWindows.size()-1]->m_window);
            WindowToFront(m_modalWindows[m_modalWindows.size()-1]->m_window);
         }

         if (imsgCopy.Class == IDCMP_VANILLAKEY || imsgCopy.Class == IDCMP_RAWKEY) {
            bool matched = false;
            for (int i= 0; i < m_actions.size(); ++i) {
               matched = m_actions[i]->matchShortcut(imsgCopy);

               if (matched)
                  break;
            }
            if (matched)
               continue;
         }

         for (int i = 0; i < m_windows.size(); ++i) {
            if (imsgCopy.IDCMPWindow == m_windows[i]->m_window) {
               m_windows[i]->event(imsgCopy);
               break;
            }
         }
      }
   }
}

void AQApplication::registerWindow(AQWindow *window)
{
   m_windows.push_back(window);
}

void AQApplication::registerModalWindow(AQWindow *window)
{
   registerWindow(window);

   m_modalWindows.push_back(window);
}

void AQApplication::unregisterWindow(AQWindow *window)
{
  m_modalWindows.erase(std::remove(m_modalWindows.begin(), m_modalWindows.end(), window), m_modalWindows.end());

  m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());
}

void AQApplication::addAction(AQAction *a)
{
   m_actions.push_back(a);
}

MsgPort *AQApplication::userPort() const
{
   return m_userPort;
} 

AQWidget *AQApplication::focusWidget() const
{
   return m_focusWidget;
}

void AQApplication::setHoveredWidget(AQWidget *w)
{
   if (w != m_hoveredWidget) {
      if (m_hoveredWidget)
         m_hoveredWidget->leaveEvent();

       m_hoveredWidget = w;

      if (m_hoveredWidget)
         m_hoveredWidget->enterEvent();
   }
}

AQWidget *AQApplication::hoveredWidget() const
{
   return m_hoveredWidget;
}

void AQApplication::exit(int cod)
{
  while (m_windows.size()) {
     m_windows[0]->hide();
  }

   m_closing = true;
}

AQApplication::~AQApplication()
{
   s_aqApp = nullptr;
   
   delete m_clipboard;

   DeleteMsgPort(m_userPort);

   if (IconBase) {
      CloseLibrary(IconBase);
   }

   if (GadToolsBase) {
      CloseLibrary(GadToolsBase);
   }

   if (IntuitionBase) {
      CloseLibrary((struct Library *)IntuitionBase);
   }

   if (LayersBase) {
      CloseLibrary(LayersBase);
   }

   if (GfxBase) {
      CloseLibrary((struct Library *)GfxBase);
   }

   if (DOSBase) {
      CloseLibrary(DOSBase);
   }
}


