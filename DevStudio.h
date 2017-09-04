#ifndef ADS_H
#define ADS_H

#include <map.h>
#include <AQMainWindow.h>
#include <AQString.h>

class AQTextEdit;
class AQListView;
class Project;
class AQLabel;
class DocInfo;

typedef long BPTR;

class DevStudio : public AQMainWindow
{
public:
   DevStudio();
   ~DevStudio();

   void openProject(const AQString &projectPath);
   void openFile();
   void saveFileAs();
   void saveFile();
   void saveAll();
   void buildProject();
   void run();
   void onFileItemDoubleClicked(AQObject *obj);
   void onDocModificationChanged(AQObject *obj);
   void onCursorPositionChanged(AQObject *obj);
   void onReadFinished();

private:
   AQTextEdit *m_textEdit;
   AQListView *m_projectView;
   AQLabel *m_positionLabel;
   Project *m_project;
   map<AQString, DocInfo *> m_loadedDocs;
   DocInfo *m_currentDoc;
   BPTR m_pipeFh;
   char *m_pipeBuffer;
};
   
#endif
