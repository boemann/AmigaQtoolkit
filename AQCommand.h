#ifndef AQCOMMAND_H
#define AQCOMMAND_H

#include <AQString.h>

class AQCommand
{
public:
   AQCommand();
   ~AQCommand();

   virtual bool mergeWith(AQCommand *cmd);
   virtual AQString actionText();
   virtual void undo();
   virtual void redo();
};


#endif
