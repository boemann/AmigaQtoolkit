
#include <AQLayout.h>
#include <AQStatusBar.h>


AQStatusBar::AQStatusBar(AQWidget *parent)
   : AQWidget(parent)
{
   setBgPen(-2); // no bg but we draw ourself

   m_normLayout = new AQLayout(true);
   m_permanentLayout = new AQLayout(true);
   AQLayout *l = new AQLayout(true);
   l->addItem(m_normLayout);
   l->addItem(new AQSpacerItem(AQPoint(3,2),true, false));
   l->addItem(m_permanentLayout);
   setLayout(l);
}

AQStatusBar::~AQStatusBar()
{
}

void AQStatusBar::addWidget(AQWidget *w)
{
   w->setParent(this);
   m_normLayout->addWidget(w);
   w->updateGeometry();
}

void AQStatusBar::addPermanentWidget(AQWidget *w)
{
   w->setParent(this);
   m_permanentLayout->addWidget(w);
   w->updateGeometry();
}
