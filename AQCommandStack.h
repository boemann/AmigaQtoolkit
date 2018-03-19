#ifndef AQCOMMANDSTACK_H
#define AQCOMMANDSTACK_H

#include <AQObject.h>
#include <AQString.h>
#include <AQCommand.h>
#include <vector.h>

class AQCommandStack : public AQObject
{
public:
   AQCommandStack();
   ~AQCommandStack();

   void push(AQCommand *cmd);

   void resetClean();
   void setClean();
   bool isClean() const;

   bool canUndo() const;
   bool canRedo() const;
   void undo();
   void redo();

   AQString undoText() const;
   AQString redoText() const;

private:
   vector<AQCommand *> m_doneCommands;
   vector<AQCommand *> m_undoneCommands;

   int m_cleanIndex;
};


#endif
