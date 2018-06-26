#ifndef AQSYNTAXHIGHLIGHTER_H
#define AQSYNTAXHIGHLIGHTER_H

#include <vector.h>

#include <AQKernel.h>
#include <AQObject.h>
#include <AQString.h>

class AQTextDoc;

class AQSyntaxHighlighter
{
public:
   AQSyntaxHighlighter(AQTextDoc *doc);

   friend class AQTextDoc;

protected:
   virtual void highlightBlock(const AQString &text);
   void setFormat(int start, int count, int color);

private:
   void setBlockNumber(int blockNumber);
   int m_blockNumber;
   AQTextDoc *m_doc;
};


#endif
