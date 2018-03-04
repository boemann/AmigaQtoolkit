#include <algorithm>

#include <AQTextDoc.h>
#include <string.h>
#include <ctype.h>

#include <graphics/gfxbase.h>
#include <graphics/rastport.h>

#include <proto/graphics.h>
#include <proto/dos.h>

AQTextCursor::AQTextCursor(AQTextDoc &doc)
   : m_doc(doc)
   , m_pos(0)
   , m_posInBlock(0)
   , m_wishX(0)
   , m_anchorPos(0)
   , m_anchorInBlock(0)
{
   m_doc.addCursor(this);
}

AQTextCursor::~AQTextCursor()
{
   m_doc.removeCursor(this);
}

void AQTextCursor::insertText(const AQString &str)
{
   if (hasSelection())
      deleteSelection();

   int addLen = str.size();

   m_doc.pushData(m_pos, addLen);
   for (int i = 0; i < addLen; i++) {
      m_doc.m_data[m_pos++] = str[i];
      m_posInBlock++;
      if (str[i] == '\n')
         m_posInBlock = 0;
   }
   m_wishX = m_posInBlock;
   m_anchorInBlock = m_posInBlock;
   m_anchorPos = m_pos;
   m_doc.updateBlocks();
}

void AQTextCursor::insertBlock()
{
   if (hasSelection())
      deleteSelection();

   m_doc.pushData(m_pos, 1);
   m_doc.m_data[m_pos] = '\n';
   ++m_pos;
   m_wishX = m_posInBlock;
   m_anchorInBlock = m_posInBlock = 0;
   m_anchorPos = m_pos;
   m_doc.updateBlocks();
}  

int AQTextCursor::position() const
{
   return m_pos;
}

int AQTextCursor::positionInBlock() const
{
   return m_posInBlock;
}

bool AQTextCursor::hasSelection() const
{
   return m_pos != m_anchorPos;
}

int AQTextCursor::selectionStart() const
{
   return m_pos < m_anchorPos ? m_pos : m_anchorPos;
}

int AQTextCursor::selectionEnd() const
{
   return m_pos > m_anchorPos ? m_pos : m_anchorPos;
}

void AQTextCursor::setPosition(int pos, bool keepAnchor)
{
   if (pos >=0 && pos <= m_doc.m_size) {
      m_pos = pos;
      m_posInBlock = 0;
      while(m_pos - m_posInBlock > 0 && m_doc.m_data[m_pos - m_posInBlock - 1] != '\n')
         ++m_posInBlock;

      m_wishX = m_posInBlock;

      if (!keepAnchor) {
         m_anchorPos = m_pos;
         m_anchorInBlock = m_posInBlock;
      }
      m_doc.emit("cursorPositionChanged", &m_doc);
   }
}

void AQTextCursor::select(AQTextCursor::SelectionType type)
{
   switch(type) {
   default:
      m_anchorPos=m_pos;
      m_anchorInBlock = m_posInBlock;
      while (m_anchorPos && isalnum(m_doc.m_data[m_anchorPos-1])) {
         --m_anchorPos;
         --m_anchorInBlock;
      }
      while (m_doc.m_data[m_pos] && isalnum(m_doc.m_data[m_pos])) {
         ++m_pos;
         ++m_posInBlock;
      }
      m_wishX = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

AQString AQTextCursor::selectedText() const
{
   if (hasSelection())
      return AQString(m_doc.m_data + selectionStart(), selectionEnd()- selectionStart());

   return AQString();
}

void AQTextCursor::deleteLeft()
{
   if (hasSelection()) {
      deleteSelection();
      return;
   }

   if (m_pos) {
      movePrevChar(false);
      m_doc.deleteData(m_pos, 1);
      m_doc.updateBlocks();
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::deleteRight()
{
   if (hasSelection()) {
      deleteSelection();
      return;
   }

   if (m_doc.m_size > m_pos) {
      m_doc.deleteData(m_pos, 1);
      m_doc.updateBlocks();
      m_wishX = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::deleteSelection()
{
   int n;
   if (m_pos > m_anchorPos) {
      n = m_pos - m_anchorPos;
      m_pos = m_anchorPos;
      m_posInBlock = m_anchorInBlock;
   } else {
      n = m_anchorPos - m_pos;
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   if (n == 0)
      return;
   m_doc.deleteData(m_pos, n);
   m_doc.updateBlocks();
   m_wishX = m_posInBlock;
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveNextChar(bool keepAnchor)
{
   if (m_doc.m_size > m_pos) {
      if (m_doc.m_data[m_pos] == '\n')
         m_posInBlock = 0;
      else
         ++m_posInBlock;
      ++m_pos;
   }
   m_wishX = m_posInBlock;

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePrevChar(bool keepAnchor)
{
   if (m_pos) {
      --m_pos;
      if (m_posInBlock == 0) {
         char *cptr = m_doc.m_data + m_pos - 1;
         while (cptr >= m_doc.m_data && *cptr != '\n')
            --cptr;
         ++cptr;
         m_posInBlock = m_doc.m_data + m_pos - cptr;
      } else
         --m_posInBlock;
   }
   m_wishX = m_posInBlock;

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveNextLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_pos);

   if (block < m_doc.m_numBlocks - 1) {
      m_pos = m_doc.m_blocks[block+1].m_pos;
      m_posInBlock = 0;
      while (m_doc.m_size > m_pos) {
         if (m_doc.m_data[m_pos] == '\n')
            break;
         if (m_posInBlock == m_wishX)
            break;
         ++m_pos;
         ++m_posInBlock;
      }
   }

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePrevLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_pos);

   if (block > 0 ) {
      m_pos = m_doc.m_blocks[block-1].m_pos;
      m_posInBlock = 0;
      while (m_doc.m_size > m_pos) {
         if (m_doc.m_data[m_pos] == '\n')
            break;
         if (m_posInBlock == m_wishX)
            break;
         ++m_pos;
         ++m_posInBlock;
      }
   }

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveStartOfLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_pos);

   m_pos = m_doc.m_blocks[block].m_pos;
   m_posInBlock = 0;
   m_wishX = m_posInBlock;

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveEndOfLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_pos);

   m_pos = m_doc.m_blocks[block+1].m_pos - 1;
   m_posInBlock = m_doc.m_blocks[block+1].m_pos - m_doc.m_blocks[block].m_pos - 1;
   m_wishX = m_posInBlock;

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePageUp(int visibleCount, bool keepAnchor)
{
   int block = m_doc.blockNumber(m_pos);

   if (block > 0 ) {
      block -= visibleCount;
      if (block < 0)
         block = 0;
      m_pos = m_doc.m_blocks[block].m_pos;
      m_posInBlock = 0;
      while (m_doc.m_size > m_pos) {
         if (m_doc.m_data[m_pos] == '\n')
            break;
         if (m_posInBlock == m_wishX)
            break;
         ++m_pos;
         ++m_posInBlock;
      }
   }

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePageDown(int visibleCount, bool keepAnchor)
{
   int block = m_doc.blockNumber(m_pos);

   if (block < m_doc.m_numBlocks - 1) {
      block += visibleCount;
      if (block >= m_doc.m_numBlocks - 1)
         block = m_doc.m_numBlocks - 1;
      m_pos = m_doc.m_blocks[block].m_pos;
      m_posInBlock = 0;
      while (m_doc.m_size > m_pos) {
         if (m_doc.m_data[m_pos] == '\n')
            break;
         if (m_posInBlock == m_wishX)
            break;
         ++m_pos;
         ++m_posInBlock;
      }
   }

   if (!keepAnchor) {
      m_anchorPos = m_pos;
      m_anchorInBlock = m_posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::dataPushed(int pos, int n)
{
}

void AQTextCursor::dataDeleted(int pos, int n)
{
}

AQTextDoc::AQTextDoc(AQObject *parent)
   : AQObject(parent)
   , m_data(nullptr)
   , m_size(0)
   , m_lineHeight(7)
   , m_modified(false)
{
   m_capacity = 1000;
   m_data = new char[m_capacity];
   m_data[0] = 0;
   strcpy(m_data, "fgfdg");
   m_size=strlen(m_data);
   updateBlocks();
}

AQTextDoc::~AQTextDoc()
{
   while (m_cursors.size())
      delete m_cursors.front();

   delete m_data;
}

void AQTextDoc::loadFile(const AQString &fileName)
{
   BPTR file =  Open(fileName, MODE_OLDFILE);
   if (file == -1)
      return;

   FileInfoBlock *fib = (FileInfoBlock *)AllocDosObject(DOS_FIB, nullptr);
   if (fib) {
      ExamineFH(file, fib);
      m_size = fib->fib_Size;
      FreeDosObject(DOS_FIB, fib);
   }
   m_capacity = (m_size + 0x1000) & 0xFFFFFF00;
   delete m_data;
   m_data = new char[m_capacity];
   m_size = Read(file, m_data, m_capacity-1);
   if (m_size >= 0) {
      m_data[m_size] = 0; // make sure we are null terminated
   }
   Close(file);
   updateBlocks();

   setModified(false);
}

bool AQTextDoc::saveFile(const AQString &fileName) const
{
   BPTR file =  Open(fileName, MODE_NEWFILE);
   if (file == -1)
      return false;

   bool success = m_size == Write(file, m_data, m_size);

   Close(file);

   return success;
}

void AQTextDoc::setData(const AQString &text)
{
   m_size = text.size();
   m_capacity = (m_size + 0x1000) & 0xFFFFFF00;
   delete m_data;
   m_data = new char[m_capacity];
   strncpy(m_data, text, m_size);
   if (m_size >= 0) {
      m_data[m_size] = 0; // make sure we are null terminated
   }
   updateBlocks();

   setModified(false);
}

AQString AQTextDoc::toString() const
{
   return AQString(m_data, m_size);
}

void AQTextDoc::render(RastPort *rp, AQPoint docOffset, AQPoint botRight)
{
   m_lineHeight = rp->TxHeight;
   m_charWidth = rp->TxWidth;

   UWORD y = rp->TxBaseline;
   rp->DrawMode = JAM1;

   AQTextCursor *cursor = m_cursors.front(); // prep for when we have more than 1

   int b = docOffset.y / m_lineHeight;
   y += b * m_lineHeight;
   while (b < m_numBlocks) {
      // first draw the plain Line
      SetAPen(rp, 1);
      SetBPen(rp, 2);
      SetDrMd(rp, JAM1);
      Move(rp, -docOffset.x, y - docOffset.y);
      UWORD len = m_blocks[b+1].m_pos - m_blocks[b].m_pos - 1;
      
      Text(rp, m_blocks[b].m_pos + m_data, len);

      // Next draw the cursors on top
      if (cursor->hasSelection() ) {
         int extra = 0;
         int nextLineStart = m_blocks[b+1].m_pos;
         int thisLineStart = m_blocks[b].m_pos;
         int selStart = cursor->selectionStart();
         int selEnd = cursor->selectionEnd();
         if (selStart < nextLineStart && selEnd >= thisLineStart) {
            if (selStart < thisLineStart)
               selStart = thisLineStart;
            if (selEnd == thisLineStart)
               extra = 2;
            if (selEnd >= nextLineStart) {
               len = nextLineStart - selStart - 1;
               extra = rp->TxWidth;
            } else
               len = selEnd - selStart;
            int x = (selStart - thisLineStart) * rp->TxWidth - docOffset.x;

            SetDrMd(rp, JAM2);
            SetAPen(rp, 2);
            SetBPen(rp, 3);
            Move(rp, x, y - docOffset.y);
            Text(rp, m_data + selStart, len);
            if (extra > 0) {
               SetAPen(rp, 3);
               x += len * rp->TxWidth;
               int tmpY = y - docOffset.y - rp->TxBaseline;
               RectFill(rp, x, tmpY, x + extra - 1, tmpY + rp->TxHeight - 1);
            }
         }   
      }

      y += m_lineHeight;
      if (y - docOffset.y >= botRight.y)
         break;
      ++b;
   }
} 

bool AQTextDoc::isModified() const
{
   return m_modified;
}

void AQTextDoc::setModified(bool m)
{
   if (m_modified == m)
      return;

   m_modified = m;

   emit("modificationChanged", this);
}

int AQTextDoc::height() const
{
   return m_numBlocks * m_lineHeight;
}

int AQTextDoc::lineHeight() const
{
   return m_lineHeight;
}

int AQTextDoc::blockNumber(int pos)
{
   // do a binary seach to find the block number
   int blockNum = m_numBlocks >> 1;
   int maxBlock = m_numBlocks;

   while (true) {
      if (blockNum == m_numBlocks)
         return blockNum - 1;

      if (pos < m_blocks[blockNum].m_pos) {
         maxBlock = blockNum;
         blockNum >>= 1;
      } else if (pos >= m_blocks[blockNum + 1].m_pos) {
         blockNum += maxBlock + 1;
         blockNum >>= 1;
      } else
         return blockNum;
   }
   return -1; // can never happen
}

void AQTextDoc::pushData(int pos, int n)
{
   if (m_capacity > m_size + n + 1)
      for (int i = m_size + n + 1; i > pos; --i) {
         m_data[i] = m_data[i-n];
      }
   else {
      // push and increase capacity in one go
      m_capacity += 1024;
      char *oldData = m_data;
      m_data = new char[m_capacity];
      for (int i = 0; i < pos; ++i)
         m_data[i] = oldData[i];
      for (int i = m_size + n +1; i > pos; --i)
         m_data[i] = oldData[i-n];
      delete oldData;
   }
   m_size += n;

   for (int i = 0; i < m_cursors.size(); ++i)
      m_cursors[i]->dataPushed(pos, n);

   setModified(true);
}

void AQTextDoc::deleteData(int pos, int n)
{
   for (int i = pos; i <  m_size - n + 1; ++i) {
         m_data[i] = m_data[i+n];
   }
   m_size -= n;

   for (int i = 0; i < m_cursors.size(); ++i)
      m_cursors[i]->dataDeleted(pos, n);

   setModified(true);
}

void AQTextDoc::addCursor(AQTextCursor *cursor)
{
   m_cursors.push_back(cursor);
}

void AQTextDoc::removeCursor(AQTextCursor *cursor)
{
   m_cursors.erase(std::remove(m_cursors.begin(), m_cursors.end(), cursor), m_cursors.end());
}

void AQTextDoc::updateBlocks()
{
   m_numBlocks = 0;
   char *ptr = m_data;
   do {
      m_blocks[m_numBlocks++].m_pos = ptr - m_data;

      while (*ptr && *ptr != '\n')
         ++ptr;
      if (*ptr == '\n') {
         ++ptr;
         if (*ptr == 0)
            m_blocks[m_numBlocks++].m_pos = ptr - m_data;
      }
   } while (*ptr);
   m_blocks[m_numBlocks].m_pos = ptr - m_data + 1;
   
}

int AQTextDoc::positionOfPoint(const AQPoint &p) const
{
   if (p.y < 0)
      return 0;

   int b = p.y / m_lineHeight;
   int posInBlock = (p.x + m_charWidth/2 + 1) / m_charWidth;

   if (b > m_numBlocks)
      return m_size - 1;

   int pos = m_blocks[b].m_pos;
   if (posInBlock >= m_blocks[b+1].m_pos - pos)
      posInBlock = m_blocks[b+1].m_pos - pos - 1;

   return pos + posInBlock;
}
