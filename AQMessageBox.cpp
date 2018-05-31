#include "AQMessageBox.h"

#include <stdio.h>

#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <dos/dos.h>
#include <dos/exall.h>

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/dos.h>

#include <AQLayout.h>
#include <AQWindow.h>
#include <AQButton.h>
#include <AQTextDoc.h>
#include <AQLineEdit.h>
#include <AQListView.h>
#include <AQScrollBar.h>
#include <AQLabel.h>
#include <AQSplitter.h>
#include <AQApplication.h>

AQMessageBox::AQMessageBox(UWORD features, AQWidget *parent)
   : AQWidget(parent)
{
   setBgPen(-2); // no bg and only subwidgets
   setWindowFlags(AQWindow::TitleBar);
   setWindowModality(2);

   AQLayout *overallLayout = new AQLayout(false);
   AQLayout *buttonLayout = new AQLayout(true);

   buttonLayout->addItem(new AQSpacerItem(AQPoint(0,0),true,false));
   if (features & OkButton) {
      AQButton *b = new AQButton(false, this);
      b->setText("Ok");
      Connect<AQMessageBox>(b, "clicked", this, &AQMessageBox::accept);
      buttonLayout->addWidget(b);
   }
   if (features & CancelButton) {
      AQButton *b = new AQButton(false, this);
      b->setText("Cancel");
      b->setToolTip("Close dialog without doing anything");
      Connect<AQMessageBox>(b, "clicked", this, &AQMessageBox::dismiss);
      buttonLayout->addWidget(b);
   }
   

   if (buttonLayout->count())
      overallLayout->addItem(buttonLayout);
   else
      delete buttonLayout;

   setPreferredSize(AQPoint(300,200));

   if (parent) {
      AQPoint p = parent->size();
      p.x /= 2;
      p.y /= 2;
      p.x -= size().x / 2;
      p.y -= size().y / 2;

      setPos(parent->pos() + p);
   }

   setLayout(overallLayout);
}

AQMessageBox::~AQMessageBox()
{

}

int AQMessageBox::exec()
{
   setWindowModality(2);
   show();
   m_visible = true;
   aqApp->processEvents(m_visible);

   return m_return;
}

void AQMessageBox::accept()
{
   m_return = 1;
   hide();
   m_visible = false;
}

void AQMessageBox::dismiss()
{
   m_return = 0;
   hide();
   m_visible = false;
}


void AQMessageBox::closeEvent()
{
   dismiss();
}

