#ifndef AQTABBAR_H
#define AQTABBAR_H

#include <AQWidget.h>
#include <vector.h>

class AQLayout;
class AQButton;

class AQTabBar : public AQWidget
{
public:
   AQTabBar(AQWidget *parent = nullptr);
   ~AQTabBar();

   int count() const;

   void addTab(const AQString &text);
   void removeTab(int index);

   void setCurrentIndex(int index);

   void setTabToolTip(int index, const AQString &tooltip);
   AQString tabToolTip(int index) const;

private:
   void onTabClicked(bool on);
   void onTabCloseClicked();

   AQLayout *m_layout;

   vector<AQButton *> m_tabs;
};
   
#endif
