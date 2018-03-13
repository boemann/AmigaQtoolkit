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
#include <AQMenu.h>

#include <proto/dos.h>

#include "Project.h"
#include "DevStudio.h"

#include <stdio.h>

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
   , m_incompleteAddedOutput(nullptr)
{
   setExpanding(true, true);
   setMinimumSize(AQPoint(60, 40));
   setPos(AQPoint(0, 20));

   AQAction *newAction = new AQAction(this);
   newAction->setShortcut("Amiga+N");
   newAction->setText("New...");
   Connect<DevStudio>(newAction, "triggered", this, &DevStudio::openFile);
   aqApp->addAction(newAction);

   AQAction *openAction = new AQAction(this);
   openAction->setShortcut("Amiga+O");
   openAction->setText("Open File...");
   Connect<DevStudio>(openAction, "triggered", this, &DevStudio::openFile);
   aqApp->addAction(openAction);

   AQAction *openProjectAction = new AQAction(this);
   openProjectAction->setShortcut("Shift+Amiga+O");
   openProjectAction->setText("Open Project...");
   Connect<DevStudio>(openProjectAction, "triggered", this, &DevStudio::openProject);
   aqApp->addAction(openProjectAction);

   AQAction *saveAction = new AQAction(this);
   saveAction->setShortcut("Amiga+S");
   saveAction->setText("Save File");
   Connect<DevStudio>(saveAction, "triggered", this, &DevStudio::saveFile);
   aqApp->addAction(saveAction);

   AQAction *saveAsAction = new AQAction(this);
   saveAsAction->setShortcut("Shift+Amiga+S");
   saveAsAction->setText("Save File As...");
   Connect<DevStudio>(saveAsAction, "triggered", this, &DevStudio::saveFileAs);
   aqApp->addAction(saveAsAction);

   AQAction *saveAllAction = new AQAction(this);
   saveAllAction->setShortcut("Alt+Amiga+S");
   saveAllAction->setText("Save All");
   Connect<DevStudio>(saveAllAction, "triggered", this, &DevStudio::saveAll);
   aqApp->addAction(saveAllAction);

   AQAction *quitAction = new AQAction(this);
   quitAction->setShortcut("Amiga+Q");
   quitAction->setText("Quit");
   Connect<DevStudio>(quitAction, "triggered", this, &DevStudio::closeEvent);
   aqApp->addAction(quitAction);

   AQAction *runAction = new AQAction(this);
   runAction->setShortcut("F5");
   runAction->setText("Run Debug");
   Connect<DevStudio>(runAction, "triggered", this, &DevStudio::run);
   aqApp->addAction(runAction);

   AQAction *buildProjectAction = new AQAction(this);
   buildProjectAction->setShortcut("F6");
   buildProjectAction->setText("Build Project");
   Connect<DevStudio>(buildProjectAction, "triggered", this, &DevStudio::buildProject);
   aqApp->addAction(buildProjectAction);

   setWindowTitle("Amiga Development Studio");

   m_textEdit = new AQTextEdit();
   m_textEdit->setPreferredSize(AQPoint(640, 400));
   setCentralWidget(m_textEdit);


   m_projectView = new AQListView();
   setLeftSideBar(m_projectView);
   Connect<DevStudio>(m_projectView, "itemDoubleClicked", this, &DevStudio::onFileItemDoubleClicked);

   m_outputView = new AQListView();
   m_outputView->setTreeMode(false);
   m_outputView->setWordWrap(true);
   setBottomSideBar(m_outputView);

   m_textEdit->setFocus();
   m_currentDoc = new DocInfo();
   m_currentDoc->doc = m_textEdit->document();

   m_positionLabel = new AQLabel();
   m_positionLabel->setText("Ln:    Col:    ");
   statusBar()->addPermanentWidget(m_positionLabel);
   openProject("Work:devel/AQ");

   Connect<DevStudio>(aqApp, "readFinished", this, &DevStudio::onReadFinished);
   m_pipeFh =  Open("PIPE:adsbuild", MODE_OLDFILE);
   if (m_pipeFh != -1) {
      m_pipeBuffer = new char[600];
      aqApp->startAsyncRead(m_pipeFh, m_pipeBuffer, 599);
   }

   AQMenu *menubar = new AQMenu();
   menubar->setMenubarMode(true);

   AQMenu *projectMenu = new AQMenu("Project");
   projectMenu->addAction(newAction);
   projectMenu->addAction(openAction);
   projectMenu->addAction(openProjectAction);
   projectMenu->addSeparator();
   projectMenu->addAction(saveAction);
   projectMenu->addAction(saveAsAction);
   projectMenu->addAction(saveAllAction);
   projectMenu->addSeparator();
   projectMenu->addAction(quitAction);
   menubar->addMenu(projectMenu);

   AQMenu *editMenu = new AQMenu("Edit");
   menubar->addMenu(editMenu);

   AQMenu *buildMenu = new AQMenu("Build");
   buildMenu->addAction(buildProjectAction);
   menubar->addMenu(buildMenu);

   AQMenu *debugMenu = new AQMenu("Debug");
   debugMenu->addAction(runAction);
   menubar->addMenu(debugMenu);

   setMenu(menubar);

   setPreferredSize(AQPoint(640, 496));
}

void DevStudio::onReadFinished()
{
   int i = 0;
   int start = 0;
   while (m_pipeBuffer[i] != 0) {
      while (m_pipeBuffer[i] != 0 && m_pipeBuffer[i] != '\n')
         ++i;

      AQListItem *outputItem;
      AQString text;
      if (m_incompleteAddedOutput) {
         outputItem = m_incompleteAddedOutput;
         text = outputItem->text(0);
      } else {
         outputItem = new AQListItem(m_outputView);
         text = AQString(m_pipeBuffer + start, i-start);
         m_outputView->addTopLevelItem(outputItem);
      }
      outputItem->setText(0, text);

      if (m_pipeBuffer[i] == '\n') {
         m_incompleteAddedOutput = nullptr;
         ++i;
         start = i;
      } else {
         m_incompleteAddedOutput = outputItem;
      }

      if (text.endsWith("stopasyncpipe"))
         return;
   }

   Close(m_pipeFh);
   m_pipeFh =  Open("PIPE:adsbuild", MODE_OLDFILE);
   aqApp->startAsyncRead(m_pipeFh, m_pipeBuffer, 599);
   m_outputView->update();
}

DevStudio::~DevStudio()
{
   delete m_project;

   Close(m_pipeFh);
}

void DevStudio::openProject()
{
   AQDialog *dialog = new AQDialog(AQDialog::OpenButton | AQDialog::CancelButton
                  | AQDialog::SelectionName | AQDialog::DrawerMode);
   dialog->setWindowTitle("Open Project");
   if (m_project)
      dialog->setDrawer(m_project->projectPath());

   if (dialog->exec()) {
      openProject(dialog->selectedPath());
   }
   delete dialog;
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
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   while (it != m_loadedDocs.end()) {
      if (it->second->doc == m_textEdit->document()) {
         dialog->setDrawer(qCdUp(it->first));
         break;
      }
      it++;
   }
   if (it == m_loadedDocs.end())
      dialog->setDrawer(m_project->projectPath());

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
   m_incompleteAddedOutput = nullptr;
   m_outputView->clear();
   m_project->run();
}

void DevStudio::buildProject()
{
   saveAll();
   m_incompleteAddedOutput = nullptr;
   m_outputView->clear();
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
