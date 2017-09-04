#include <AQScrollBar.h>
#include <AQDialog.h>
#include <AQTextEdit.h>
#include <AQTextDoc.h>
#include <AQLayout.h>
#include <AQListView.h>
#include <AQApplication.h>
#include <AQAction.h>
#include <AQStatusBar.h>
#include <AQLabel.h>

#include <proto/dos.h>

#include "Project.h"
#include "DevStudio.h"

struct DocInfo
{
   DocInfo() {}
   DocInfo(DevStudio *studio, class AQString &path);
   AQTextDoc *doc;
   AQTextCursor *cursor;
   AQPoint offset;
};

DocInfo::DocInfo(DevStudio *studio, class AQString &path)
{
   doc = new AQTextDoc(studio);
   doc->loadFile(path);
   cursor = new AQTextCursor(*doc);
   Connect<DevStudio>(doc, "modificationChanged", studio, &DevStudio::onDocModificationChanged);
   Connect<DevStudio>(doc, "cursorPositionChanged", studio, &DevStudio::onCursorPositionChanged);
}

DevStudio::DevStudio()
   : AQMainWindow()
   , m_project(nullptr)
{
   setExpanding(true, true);
   setMinimumSize(AQPoint(60, 40));
   setPos(AQPoint(0, 20));
   setPreferredSize(AQPoint(640, 492));

   AQAction *openAction = new AQAction(this);
   openAction->setShortcut("Amiga+O");
   openAction->setText("Open...");
   Connect<DevStudio>(openAction, "triggered", this, &DevStudio::openFile);
   aqApp->addAction(openAction);

   AQAction *saveAction = new AQAction(this);
   saveAction->setShortcut("Amiga+S");
   saveAction->setText("Save");
   Connect<DevStudio>(saveAction, "triggered", this, &DevStudio::saveFile);
   aqApp->addAction(saveAction);

   AQAction *saveAsAction = new AQAction(this);
   saveAsAction->setShortcut("Shift+Amiga+S");
   saveAsAction->setText("Save As...");
   Connect<DevStudio>(saveAsAction, "triggered", this, &DevStudio::saveFileAs);
   aqApp->addAction(saveAsAction);

   AQAction *saveAllAction = new AQAction(this);
   saveAllAction->setShortcut("Altt+Amiga+S");
   saveAllAction->setText("Save All");
   Connect<DevStudio>(saveAllAction, "triggered", this, &DevStudio::saveAll);
   aqApp->addAction(saveAllAction);

   AQAction *runAction = new AQAction(this);
   runAction->setShortcut("F5");
   runAction->setText("Run Debug");
   Connect<DevStudio>(runAction, "triggered", this, &DevStudio::run);
   aqApp->addAction(runAction);

   AQAction *buildProjectAction = new AQAction(this);
   buildProjectAction->setShortcut("F7");
   buildProjectAction->setText("Build Project");
   Connect<DevStudio>(buildProjectAction, "triggered", this, &DevStudio::buildProject);
   aqApp->addAction(buildProjectAction);

   setWindowTitle("Amiga Development Studio");

   m_textEdit = new AQTextEdit();
   setCentralWidget(m_textEdit);


   m_projectView = new AQListView();
   setLeftSideBar(m_projectView);
   Connect<DevStudio>(m_projectView, "itemDoubleClicked", this, &DevStudio::onFileItemDoubleClicked);

   m_textEdit->setFocus();
   m_currentDoc = new DocInfo();
   m_currentDoc->doc = m_textEdit->document();

   m_positionLabel = new AQLabel();
   m_positionLabel->setText("Ln:    Col:    ");
   statusBar()->addPermanentWidget(m_positionLabel);
   openProject("Work:devel/AQ");

   Connect<DevStudio>(aqApp, "readFinished", this, &DevStudio::onReadFinished);
}

void DevStudio::onReadFinished()
{
   m_currentDoc->cursor->insertText(m_pipeBuffer);
   m_currentDoc->cursor->insertText("ReadFinished");
   m_textEdit->update();
   Close(m_pipeFh);
}

DevStudio::~DevStudio()
{
   delete m_project;
}

void DevStudio::openProject(const AQString &projectPath)
{
   if (m_project)
      delete m_project;

   m_project = new Project(projectPath);

   m_projectView->clear();

   for (int i = 0; i < m_project->filesCount(); ++i) {
      AQListItem *sourceItem= new AQListItem(m_projectView);
      sourceItem->setText(0, m_project->filename(i));
      sourceItem->setText(1, m_project->projectPath() + "/" + m_project->filename(i));
      m_projectView->addTopLevelItem(sourceItem);
   }
}

void DevStudio::openFile()
{
   AQDialog *dialog = new AQDialog(AQDialog::OpenButton | AQDialog::CancelButton
                  | AQDialog::SelectionName);
   dialog->setWindowTitle("Open File");
   if (dialog->exec()) {
      m_currentDoc->offset.y = m_textEdit->verticalScrollBar()->value();
      AQString path(dialog->selectedPath());
      m_currentDoc = new DocInfo(this, path);
      m_loadedDocs[path] = m_currentDoc;

      m_textEdit->setDocument(m_currentDoc->doc, m_currentDoc->cursor);
      m_textEdit->verticalScrollBar()->setValue(0);

      // FIXME for now insert into project tree - once we have files tab place it there instead
      AQListItem *sourceItem= new AQListItem(m_projectView);
      sourceItem->setText(0, path);
      sourceItem->setText(1, path);
      m_projectView->addTopLevelItem(sourceItem);
   }
   delete dialog;
}

void DevStudio::saveFile()
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   while (it != m_loadedDocs.end()) {
      if (it->second->doc == m_textEdit->document()) {
         m_textEdit->document()->saveFile(it->first);
         return;
      }
      it++;
   }
   saveFileAs();
}

void DevStudio::saveFileAs()
{
   AQDialog dialog(AQDialog::SaveButton | AQDialog::CancelButton
                  | AQDialog::SelectionName);
   dialog.setWindowTitle("Save File As");
   if (dialog.exec()) {
      m_textEdit->document()->saveFile(dialog.selectedPath());
   }
}

void DevStudio::saveAll()
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   AQString filename;
   while (it != m_loadedDocs.end()) {
      if (it->second->doc->isModified())
         it->second->doc->saveFile(it->first);

      it->second->doc->setModified(false);

      it++;
   }
}

void DevStudio::run()
{
   //m_project->run();
   m_currentDoc->cursor->insertText("StartRead");
   m_textEdit->update();
   m_pipeFh =  Open("PIPE:adsbuild", MODE_OLDFILE);
   if (m_pipeFh != -1) {
      m_pipeBuffer = new char[200];
      aqApp->startAsyncRead(m_pipeFh, m_pipeBuffer, 199);
   }
}

void DevStudio::buildProject()
{
   saveAll();
   m_project->build();
}


void DevStudio::onFileItemDoubleClicked(AQObject *obj)
{
   AQListItem *item = (AQListItem *)(obj);
   AQString path(item->text(1));

   m_currentDoc->offset.y = m_textEdit->verticalScrollBar()->value();

   if (m_loadedDocs.find(path) == m_loadedDocs.end()) {
      m_currentDoc = new DocInfo(this, path);
      m_loadedDocs[path] = m_currentDoc;
   } else
      m_currentDoc = m_loadedDocs[path];

   m_textEdit->setDocument(m_currentDoc->doc, m_currentDoc->cursor);
   m_textEdit->verticalScrollBar()->setValue(m_currentDoc->offset.y);
   m_textEdit->setFocus();
   onCursorPositionChanged(m_currentDoc->doc);
}

void DevStudio::onDocModificationChanged(AQObject *obj)
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   AQString filename;
   while (it != m_loadedDocs.end()) {
      if (it->second->doc == obj) {
         filename = it->first;
         break;
      }
      it++;
   }
}

void DevStudio::onCursorPositionChanged(AQObject *obj)
{
   AQTextDoc *doc = static_cast<AQTextDoc *>(obj);
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();

   while (it != m_loadedDocs.end()) {
      if (it->second->doc == doc) {
         AQTextCursor *cursor = it->second->cursor;

         AQString t("Ln: ");
         t += AQString::number(1 + doc->blockNumber(cursor->position()));
         t += " Col: ";
         t += AQString::number(1 + cursor->positionInBlock());
         m_positionLabel->setText(t);
         return;
      }
      it++;
   }
}
