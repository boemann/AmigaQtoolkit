
#include <AQCommandStack.h>
#include <stdio.h>

AQCommand::AQCommand()
{
}

AQCommand::~AQCommand()
{
}

bool AQCommand::mergeWith(AQCommand *cmd)
{
   return false;
}

AQString AQCommand::actionText()
{
   return "";
}

void AQCommand::undo()
{
}

void AQCommand::redo()
{
}

AQCommandStack::AQCommandStack()
   : AQObject(nullptr)
   , m_cleanIndex(0)
{
} 

AQCommandStack::~AQCommandStack()
{
   // delete all commands as we per definition own them
   while (m_doneCommands.size()) {
      delete m_doneCommands.back();
      m_doneCommands.pop_back();
   }

   while (m_undoneCommands.size()) {
      delete m_undoneCommands.back();
      m_undoneCommands.pop_back();
   }
}

void AQCommandStack::push(AQCommand *cmd)
{
   bool wasClean = isClean();

   if (m_cleanIndex > m_doneCommands.size())
      resetClean();

   if (isClean() || !canUndo() || !m_doneCommands.back()->mergeWith(cmd))
      m_doneCommands.push_back(cmd);



   // At this point the undone commands are no longer relevant so delete them
   while (m_undoneCommands.size()) {
      delete m_undoneCommands.back();
      m_undoneCommands.pop_back();
   }

   if (wasClean)
      emit("cleanChanged");
   emit("actionTextsChanged");
}

void AQCommandStack::resetClean()
{
   bool wasClean = isClean();

   m_cleanIndex = -1;

   if (wasClean)
      emit("cleanChanged");
}

void AQCommandStack::setClean()
{
   if (isClean())
      return;

   m_cleanIndex = m_doneCommands.size();
   emit("cleanChanged");
}

bool AQCommandStack::isClean() const
{
   return m_cleanIndex == m_doneCommands.size();
}

bool AQCommandStack::canUndo() const
{
   return m_doneCommands.size() > 0;
}

bool AQCommandStack::canRedo() const
{
   return m_undoneCommands.size() > 0;
}

void AQCommandStack::undo()
{
   if (!canUndo())
      return;

   bool wasClean = isClean();

   m_doneCommands.back()->undo();

   m_undoneCommands.push_back(m_doneCommands.back());
   m_doneCommands.pop_back();

   if (wasClean != isClean())
      emit("cleanChanged");
   emit("actionTextsChanged");
}

void AQCommandStack::redo()
{
   if (!canRedo())
      return;

   bool wasClean = isClean();

   m_undoneCommands.back()->redo();

   m_doneCommands.push_back(m_undoneCommands.back());
   m_undoneCommands.pop_back();

   if (wasClean != isClean())
      emit("cleanChanged");
   emit("actionTextsChanged");
}

AQString AQCommandStack::undoText() const
{
   if (canUndo())
      return AQString("Undo ") + m_doneCommands.back()->actionText();
   else
      return AQString("Undo");
}

AQString AQCommandStack::redoText() const
{
   if (canRedo())
      return AQString("Redo ") + m_undoneCommands.back()->actionText();
   else
      return AQString("Redo");
}
