#ifndef ADS_H
#define ADS_H

#include <map.h>
#include <vector.h>
#include <AQMainWindow.h>
#include <AQString.h>

class AQTextEdit;
class AQTextCursor;
class AQListView;
class AQListItem;
class Project;
class AQLabel;
class DocInfo;
class FindWidget;

typedef long BPTR;

class DevStudio : public AQMainWindow
{
public:
   DevStudio();
   ~DevStudio();


   void openProject();
   void newFile();
   void openFile();
   void saveFileAs();
   void saveFile();
   void saveAll();
   void onBuildProject();
   void onRun();
   void onFileItemDoubleClicked(AQObject *obj);
   void onDocModificationChanged(AQObject *obj);
   void onCursorPositionChanged(AQObject *obj);
   void onOutputItemActivated(AQObject *obj);
   void onReadFinished();
   void onPrevMessage();
   void onNextMessage();
   void onUndoRedoActionTextsChanged();
   void onUndo();
   void onRedo();
   void onCut();
   void onCopy();
   void onPaste();

private:
   void openProject(const AQString &projectPath);
   void openFile(const AQString &path);
   void switchToDocument(DocInfo *newCurrent);
   void gotoLine(int n);

   AQTextEdit *m_textEdit;
   AQListView *m_projectView;
   AQListView *m_outputView;
   AQListItem *m_incompleteAddedOutput;
   AQLabel *m_positionLabel;
   FindWidget *m_findWidget;

   Project *m_project;
   map<AQString, DocInfo *> m_loadedDocs;
   DocInfo *m_currentDoc;
   BPTR m_pipeFh;
   char *m_pipeBuffer;

   AQAction *m_undoAction;
   AQAction *m_redoAction;
   AQAction *m_cutAction;
   AQAction *m_copyAction;
   AQAction *m_pasteAction;
   AQAction *m_findAction;
   AQAction *m_replaceAction;
};
   
#endif
