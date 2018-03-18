#ifndef ADS_H
#define ADS_H

#include <map.h>
#include <AQMainWindow.h>
#include <AQString.h>

class AQTextEdit;
class AQListView;
class AQListItem;
class Project;
class AQLabel;
class DocInfo;

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

private:
   void openProject(const AQString &projectPath);
   void openFile(const AQString &path);
   void gotoLine(int n);

   AQTextEdit *m_textEdit;
   AQListView *m_projectView;
   AQListView *m_outputView;
   AQListItem *m_incompleteAddedOutput;
   AQLabel *m_positionLabel;
   Project *m_project;
   map<AQString, DocInfo *> m_loadedDocs;
   DocInfo *m_currentDoc;
   BPTR m_pipeFh;
   char *m_pipeBuffer;
};
   
#endif
