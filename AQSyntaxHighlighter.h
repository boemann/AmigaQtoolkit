#ifndef AQSYNTAXHIGHLIGHTER_H
#define AQSYNTAXHIGHLIGHTER_H

#include <vector.h>

#include <AQKernel.h>
#include <AQObject.h>
#include <AQString.h>

class AQTextDoc;
class CharState;

class AQSyntaxHighlighter
{
public:
   AQSyntaxHighlighter(AQTextDoc *doc);

   friend class AQTextDoc;

protected:
   virtual void highlightBlock(const AQString &text);
   void setFormat(int start, int count, int color);

   void addKeyWord(char *keyword, int color, int appendNonWord = false);


private:
   void setBlockNumber(int blockNumber);
   int m_blockNumber;
   AQTextDoc *m_doc;
   CharState *m_defaultState;
   CharState *m_nonWordState;
};


#endif
