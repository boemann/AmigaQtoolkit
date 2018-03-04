#include "AQDialog.h"

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

AQDialog::AQDialog(UWORD features, AQWidget *parent)
   : AQWidget(parent)
   , m_selectName(nullptr)
   , m_drawerMode(features & DrawerMode)
   , m_drawerIsSet(false)
{
   setBgPen(-2); // no bg and only subwidgets

   AQLayout *overallLayout = new AQLayout(false);
   AQLayout *locationLayout = new AQLayout(true);
   AQLayout *fileLayout = new AQLayout(true);
   AQLayout *buttonLayout = new AQLayout(true);
   AQSplitter *splitter = new AQSplitter(true, this);

   buttonLayout->addItem(new AQSpacerItem(AQPoint(0,0),true,false));
   if (features & SaveButton) {
      AQButton *b = new AQButton(false, this);
      b->setText("Save");
      Connect<AQDialog>(b, "clicked", this, &AQDialog::accept);
      buttonLayout->addWidget(b);
   }
   if (features & OpenButton) {
      AQButton *b = new AQButton(false, this);
      if (m_drawerMode)
         b->setText("Select drawer");
      else
         b->setText("Open");
      Connect<AQDialog>(b, "clicked", this, &AQDialog::accept);
      buttonLayout->addWidget(b);
   }
   if (features & CancelButton) {
      AQButton *b = new AQButton(false, this);
      b->setText("Cancel");
      Connect<AQDialog>(b, "clicked", this, &AQDialog::dismiss);
      buttonLayout->addWidget(b);
   }
   
   AQButton *b = new AQButton(true, this);
   b->setIcon(AQIcon("back"));
   locationLayout->addWidget(b);
   b = new AQButton(true, this);
   b->setIcon(AQIcon("forward"));
   locationLayout->addWidget(b);
   b = new AQButton(true, this);
   b->setIcon(AQIcon("levelup"));
   locationLayout->addWidget(b);
   Connect<AQDialog>(b, "clicked", this, &AQDialog::onCdUp);
   m_locationName = new AQLineEdit(this);
   locationLayout->addWidget(m_locationName);
   m_drawerView = new AQListView();
   m_drawerView->setExpanding(false, true);
   Connect<AQDialog>(m_drawerView, "itemActivated", this, &AQDialog::onItemActivated);
   Connect<AQDialog>(m_drawerView, "itemExpanded", this, &AQDialog::onItemExpanded);

   m_filesView = new AQListView();
   m_filesView->setTreeMode(false);
   Connect<AQDialog>(m_filesView, "itemActivated", this, &AQDialog::onFileItemActivated);
   Connect<AQDialog>(m_filesView, "itemDoubleClicked", this, &AQDialog::onFileItemDoubleClicked);

   if (features & SelectionName) {
      AQLabel *label = new AQLabel(this);
      if (m_drawerMode)
         label->setText("Drawer:");
      else
         label->setText("File name:");
      fileLayout->addWidget(label);
      m_selectName  = new AQLineEdit(this);
      fileLayout->addWidget(m_selectName);
   }
   overallLayout->addItem(locationLayout);
   splitter->setWidgets(m_drawerView, m_filesView);
   overallLayout->addWidget(splitter);
   if (fileLayout->count())
      overallLayout->addItem(fileLayout);
   else
      delete fileLayout;

   if (buttonLayout->count())
      overallLayout->addItem(buttonLayout);
   else
      delete buttonLayout;

   setSize(AQPoint(400,300));

   populateDrawerView();

   setLayout(overallLayout);
}

AQDialog::~AQDialog()
{

}

int AQDialog::exec()
{
   setWindowModality(2);
   show();
   m_visible = true;
   aqApp->processEvents(m_visible);

   return m_return;
}

void AQDialog::accept()
{
   m_return = 1;
   hide();
   m_visible = false;
}

void AQDialog::dismiss()
{
   m_return = 0;
   hide();
   m_visible = false;
}

void AQDialog::onCdUp()
{
   setDrawer(qCdUp(drawer()));
}

AQString AQDialog::drawer() const
{
   return m_locationName->text();
}

void AQDialog::setDrawer(const AQString &drawerPath)
{
   if (!qIsFolder(drawerPath)) {
      return;
   }

   m_filesView->clear();
   m_filesView->rootItem()->setText(1, drawerPath);

   populateDrawer(m_filesView->rootItem(), true);
   m_locationName->document()->setData(drawerPath);
   m_locationName->update();

   if (m_drawerMode) {
      m_selectName->document()->setData(qFileName(drawerPath));
      m_selectName->update();
   }
   m_drawerIsSet = true;
}

AQString AQDialog::selectedPath() const
{
   AQString p = drawer();

   if (!m_selectName)
      return p;

   if (m_drawerIsSet)
      p = qCdUp(p);

   if (p.endsWith(":") || p.isEmpty())
      return p + m_selectName->text();
   else
      return p + "/" + m_selectName->text();
}

void AQDialog::closeEvent()
{
   dismiss();
}

bool order(const AQListItem *lhs, const AQListItem *rhs)
{
   if ((lhs->text(2) == AQString("drawer")) != (rhs->text(2) == AQString("drawer")))
      return lhs->text(2) == AQString("drawer");

   return strcmp(lhs->text(0), rhs->text(0)) < 0;
}

void AQDialog::populateDrawer(AQListItem *drawerItem, bool alsoFiles)
{
   const int bufsize = 512;
   bool more;
   ExAllData *buffer = (ExAllData *)AllocMem(bufsize, MEMF_CLEAR);
   ExAllControl *eac = (ExAllControl *)AllocDosObject(DOS_EXALLCONTROL, nullptr);

   if (!eac)
      return;

   AQString path(drawerItem->text(1));

   BPTR lock = Lock(path, SHARED_LOCK);
   if (lock == nullptr) {
      FreeDosObject(DOS_EXALLCONTROL, eac);
      FreeMem(buffer, bufsize);
      return;
   }

   eac->eac_LastKey = 0;
   do {
       more = ExAll(lock, buffer, bufsize, ED_TYPE, eac);
       int err = IoErr();
       if ((!more) && (err != ERROR_NO_MORE_ENTRIES)) {
           break;
       }
       if (eac->eac_Entries == 0) {
           /* ExAll failed normally with no entries */
           continue;        /* ("more" is *usually* zero) */
       }
       ExAllData *ead = buffer;
       do {
           if (!AQString(ead->ed_Name).endsWith(".info"))
              if ((ead->ed_Type > 0) || alsoFiles) {
                 AQListItem *sub = new AQListItem(alsoFiles ? m_filesView : m_drawerView);
                 sub->setText(0, ead->ed_Name);
                 if (ead->ed_Type > 0) {
                    sub->setIcon(0, AQIcon("drawer"));
                    sub->setText(2, "drawer");
                 }
                 if (path[path.size()-1] == ':')
                    sub->setText(1, path + ead->ed_Name);
                 else
                    sub->setText(1, path + "/" + ead->ed_Name);
            
                 drawerItem->addChild(sub);
                 drawerItem->setExpanderPolicy(1);

                 if (ead->ed_Type > 0 && !alsoFiles)
                    sub->setExpanderPolicy(2);
              }

           ead = ead->ed_Next;
       } while (ead);

   } while (more);
   
   UnLock(lock);

   FreeDosObject(DOS_EXALLCONTROL, eac);
   FreeMem(buffer, bufsize);

   drawerItem->sortChildren(order);
}

void AQDialog::populateDrawerView()
{
   AQListItem *favorites= new AQListItem(m_drawerView);
   favorites->setText(0, "Favorites");
   m_drawerView->addTopLevelItem(favorites);

   AQListItem *volumes= new AQListItem(m_drawerView);
   volumes->setText(0, "The Amiga");
   volumes->setIcon(0, AQIcon("AmigaTick"));
   volumes->setExpanded(true);
   DosList *dl = LockDosList(LDF_VOLUMES | LDF_READ);
   while ((dl = NextDosEntry(dl, LDF_VOLUMES))) {
      AQListItem *volume= new AQListItem(m_drawerView);
      volume->setText(0, dl->dol_Name);
      volume->setIcon(0, AQIcon("drawer"));
      volume->setText(1, AQString(dl->dol_Name)+":");
      volume->setExpanderPolicy(2);
      volumes->addChild(volume);
   }

   UnLockDosList(LDF_VOLUMES | LDF_READ);

   m_drawerView->addTopLevelItem(volumes);
}

void AQDialog::onItemExpanded(AQObject *obj)
{
   AQListItem *item = (AQListItem *)(obj);
   if (item->childCount() == 0) {
      populateDrawer(item, false);
   }
}

void AQDialog::onItemActivated(AQObject *obj)
{
   AQListItem *item = (AQListItem *)(obj);

   setDrawer(item->text(1));
}

void AQDialog::onFileItemActivated(AQObject *obj)
{
   if (m_selectName) {
      AQListItem *item = (AQListItem *)(obj);
      if (!m_drawerMode != qIsFolder(m_filesView->rootItem()->text(1) + "/" + item->text(0))) {
         m_selectName->document()->setData(item->text(0));
         m_selectName->update();
         m_drawerIsSet = false;
      }
   }
}

void AQDialog::onFileItemDoubleClicked(AQObject *obj)
{
   AQListItem *item = (AQListItem *)(obj);
   AQString path(m_filesView->rootItem()->text(1));
   if (path.endsWith(":"))
      path += item->text(0);
   else
      path += "/" + item->text(0);

   if (qIsFolder(path)) {
      setDrawer(path);
   } else if (m_selectName)
      accept();
}

