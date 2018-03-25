#ifndef AQLABEL_H
#define AQLABEL_H

#include <AQWidget.h>
#include <AQIcon.h>

class AQLabel : public AQWidget
{
public:
   AQLabel(AQWidget *parent = nullptr);
   ~AQLabel();

   void setText(const AQString &text, bool updateSize = true);
   void setIcon(const AQIcon &icon);

protected:
   virtual void paintEvent(RastPort *rp, const AQRect &rect);

private:
   AQString m_text;
   AQIcon m_icon;
};
   
#endif
