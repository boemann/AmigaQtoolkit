#include <algorithm>

#include <AQTextDoc.h>
#include <AQCommand.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <graphics/gfxbase.h>
#include <graphics/rastport.h>

#include <proto/graphics.h>
#include <proto/dos.h>



AQFormatRange::AQFormatRange(int fg, int bg, TextFont *font)
   : m_fgColor(fg)
   , m_bgColor(bg)
   , m_font(font)
   , m_next(nullptr)
{
}

void AQFormatRange::setStartEnd(int start, int end)
{
   m_startInBlock = start;
   m_endInBlock = end;
}

void AQFormatRange::split(int at)
{
   if (at >= m_endInBlock)
      return;

   AQFormatRange *rng = new AQFormatRange(m_fgColor, m_bgColor, m_font);
   rng->m_startInBlock = at;
   rng->m_endInBlock = m_endInBlock;
   m_endInBlock = at;

   rng->m_next = m_next;
   m_next = rng;
}

void AQFormatRange::modifyWith(AQFormatRange *other)
{
   if (other->m_startInBlock <= m_startInBlock) {
      if (other->m_endInBlock <= m_startInBlock)
         return;

      // we will morph ourself

      if (other->m_endInBlock <= m_endInBlock)
         split(other->m_endInBlock); // but don't morph all of us
      
      if (other->m_fgColor >= 0)
         m_fgColor = other->m_fgColor;

      if (other->m_bgColor >= 0)
         m_bgColor = other->m_bgColor;

      if (other->m_font)
         m_font = other->m_font;
   } else if (other->m_startInBlock < m_endInBlock) {
      // We will keep ourself but split so it can morph next
      split(other->m_startInBlock);
   }
   if (m_next)
      m_next->modifyWith(other);
}

void AQFormatRange::render(RastPort *rp, char *data)
{
   SetAPen(rp, m_fgColor);
   SetBPen(rp, m_bgColor);
   SetDrMd(rp, m_bgColor < 0 ? JAM1 : JAM2);
   if (m_font)
      SetFont(rp, m_font);

   char *firstPrintable = data + m_startInBlock;
   char *pastRange = data + m_endInBlock;
   while (firstPrintable < pastRange) {
      if (*firstPrintable == '\n' || *firstPrintable == 0) {
         if (m_bgColor < 0)
            return;

         SetAPen(rp, m_bgColor);

         int extra = rp->TxWidth;
         int top = rp->cp_y - rp->TxBaseline;

         RectFill(rp, rp->cp_x, top, rp->cp_x + extra, top + rp->TxHeight - 1);

         return;
      }
      if (*firstPrintable == '\t') {
         int extra = 3 * rp->TxWidth;

         if (m_bgColor >= 0) {
            SetAPen(rp, m_bgColor);

            int top = rp->cp_y - rp->TxBaseline;

            RectFill(rp, rp->cp_x, top, rp->cp_x + extra, top + rp->TxHeight - 1);

            SetAPen(rp, m_fgColor);
         }

         Move(rp, rp->cp_x + extra, rp->cp_y);
      }

      if (*firstPrintable < 32)
         ++firstPrintable;
      

      int printableCount = 0;
      while (firstPrintable + printableCount < pastRange) {
         if (firstPrintable[printableCount] < 32)
            break;
         ++printableCount;
      }

      Text(rp, firstPrintable, printableCount);

      firstPrintable += printableCount;
   }
}


void AQTextDoc::renderBlock(RastPort *rp, int x, int &yTop, int b)
{
   int yBase = yTop + rp->TxBaseline;
   char *lineData = m_blocks[b].m_pos + m_data;
   int len = m_blocks[b+1].m_pos - m_blocks[b].m_pos;

   AQFormatRange rng(1, -2, m_defaultFont);
   rng.setStartEnd(0, len);
   AQFormatRange *r = &rng;

   for (int i = m_cursors.size() -1; i >= 0; --i) {
      AQTextCursor *cursor = m_cursors[i];

      if (cursor->hasSelection() ) {
         int thisLineStart = m_blocks[b].m_pos;
         int selStart = cursor->selectionStart() - thisLineStart;
         int selEnd = cursor->selectionEnd() - thisLineStart;
         if (selStart < len && selEnd >= 0) {
            if (selEnd >= len)
               selEnd = len;

            cursor->setStartEnd(selStart, selEnd);
            rng.modifyWith(cursor);
         }   
      }
   }

   Move(rp, x, yBase);
   while (r) {
      r->render(rp, lineData);
      r = r->next();
   }

   yTop += lineHeight();
}

void AQTextDoc::render(RastPort *rp, AQPoint docOffset, AQPoint botRight)
{
   TextFont *oldFont = rp->Font;
   SetFont(rp, m_defaultFont);
   m_charWidth = rp->TxWidth;

   int b = docOffset.y / lineHeight();
   int y = b * lineHeight() - docOffset.y;
   while (b < m_numBlocks) {
      renderBlock(rp, -docOffset.x, y, b);

      if (rp->TxBaseline + y >= botRight.y)
         break;
      ++b;
   }

   SetFont(rp, oldFont);
} 

void AQTextDoc::renderCursor(RastPort *rp, AQTextCursor *cursor,
                     AQPoint docOffset, AQPoint botRight)
{
   bool draw = !cursor->hasSelection();
   if (!draw && cursor->position() == cursor->selectionEnd())
       draw = cursor->positionInBlock() == 0;

   if (draw) {
      TextFont *oldFont = rp->Font;
      SetFont(rp, m_defaultFont);

      SetAPen(rp, 3);
      int linestart = cursor->position() - cursor->positionInBlock();
      LONG x = TextLength(rp, m_data + linestart, cursor->positionInBlock());
      LONG y = blockNumber(cursor->position()) * lineHeight();
      x -= docOffset.x;
      y -= docOffset.y;
      int tabCount = 0;
      for (int i = 0; i < cursor->positionInBlock(); ++i)
         if (m_data[linestart + i] == '\t')
            ++tabCount;
      x += tabCount*2*rp->TxWidth;
      if (y + lineHeight() <= botRight.y + 1)
         RectFill(rp, x, y, x + 1, y + lineHeight());

      SetFont(rp, oldFont);
   }
}

AQPoint AQTextDoc::pointOfPosition(int pos) const
{
   int b = blockNumber(pos);
   int linestart = m_blocks[b].m_pos;


   RastPort rp;
   InitRastPort(&rp);
   SetFont(&rp, m_defaultFont);

   int x = TextLength(&rp, m_data + linestart, pos - linestart);
   int y = b * lineHeight() + rp.TxBaseline;

   int tabCount = 0;
   for (int i = 0; i < pos - linestart; ++i)
      if (m_data[linestart + i] == '\t')
         ++tabCount;
   x += tabCount*2*rp.TxWidth;

   return AQPoint(x,y);
}

int AQTextDoc::positionOfPoint(const AQPoint &p) const
{
   if (p.y < 0)
      return 0;

   int b = p.y / lineHeight();
   if (b > m_numBlocks)
      return m_size - 1;

   RastPort rp;
   InitRastPort(&rp);
   SetFont(&rp, m_defaultFont);

   int pos = m_blocks[b].m_pos;
   int lowpos = 0;
   int highpos = m_blocks[b+1].m_pos - pos -1;

   char *d = m_data +pos;
   int posInBlock;
   int testX;
   do {
      posInBlock = (highpos + lowpos) / 2;
      testX = TextLength(&rp, d, posInBlock);
      if (testX < p.x)
         lowpos = posInBlock;
      else
         highpos = posInBlock;
   } while (highpos - lowpos > 1);

   if (posInBlock == lowpos) {
      if ((TextLength(&rp, d, highpos) + testX +1) / 2 < p.x)
         posInBlock = highpos;
   } else {
      if ((testX + TextLength(&rp, d, lowpos)+1) / 2 > p.x)
         posInBlock = lowpos;
   }
   
   return pos + posInBlock;
}
