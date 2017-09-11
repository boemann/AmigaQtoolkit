#ifndef AQMAINWINDOW_H
#define AQMAINWINDOW_H

#include <AQWidget.h>

class AQSplitter;
class AQStatusBar;

class AQMainWindow : public AQWidget
{
public:
   AQMainWindow();
   ~AQMainWindow();

   void setCentralWidget(AQWidget *w);
   void setLeftSideBar(AQWidget *);
   void setBottomSideBar(AQWidget *);
   AQStatusBar *statusBar();

private:
   void setupLayout();
   AQWidget *setupVerticalPart();

   AQWidget *m_centralWidget;
   AQWidget *m_leftSideBar;
   AQWidget *m_bottomSideBar;
   AQSplitter *m_hSplitter;
   AQSplitter *m_vSplitter;
   AQStatusBar *m_statusBar;
};
   
#endif
