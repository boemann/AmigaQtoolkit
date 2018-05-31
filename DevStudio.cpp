#include <AQScrollBar.h>
#include <AQFileDialog.h>
#include <AQMessageBox.h>
#include <AQTextEdit.h>
#include <AQTextDoc.h>
#include <AQLayout.h>
#include <AQListView.h>
#include <AQApplication.h>
#include <AQAction.h>
#include <AQStatusBar.h>
#include <AQLabel.h>
#include <AQMenu.h>
#include <AQCommandStack.h>
#include <AQTabBar.h>
#include <AQClipboard.h>

#include <proto/dos.h>

#include "Project.h"
#include "DevStudio.h"
#include "FindWidget.h"

#include <stdio.h>

struct DocInfo
{
   DocInfo() {}
   DocInfo(DevStudio *studio, const AQString &path);
   AQTextDoc *doc;
   AQTextCursor *cursor;
   AQPoint offset;
   AQCommandStack *commandStack;
   int m_findReplaceMode; //0=no, 1=find, replace = 2

   void onCommandAvailable();
};

DocInfo::DocInfo(DevStudio *studio, const AQString &path)
{
   doc = new AQTextDoc(studio);
   doc->loadFile(path);
   cursor = new AQTextCursor(*doc);
   m_findReplaceMode = 0; // intiallly no find replace widget
   commandStack = new AQCommandStack();
   Connect<DocInfo>(doc, "commandAvailable", this, &DocInfo::onCommandAvailable);
   Connect<DevStudio>(doc, "cursorPositionChanged", studio, &DevStudio::onCursorPositionChanged);

   Connect<DevStudio>(commandStack, "actionTextsChanged", studio, &DevStudio::onUndoRedoActionTextsChanged);
}

void DocInfo::onCommandAvailable()
{
   commandStack->push(doc->takeLatestCommand());
}

DevStudio::DevStudio()
   : AQMainWindow()
   , m_project(nullptr)
   , m_incompleteAddedOutput(nullptr)
   , m_pipeFh(nullptr)
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

   AQAction *closeAction = new AQAction(this);
   closeAction->setShortcut("Amiga+W");
   closeAction->setText("Close File");
   Connect<DevStudio>(closeAction, "triggered", this, &DevStudio::closeFile);
   aqApp->addAction(closeAction);

   AQAction *quitAction = new AQAction(this);
   quitAction->setShortcut("Amiga+Q");
   quitAction->setText("Quit");
   Connect<DevStudio>(quitAction, "triggered", this, &DevStudio::closeEvent);
   aqApp->addAction(quitAction);

   m_undoAction = new AQAction(this);
   m_undoAction->setShortcut("Amiga+Z");
   m_undoAction->setText("Undo");
   Connect<DevStudio>(m_undoAction, "triggered", this, &DevStudio::onUndo);
   aqApp->addAction(m_undoAction);

   m_redoAction = new AQAction(this);
   m_redoAction->setShortcut("Shift+Amiga+Z");
   m_redoAction->setText("Redo");
   Connect<DevStudio>(m_redoAction, "triggered", this, &DevStudio::onRedo);
   aqApp->addAction(m_redoAction);

   m_cutAction = new AQAction(this);
   m_cutAction->setShortcut("Amiga+X");
   m_cutAction->setText("Cut");
   Connect<DevStudio>(m_cutAction, "triggered", this, &DevStudio::onCut);
   aqApp->addAction(m_cutAction);

   m_copyAction = new AQAction(this);
   m_copyAction->setShortcut("Amiga+C");
   m_copyAction->setText("Copy");
   Connect<DevStudio>(m_copyAction, "triggered", this, &DevStudio::onCopy);
   aqApp->addAction(m_copyAction);

   m_pasteAction = new AQAction(this);
   m_pasteAction->setShortcut("Amiga+V");
   m_pasteAction->setText("Paste");
   Connect<DevStudio>(m_pasteAction, "triggered", this, &DevStudio::onPaste);
   Connect<AQAction>(aqApp->clipboard(), "changed", m_pasteAction, &AQAction::setEnabled);
   aqApp->addAction(m_pasteAction);

   AQAction *clearFindAction = new AQAction(this);
   clearFindAction->setShortcut("Esc");
   aqApp->addAction(clearFindAction);

   m_findAction = new AQAction(this);
   m_findAction->setShortcut("Amiga+F");
   m_findAction->setText("Find...");
   aqApp->addAction(m_findAction);

   m_replaceAction = new AQAction(this);
   m_replaceAction->setShortcut("Amiga+H");
   m_replaceAction->setText("Replace...");
   aqApp->addAction(m_replaceAction);

   AQAction *runAction = new AQAction(this);
   runAction->setShortcut("F5");
   runAction->setText("Run Debug");
   Connect<DevStudio>(runAction, "triggered", this, &DevStudio::onRun);
   aqApp->addAction(runAction);

   AQAction *buildProjectAction = new AQAction(this);
   buildProjectAction->setShortcut("F6");
   buildProjectAction->setText("Build Project");
   Connect<DevStudio>(buildProjectAction, "triggered", this, &DevStudio::onBuildProject);
   aqApp->addAction(buildProjectAction);

   AQAction *cleanProjectAction = new AQAction(this);
   cleanProjectAction->setText("Clean Project");
   Connect<DevStudio>(cleanProjectAction, "triggered", this, &DevStudio::onCleanProject);
   aqApp->addAction(cleanProjectAction);

   AQAction *releaseConfigurationAction = new AQAction(this);
   releaseConfigurationAction->setText("Release");
   releaseConfigurationAction->setCheckable(true);
   Connect<DevStudio>(releaseConfigurationAction, "triggered", this, &DevStudio::onReleaseConfiguration);
   aqApp->addAction(releaseConfigurationAction);

   AQAction *debugConfigurationAction = new AQAction(this);
   debugConfigurationAction->setText("Debug");
   debugConfigurationAction->setCheckable(true);
   debugConfigurationAction->joinMutexWith(releaseConfigurationAction);
   releaseConfigurationAction->setChecked(true);
   Connect<DevStudio>(debugConfigurationAction, "triggered", this, &DevStudio::onDebugConfiguration);
   aqApp->addAction(debugConfigurationAction);

   AQAction *prevMessageAction = new AQAction(this);
   prevMessageAction->setShortcut("Shift+F8");
   prevMessageAction->setText("Previous message");
   Connect<DevStudio>(prevMessageAction, "triggered", this, &DevStudio::onPrevMessage);
   aqApp->addAction(prevMessageAction);

   AQAction *nextMessageAction = new AQAction(this);
   nextMessageAction->setShortcut("F8");
   nextMessageAction->setText("Next message");
   Connect<DevStudio>(nextMessageAction, "triggered", this, &DevStudio::onNextMessage);
   aqApp->addAction(nextMessageAction);

   setWindowTitle("Amiga Development Studio");

   AQWidget *central = new AQWidget(this);
   AQLayout *centralL = new AQLayout(false);

   m_tabBar = new AQTabBar(central);
   centralL->addWidget(m_tabBar);
   Connect<DevStudio>(m_tabBar, "currentChanged", this, &DevStudio::onCurrentTabChanged);
   Connect<DevStudio>(m_tabBar, "tabCloseClicked", this, &DevStudio::onTabCloseRequest);

   m_textEdit = new AQTextEdit(central);
   m_textEdit->setPreferredSize(AQPoint(640, 400));
   centralL->addWidget(m_textEdit);

   central->setLayout(centralL);
   setCentralWidget(central);

   m_projectView = new AQListView();
   setLeftSideBar(m_projectView);
   Connect<DevStudio>(m_projectView, "itemDoubleClicked", this, &DevStudio::onFileItemDoubleClicked);

   m_outputView = new AQListView();
   m_outputView->setTreeMode(false);
   m_outputView->setWordWrap(true);
   setBottomSideBar(m_outputView);
   Connect<DevStudio>(m_outputView, "itemActivated", this, &DevStudio::onOutputItemActivated);

   m_textEdit->setFocus();
   m_currentDoc = new DocInfo();
   m_currentDoc->doc = m_textEdit->document();

   m_positionLabel = new AQLabel();
   m_positionLabel->setText("Ln:    Col:    ");
   statusBar()->addPermanentWidget(m_positionLabel);

   m_findWidget = new FindWidget(m_textEdit);
   statusBar()->addWidget(m_findWidget);
   Connect<FindWidget>(clearFindAction, "triggered", m_findWidget, &FindWidget::deactivate);
   Connect<FindWidget>(m_findAction, "triggered", m_findWidget, &FindWidget::activateFind);
   Connect<FindWidget>(m_replaceAction, "triggered", m_findWidget, &FindWidget::activateReplace);

   AQAction *findNextAction = new AQAction(this);
   findNextAction->setShortcut("F3");
   findNextAction->setText("Find Next");
   aqApp->addAction(findNextAction);
   Connect<FindWidget>(findNextAction, "triggered", m_findWidget, &FindWidget::findNext);
   
   AQAction *findPrevAction = new AQAction(this);
   findPrevAction->setShortcut("Shift+F3");
   findPrevAction->setText("Find Previous");
   aqApp->addAction(findPrevAction);
   Connect<FindWidget>(findPrevAction, "triggered", m_findWidget, &FindWidget::findPrevious);
  
   m_lineNumberAction = new AQAction(this);
   m_lineNumberAction->setText("Show Line Numbers?");
   m_lineNumberAction->setCheckable(true);
   m_lineNumberAction->setChecked(false);
   aqApp->addAction(m_lineNumberAction);
   Connect<DevStudio>(m_lineNumberAction, "triggered", this, &DevStudio::onLineNumbers);

   openProject("Work:devel/AQ");

   Connect<DevStudio>(aqApp, "readFinished", this, &DevStudio::onReadFinished);
   m_pipeBuffer = new char[600];

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
   projectMenu->addAction(closeAction);
   projectMenu->addSeparator();
   projectMenu->addAction(quitAction);
   menubar->addMenu(projectMenu);

   AQMenu *editMenu = new AQMenu("Edit");
   editMenu->addAction(m_undoAction);
   editMenu->addAction(m_redoAction);
   editMenu->addSeparator();
   editMenu->addAction(m_cutAction);
   editMenu->addAction(m_copyAction);
   editMenu->addAction(m_pasteAction);
   editMenu->addSeparator();
   editMenu->addAction(m_findAction);
   editMenu->addAction(m_replaceAction);
   menubar->addMenu(editMenu);

   AQMenu *buildMenu = new AQMenu("Build");
   buildMenu->addAction(buildProjectAction);
   buildMenu->addAction(cleanProjectAction);
   AQMenu *configurationMenu = new AQMenu("Configuration");
   configurationMenu->addAction(debugConfigurationAction);
   configurationMenu->addAction(releaseConfigurationAction);
   buildMenu->addMenu(configurationMenu);
   buildMenu->addSeparator();
   buildMenu->addAction(prevMessageAction);
   buildMenu->addAction(nextMessageAction);
   menubar->addMenu(buildMenu);

   AQMenu *debugMenu = new AQMenu("Debug");
   debugMenu->addAction(runAction);
   menubar->addMenu(debugMenu);

   AQMenu *settingsMenu = new AQMenu("Settings");
   settingsMenu->addAction(m_lineNumberAction);
   menubar->addMenu(settingsMenu);

   setMenu(menubar);

   setPreferredSize(AQPoint(640, 496));
}

void DevStudio::onReadFinished()
{
   Close(m_pipeFh);
   m_pipeFh = nullptr;

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
         m_outputView->addTopLevelItem(outputItem);
      }
      text += AQString(m_pipeBuffer + start, i-start);
      outputItem->setText(0, text);

      if (m_pipeBuffer[i] == '\n') {
         m_incompleteAddedOutput = nullptr;
         ++i;
      } else {
         m_incompleteAddedOutput = outputItem;
      }
      start = i;

      if (text.contains("====== build completed ======")) {
         m_outputView->update();
         return;
      }
   }

   m_pipeFh =  Open("PIPE:adsbuild", MODE_OLDFILE);
   aqApp->startAsyncRead(m_pipeFh, m_pipeBuffer, 599);
   m_outputView->update();
}

DevStudio::~DevStudio()
{
   delete m_project;

   if (m_pipeFh)
      Close(m_pipeFh);
}

void DevStudio::openProject()
{
   AQFileDialog *dialog = new AQFileDialog(AQFileDialog::OpenButton | AQFileDialog::CancelButton
                  | AQFileDialog::SelectionName | AQFileDialog::DrawerMode, this);
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
   AQFileDialog *dialog = new AQFileDialog(AQFileDialog::OpenButton | AQFileDialog::CancelButton
                  | AQFileDialog::SelectionName, this);
   dialog->setWindowTitle("Open File");

   // Let's figure out what folder to start dialog in
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

   // Now let's ask user
   if (dialog->exec()) {
      openFile(dialog->selectedPath());
   }
   delete dialog;
}

void DevStudio::openFile(const AQString &path)
{
   if (m_loadedDocs.find(path) == m_loadedDocs.end()) {
      m_loadedDocs[path] = new DocInfo(this, path);
      m_tabBar->addTab(qFileName(path));
      m_tabBar->setTabToolTip(m_tabBar->count()-1, path);
   } else {
      if (m_currentDoc == m_loadedDocs[path])
         return; // already current
   }

   switchToDocument(path);
}

void DevStudio::closeFile()
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   while (it != m_loadedDocs.end()) {
      if (it->second == m_currentDoc) {
         closeFile(it->first);
         break;
      }
      it++;
   }
}

void DevStudio::closeFile(const AQString &path)
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.find(path);
   if (it == m_loadedDocs.end())
      return;

   if (!it->second->commandStack->isClean()) {
      AQMessageBox *dialog = new AQMessageBox(AQMessageBox::OkButton |
                               AQMessageBox::CancelButton, this);
      dialog->setWindowTitle("File Changed");
//path+"\nWhat do you want to do?\n";


      if (!dialog->exec()) {
         return;
      }
   }


   m_loadedDocs.erase(path);

   if (m_loadedDocs.empty()) {
      m_currentDoc = nullptr;
      m_textEdit->setDocument(nullptr, nullptr);
      onCursorPositionChanged(nullptr);
//   onUndoRedoActionTextsChanged();
//   m_findWidget->setDocument(m_currentDoc->doc, m_currentDoc->cursor, m_currentDoc->m_findReplaceMode);
      
   } else
      switchToDocument(m_loadedDocs.begin()->first);

   int tabIndex = 0;
   while (tabIndex < m_tabBar->count()) {
      if (m_tabBar->tabToolTip(tabIndex) == path)
         break;
      ++tabIndex;
   } 

   delete it->second;

   m_tabBar->removeTab(tabIndex);
}

void DevStudio::gotoLine(int n)
{
   if (!m_currentDoc)
      return;

   m_currentDoc->cursor->setPosition(
                  m_currentDoc->doc->findBlockByLineNumber(n).m_pos);
   onCursorPositionChanged(m_currentDoc->doc);
   m_textEdit->setFocus();
   m_textEdit->ensureCursorVisible();
}

void DevStudio::saveFile()
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   while (it != m_loadedDocs.end()) {
      if (it->second->doc == m_textEdit->document()) {
         it->second->doc->saveFile(it->first);
         it->second->commandStack->setClean();
         return;
      }
      it++;
   }
   saveFileAs();
}

void DevStudio::saveFileAs()
{
   AQFileDialog *dialog = new AQFileDialog(AQFileDialog::SaveButton | AQFileDialog::CancelButton
                  | AQFileDialog::SelectionName, this);
   dialog->setWindowTitle("Save File As");

   // Let's figure out what folder to start dialog in
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
      m_textEdit->document()->saveFile(dialog->selectedPath());
   }
   delete dialog;
}

void DevStudio::saveAll()
{
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();
   AQString filename;
   while (it != m_loadedDocs.end()) {
      if (!it->second->commandStack->isClean())
         it->second->doc->saveFile(it->first);

      it->second->commandStack->setClean();

      it++;
   }
}

void DevStudio::onRun()
{
   m_incompleteAddedOutput = nullptr;
   m_outputView->clear();
   m_project->run();
}

void DevStudio::onReleaseConfiguration()
{
   m_project->setConfiguration("release");
}

void DevStudio::onDebugConfiguration()
{
   m_project->setConfiguration("debug");
}

void DevStudio::onBuildProject()
{
   saveAll();
   m_incompleteAddedOutput = nullptr;
   m_outputView->clear();

   m_pipeFh =  Open("PIPE:adsbuild", MODE_OLDFILE);
   aqApp->startAsyncRead(m_pipeFh, m_pipeBuffer, 599);

   m_project->build();
}

void DevStudio::onCleanProject()
{
   saveAll();
   m_incompleteAddedOutput = nullptr;
   m_outputView->clear();

   m_pipeFh =  Open("PIPE:adsbuild", MODE_OLDFILE);
   aqApp->startAsyncRead(m_pipeFh, m_pipeBuffer, 599);

   m_project->clean();
}

void DevStudio::switchToDocument(const AQString &path)
{
   DocInfo *newCurrent = m_loadedDocs[path];

   m_currentDoc->offset.y = m_textEdit->verticalScrollBar()->value();
   m_currentDoc->m_findReplaceMode = m_findWidget->currentActiveMode();

   m_currentDoc = newCurrent;

   int tabIndex = 0;
   while (tabIndex < m_tabBar->count()) {
      if (m_tabBar->tabToolTip(tabIndex) == path)
         break;
      ++tabIndex;
   } 
   m_tabBar->setCurrentIndex(tabIndex);


   m_textEdit->setDocument(m_currentDoc->doc, m_currentDoc->cursor);
   m_textEdit->verticalScrollBar()->setValue(m_currentDoc->offset.y);
   m_textEdit->setFocus();
   onCursorPositionChanged(m_currentDoc->doc);
   onUndoRedoActionTextsChanged();
   m_findWidget->setDocument(m_currentDoc->doc, m_currentDoc->cursor, m_currentDoc->m_findReplaceMode);
}

void DevStudio::onFileItemDoubleClicked(AQObject *obj)
{
   AQListItem *item = (AQListItem *)(obj);
   AQString path(item->text(1));

   openFile(path);
}

void DevStudio::onCurrentTabChanged(int i)
{
   AQString path = m_tabBar->tabToolTip(i);

   switchToDocument(path);
}

void DevStudio::onTabCloseRequest(int i)
{
   AQString path = m_tabBar->tabToolTip(i);

   closeFile(path);
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
   if (obj == nullptr) {
      m_positionLabel->setText("Ln:     Col:     ", false);
      return;
   }

   AQTextDoc *doc = static_cast<AQTextDoc *>(obj);
   map<AQString, DocInfo *>::iterator it = m_loadedDocs.begin();

   while (it != m_loadedDocs.end()) {
      if (it->second->doc == doc) {
         AQTextCursor *cursor = it->second->cursor;

         AQString t("Ln: ");
         t += AQString::number(1 + doc->blockNumber(cursor->position()));
         t += " Col: ";
         t += AQString::number(1 + cursor->positionInBlock());
         m_positionLabel->setText(t, false);
         m_cutAction->setEnabled(cursor->hasSelection());
         m_copyAction->setEnabled(cursor->hasSelection());
         return;
      }
      it++;
   }
}

void DevStudio::onOutputItemActivated(AQObject *obj)
{
   AQListItem *item = (AQListItem *)(obj);
   AQString text(item->text(0));

   int i = text.indexOf(":");

   if (i > 0) {
      AQString filename = text.left(i);
      bool ok;
      int linenumber = text.mid(i+1).toInt(&ok);
      if (ok && !filename.isEmpty()) {
         openFile(m_project->projectPath() + "/" + filename);
         gotoLine(linenumber-1);
         return;
      }
   }
   m_textEdit->setFocus();
}

void DevStudio::onPrevMessage()
{
   AQListItem *item;
   if (m_outputView->selectedItems().size() == 0)
      item = m_outputView->rootItem()->child(0);
   else
      item = m_outputView->itemAbove(m_outputView->selectedItems()[0]);

   if (item)
      m_outputView->selectItem(item);
}

void DevStudio::onNextMessage()
{
   AQListItem *item;
   if (m_outputView->selectedItems().size() == 0)
      item = m_outputView->rootItem()->child(0);
   else
      item = m_outputView->itemBelow(m_outputView->selectedItems()[0]);

   if (item)
      m_outputView->selectItem(item);
}

void DevStudio::onUndoRedoActionTextsChanged()
{
   if (!m_currentDoc)
      return;

   m_undoAction->setText(m_currentDoc->commandStack->undoText());
   m_undoAction->setEnabled(m_currentDoc->commandStack->canUndo());

   m_redoAction->setText(m_currentDoc->commandStack->redoText());
   m_redoAction->setEnabled(m_currentDoc->commandStack->canRedo());
}

void DevStudio::onUndo()
{
   if (!m_currentDoc)
      return;

   m_currentDoc->commandStack->undo();
   m_textEdit->update();
   m_textEdit->ensureCursorVisible();
}

void DevStudio::onRedo()
{
   if (!m_currentDoc)
      return;

   m_currentDoc->commandStack->redo();
   m_textEdit->update();
   m_textEdit->ensureCursorVisible();
}

void DevStudio::onCut()
{
   if (!m_currentDoc)
      return;

   m_textEdit->cut();
}

void DevStudio::onCopy()
{
   if (!m_currentDoc)
      return;

   m_textEdit->copy();
}

void DevStudio::onPaste()
{
   if (!m_currentDoc)
      return;

   m_textEdit->paste();
}


void DevStudio::onLineNumbers(bool on)
{
   m_textEdit->setAuxColumnWidth(on ? 32 : 0);
}

