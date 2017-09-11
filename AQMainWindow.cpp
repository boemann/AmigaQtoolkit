#include "AQMainWindow.h"

#include <graphics/gfxbase.h>

#include <proto/graphics.h>

#include <AQSplitter.h>
#include <AQStatusBar.h>
#include <AQLayout.h>

AQMainWindow::AQMainWindow()
   : AQWidget(nullptr)
   , m_centralWidget(nullptr)
   , m_leftSideBar(nullptr)
   , m_bottomSideBar(nullptr)
   , m_statusBar(nullptr)
   , m_hSplitter(nullptr)
   , m_vSplitter(nullptr)
{
   setBgPen(-2); // no bg and we defer all to children

   setExpanding(false, false);
   setMinimumSize(AQPoint(140, 120));
}

AQMainWindow::~AQMainWindow()
{
}

AQWidget *AQMainWindow::setupVerticalPart()
{
   AQWidget *vertW = nullptr;

   if (m_centralWidget && m_bottomSideBar) {
      if (!m_vSplitter)
         m_vSplitter = new AQSplitter(false, m_hSplitter);
      m_vSplitter->setWidgets(m_centralWidget, m_bottomSideBar);
      vertW = m_vSplitter;
   } else {
      if (m_centralWidget)
         vertW = m_centralWidget;
      else
         vertW = m_bottomSideBar;

      if (vertW)
         vertW->setParent(this);

      delete m_vSplitter;
      m_vSplitter = nullptr;
   }

   return vertW;
}

void AQMainWindow::setupLayout()
{
   delete layout();
   setLayout(nullptr);

   AQLayout *l = new AQLayout(false);

   if (m_leftSideBar && (m_centralWidget || m_bottomSideBar)) {
      if (!m_hSplitter)
         m_hSplitter = new AQSplitter(true, this);

      AQWidget *rightSideW = setupVerticalPart();

      m_hSplitter->setWidgets(m_leftSideBar, rightSideW);
      l->addWidget(m_hSplitter);
   } else {
     delete m_hSplitter;
     m_hSplitter = nullptr;
   }

   if (m_statusBar)
      l->addWidget(m_statusBar);

   setLayout(l);
}

void AQMainWindow::setCentralWidget(AQWidget *w)
{
   if (m_centralWidget == w)
      return;
   m_centralWidget = w;
   setupLayout();
}

void AQMainWindow::setLeftSideBar(AQWidget *w)
{
   if (m_leftSideBar == w)
      return;
   m_leftSideBar = w;
   setupLayout();
}

void AQMainWindow::setBottomSideBar(AQWidget *w)
{
   if (m_bottomSideBar == w)
      return;
   m_bottomSideBar = w;
   setupLayout();
}

AQStatusBar *AQMainWindow::statusBar()
{
   if (!m_statusBar) {
      m_statusBar = new AQStatusBar(this);
   }
   setupLayout();
   return m_statusBar;
}