#include "AQApplication.h"

#include <algorithm>

#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/layers.h>
#include <proto/dos.h>

#include <AQWindow.h>
#include <AQObject.h>
#include <AQString.h>
#include <AQClipboard.h>
#include <AQAction.h>
#include <AQToolTip.h>

using std::vector;

extern struct ExecBase *SysBase;
struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;
struct Library *LayersBase = nullptr;
//extern struct Library *DOSBase;
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

void ConnectionBase::unregisterConnection(AQObject *sender, const ConnectionBase &c)
{
   sender->unregisterConnection(c);
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
   aqApp->latestSignalSender = this;
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *c = m_connections[i];
      if (c->m_signalName == signalName)
         c->invoke();
   }
}
void AQObject::emit(const AQString &signalName, bool arg) {
   aqApp->latestSignalSender = this;
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *c = m_connections[i];
      if (c->m_signalName == signalName)
         c->invoke(arg);
   }
}
void AQObject::emit(const AQString &signalName, int arg) {
   aqApp->latestSignalSender = this;
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *c = m_connections[i];
      if (c->m_signalName == signalName)
         c->invoke(arg);
   }
}
void AQObject::emit(const AQString &signalName, AQObject *arg) {
   aqApp->latestSignalSender = this;
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

void AQObject::unregisterConnection(const ConnectionBase &cb)
{
   for (int i= 0; i < m_connections.size(); ++i) {
      ConnectionBase *toremove = m_connections[i];
      if (toremove->isEqual(cb)) {
         m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), toremove) , m_connections.end());
         delete toremove;
         return;
      }
   }
}

AQScreen::AQScreen()
   : m_screen(nullptr)
   , m_drawInfo(nullptr)
{
   m_screen = LockPubScreen(NULL);

   if (m_screen) {
      m_drawInfo = GetScreenDrawInfo(m_screen);
   }
}

AQScreen::~AQScreen()
{
   if (m_drawInfo) {
      FreeScreenDrawInfo(m_screen, m_drawInfo);
   }
   UnlockPubScreen(NULL, m_screen);
}

AQPoint AQScreen::menubarSize() const
{
   return AQPoint(m_screen->Width, m_screen->BarHeight+1);
}

TextFont *AQScreen::font()
{
   return m_drawInfo->dri_Font;
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
   DOSBase = (struct DosLibrary *)OpenLibrary((UBYTE*)"dos.library", 39L);
   IconBase = OpenLibrary((UBYTE*)"icon.library", 39L);

   if (LayersBase == nullptr)
      exit(20);

   m_defaultScreen = new AQScreen();

   s_aqApp = this;

   m_userPort = CreateMsgPort();

   m_asyncFilePort = CreateMsgPort();
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

void AQApplication::startAsyncRead(BPTR file, char *buffer, int size)
{
   struct FileHandle *fh = (struct FileHandle *)BADDR(file);
   struct DosPacket *packet;

   if (fh && fh->fh_Type)
   {
      if (packet = (struct DosPacket *)AllocDosObject (DOS_STDPKT,TAG_END))
      {
         packet->dp_Port = m_asyncFilePort;

         packet->dp_Type = ACTION_READ;
         packet->dp_Arg1 = fh->fh_Arg1;
         packet->dp_Arg2 = (LONG)buffer;
         packet->dp_Arg3 = (LONG)size;
         PutMsg (fh->fh_Type,packet->dp_Link);
      }
   }
}

void AQApplication::exec()
{
   bool stayAlive = true;
   processEvents(stayAlive);
}

void AQApplication::processEvents(bool &stayAlive)
{
   IntuiMessage *imsg;
   struct Message *asyncMsg;

   ULONG sigFlag = (1L << m_userPort->mp_SigBit)
             | (1L << m_asyncFilePort->mp_SigBit);

   while (stayAlive & !m_closing) {
      ULONG sigs = Wait (sigFlag);

      while (stayAlive && (!m_closing) && (imsg = (IntuiMessage *)GetMsg(m_userPort))) {
         IntuiMessage imsgCopy = *imsg;
         ReplyMsg((Message *)imsg);

         AQToolTip::filterIntuiMessage(imsgCopy);

         if (imsgCopy.Class == IDCMP_ACTIVEWINDOW) {
            if (m_popupWindows.size()) {
               continue;
            } else if (m_modalWindows.size())
               if (imsgCopy.IDCMPWindow != m_modalWindows[m_modalWindows.size()-1]->m_window) { {
                  ActivateWindow(m_modalWindows[m_modalWindows.size()-1]->m_window);
                  WindowToFront(m_modalWindows[m_modalWindows.size()-1]->m_window);
               }
            }
         } else if (imsgCopy.Class == IDCMP_INACTIVEWINDOW) {
            if (m_popupWindows.size()) {
               continue;
            }
         }

         if (imsgCopy.Class == IDCMP_VANILLAKEY || imsgCopy.Class == IDCMP_RAWKEY) {
            bool matched = false;
            AQWidget *receiver = aqApp->focusWidget();
            while (!matched && receiver) { // first check actions from focus and up
               for (int i= 0; !matched && i < receiver->m_actions.size(); ++i)
                  matched = receiver->m_actions[i]->matchShortcut(imsgCopy);

               if (receiver->isTopLevel())
                  receiver = nullptr;
               else {
                  receiver = receiver->parent();
               }
            }

            // application global actions
            if (!m_modalWindows.size())
               for (int i= 0; !matched && i < m_actions.size(); ++i)
                  matched = m_actions[i]->matchShortcut(imsgCopy);

            if (matched)
               continue;
         }


         if (imsgCopy.Class == IDCMP_INTUITICKS)
            for (int i = 0; i < m_windows.size(); ++i)
               m_windows[i]->paintDirty();

         for (int i = 0; i < m_windows.size(); ++i) {
            if (imsgCopy.IDCMPWindow == m_windows[i]->m_window) {
               m_windows[i]->event(imsgCopy);
               break;
            }
         }

         AQToolTip::checkShow(m_hoveredWidget);
      }
      while (stayAlive && (!m_closing) && (asyncMsg = GetMsg(m_asyncFilePort))) {
         struct DosPacket *packet;
         long rc;

         packet = (struct DosPacket *) asyncMsg->mn_Node.ln_Name;
         rc = packet->dp_Res1;
         char *buf = (char *)packet->dp_Arg2;
         buf [rc] = 0;
         FreeDosObject (DOS_STDPKT, packet);
         emit("readFinished");
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

void AQApplication::registerPopupWindow(AQWindow *window)
{
   registerWindow(window);

   m_popupWindows.push_back(window);
}

void AQApplication::unregisterWindow(AQWindow *window)
{
   m_modalWindows.erase(std::remove(m_modalWindows.begin(), m_modalWindows.end(), window), m_modalWindows.end());
   m_popupWindows.erase(std::remove(m_popupWindows.begin(), m_popupWindows.end(), window), m_popupWindows.end());

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

AQScreen *AQApplication::screen(const AQWidget *w) const
{
//For now all windows on defaultScreen
// Screen *s = w->m_window->m_window->WScreen;

   return m_defaultScreen;
}

AQWidget *AQApplication::focusWidget() const
{
   AQWidget *w = m_focusWidget;

   if (!w)
      return nullptr;

   while (!w->isTopLevel())
      w = w->parent();

   if (isWindowBlocked(w->m_window))
      return nullptr;

   return m_focusWidget;
}

void AQApplication::setHoveredWidget(AQWidget *w)
{
   if (w != m_hoveredWidget) {
      if (m_hoveredWidget)
         m_hoveredWidget->leaveEvent();

      m_hoveredWidget = w;

      
      AQToolTip::s_avaitReactivation = false;

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
   
   delete m_defaultScreen;
   delete m_clipboard;

   DeleteMsgPort(m_userPort);
   DeleteMsgPort(m_asyncFilePort);

   if (IconBase) {
      CloseLibrary(IconBase);
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
      CloseLibrary((struct Library *)DOSBase);
   }
}


