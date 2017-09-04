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
   , m_rightSideBar(nullptr)
   , m_bottomSideBar(nullptr)
   , m_statusBar(nullptr)
{
   setBgPen(-2); // no bg and we defer all to children

   setExpanding(false, false);
   setMinimumSize(AQPoint(140, 120));

   m_hSplitter = new AQSplitter(true, this);

   AQLayout *l = new AQLayout(false);
   l->addWidget(m_hSplitter);
   setLayout(l);
}

AQMainWindow::~AQMainWindow()
{
}


void AQMainWindow::setCentralWidget(AQWidget *w)
{
   m_centralWidget = w;
   if (m_leftSideBar)
      m_hSplitter->setWidgets(m_leftSideBar, m_centralWidget);
}

void AQMainWindow::setLeftSideBar(AQWidget *w)
{
   m_leftSideBar = w;
   if (m_centralWidget)
      m_hSplitter->setWidgets(m_leftSideBar, m_centralWidget);
}

AQStatusBar *AQMainWindow::statusBar()
{
   if (!m_statusBar) {
      m_statusBar = new AQStatusBar(this);
      layout()->addWidget(m_statusBar);
   }
   return m_statusBar;
}