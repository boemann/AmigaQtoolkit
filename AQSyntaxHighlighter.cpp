#include <algorithm>

#include <AQSyntaxHighlighter.h>
#include <AQTextDoc.h>

#include <string.h>
#include <ctype.h>
#include <stdio.h>



AQSyntaxHighlighter::AQSyntaxHighlighter(AQTextDoc *doc)
   : m_blockNumber(0)
   , m_doc(doc)
{
   doc->m_highlighter = this;
}


void AQSyntaxHighlighter::highlightBlock(const AQString &text)
{
   if (text.contains("void"))
      setFormat(0, text.size(), 5);
}

void AQSyntaxHighlighter::setBlockNumber(int b)
{
   m_blockNumber = b;
}

void AQSyntaxHighlighter::setFormat(int start, int count, int color)
{
   if (m_doc->m_highlightRanges[m_blockNumber] == nullptr) {
      AQFormatRange *rng = new AQFormatRange(color, -1, nullptr);
      rng->setStartEnd(start, start+count);
      
      m_doc->m_highlightRanges[m_blockNumber] = rng;
   } else {
      AQFormatRange rng(color, -1, nullptr);
      rng.setStartEnd(start, start+count);
      
      m_doc->m_highlightRanges[m_blockNumber]->modifyWith(&rng);
   }
}

