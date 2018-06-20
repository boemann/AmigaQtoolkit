#include "AQTabBar.h"

#include <AQLayout.h>
#include <AQButton.h>
#include <AQApplication.h>

AQTabBar::AQTabBar(AQWidget *parent)
   : AQWidget(parent)
{
   setBgPen(-1); // no bg but we draw ourself

   setExpanding(true, false);

   m_layout = new AQLayout(true);
   setLayout(m_layout);
}

AQTabBar::~AQTabBar()
{
}

int AQTabBar::count() const
{
   return m_tabs.size();
}

void AQTabBar::addTab(const AQString &text)
{
   for (int i= 0; i < m_tabs.size(); ++i)
      m_tabs[i]->setChecked(false);

   AQButton *button = new AQButton(true, this);

   button->setText(text);
   button->setCheckable(true);
   button->setChecked(true);
   button->setShowSubButton();
   Connect<AQTabBar>(button, "clicked", this, &AQTabBar::onTabClicked);
   Connect<AQTabBar>(button, "subClicked", this, &AQTabBar::onTabCloseClicked);

   m_tabs.push_back(button);

   m_layout->addWidget(button);

   updateGeometry();
}

void AQTabBar::removeTab(int index)
{
   m_tabs[index]->hide();
   m_tabs.erase(m_tabs.begin() + index);

// TODO should delete it - but currently that wont't remove from layout
}

void AQTabBar::setTabToolTip(int index, const AQString &tip)
{
   if (index >= m_tabs.size())
      return;

   m_tabs[index]->setToolTip(tip);
}

AQString AQTabBar::tabToolTip(int index) const
{
   if (index >= m_tabs.size())
      return AQString();

   return m_tabs[index]->toolTip();
}

void AQTabBar::setCurrentIndex(int index)
{
   if (index >= m_tabs.size())
      return;

   if (m_tabs[index]->isChecked())
      return;

   for (int i= 0; i < m_tabs.size(); ++i)
      m_tabs[i]->setChecked(i == index);
}


void AQTabBar::onTabClicked(bool on)
{
   AQButton *button = dynamic_cast<AQButton *>(aqApp->latestSignalSender);

   if (!button)
      return;

   if (!on) {
      button->setChecked(true);
      return;
   }

   for (int i= 0; i < m_tabs.size(); ++i) {
      AQButton *tab = m_tabs[i];
      if (tab == button)
         emit("currentChanged", i);
      else
         tab->setChecked(false);
   }
}

void AQTabBar::onTabCloseClicked()
{
   AQButton *button = dynamic_cast<AQButton *>(aqApp->latestSignalSender);

   if (!button)
      return;

   for (int i= 0; i < m_tabs.size(); ++i) {
      if (m_tabs[i] == button)
         emit("tabCloseClicked", i);
   }
}
