#ifndef AQSTATUSBAR_H
#define AQSTATUSBAR_H

#include <AQWidget.h>

class AQLayout;

class AQStatusBar : public AQWidget
{
public:
   AQStatusBar(AQWidget *parent = nullptr);
   ~AQStatusBar();

   void addWidget(AQWidget *w);
   void addPermanentWidget(AQWidget *w);

private:
   AQLayout *m_normLayout;
   AQLayout *m_permanentLayout;
};
   
#endif
