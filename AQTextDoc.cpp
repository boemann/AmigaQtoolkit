#include <algorithm>

#include <AQTextDoc.h>
#include <AQCommand.h>
#include <AQSyntaxHighlighter.h>

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <graphics/gfxbase.h>
#include <graphics/rastport.h>

#include <proto/graphics.h>
#include <proto/dos.h>

class TextCommandBase : public AQCommand
{
public:
   TextCommandBase()
   {
   }

   ~TextCommandBase()
   {
   }

   void storeCursorStates(AQTextCursor *curs,
          const AQTextCursor::State &undone, const AQTextCursor::State &done)
   {
      m_undone = undone;
      m_done = done;
   }

   void restoreUndoneState(AQTextCursor *curs, AQTextCursor::State &s)
   {
      s = m_undone;
   }

   void restoreDoneState(AQTextCursor *curs, AQTextCursor::State &s)
   {
      s = m_done;
   }

   AQTextCursor::State m_undone;
   AQTextCursor::State m_done;
};

class InsertCommand : public TextCommandBase
{
public:
   InsertCommand(AQTextDoc *doc, int pos, int n, char *data)
      : m_doc(doc)
      , m_pos(pos)
      , m_n(n)
      , m_data(new char[n])
      , m_mergable(n==1 && *data!= '\n')
   {
      for (int i = 0; i < n; ++i)
         m_data[i] = data[i];
   }

   InsertCommand(const InsertCommand &other)
      : m_doc(other.m_doc)
      , m_pos(other.m_pos)
      , m_n(other.m_n)
      , m_data(new char[m_n])
      , m_mergable(other.m_mergable)
   {
      for (int i = 0; i < m_n; ++i)
         m_data[i] = other.m_data[i];
   }

   ~InsertCommand()
   {
      delete m_data;
   }

   bool mergeWith(AQCommand *cmd)
   {
      InsertCommand *other = dynamic_cast<InsertCommand *>(cmd);

      if (!other)
         return false;

      if (other->m_doc != m_doc)
         return false;

      if (!other->m_mergable)
         return false;

      if (!m_mergable)
         return false;

      if (other->m_pos != m_pos + m_n)
         return false;

      char *oldData = m_data;
      m_data = new char[m_n+1];
      for (int i = 0; i < m_n; ++i) // copy old
         m_data[i] = oldData[i];
      m_data[m_n] = other->m_data[0];
      delete oldData;
      m_n++;

      m_done = other->m_done;
      return true;
   }

   AQString actionText()
   {
      return "typing";
   }

   void undo()
   {
      m_doc->deleteData(m_pos, m_n, false);
      m_doc->restoreUndoneStateToCursors(this);
      m_doc->emit("documentChanged", m_doc);
   }

   void redo()
   {
      m_doc->pushData(m_pos, m_n, m_data, false);
      m_doc->restoreDoneStateToCursors(this);
      m_doc->emit("documentChanged", m_doc);
   }

   AQTextDoc *m_doc;
   int m_pos;
   int m_n;
   char *m_data;
   bool m_mergable;
};

class DeleteCommand : public TextCommandBase
{
public:
   DeleteCommand(AQTextDoc *doc, int pos, int n, char *data)
      : m_doc(doc)
      , m_pos(pos)
      , m_n(n)
      , m_data(new char[n])
      , m_mergedInsert(nullptr)
   {
      for (int i = 0; i <  n; ++i)
         m_data[i] = data[i];
   }

   ~DeleteCommand()
   {
      delete m_data;
      delete m_mergedInsert;
   }

   bool mergeWith(AQCommand *cmd)
   {
      InsertCommand *otherInsert = dynamic_cast<InsertCommand *>(cmd);

      if (otherInsert) {
         if (otherInsert->m_doc != m_doc)
            return false;

         if (m_undone.pos == m_undone.anchorPos)
            return false;

         if (m_mergedInsert) {
            bool ret = m_mergedInsert->mergeWith(otherInsert);
            if (ret)
               m_done = m_mergedInsert->m_done;
            return ret;
          }
         if (otherInsert->m_pos != m_pos)
            return false;

         m_mergedInsert = new InsertCommand(*otherInsert);

         m_done = m_mergedInsert->m_done;

         return true;
      }

      return false; // no merge of deletes for now

      DeleteCommand *other = dynamic_cast<DeleteCommand *>(cmd);

      if (!other)
         return false;

      if (other->m_doc != m_doc)
         return false;

      if (m_undone.pos != m_undone.anchorPos) // was a selection delete
            return false;

      if (other->m_n > 1)
         return false;

      if (other->m_pos != m_pos)
         return false;

      if (other->m_pos != m_pos)
         return false;

      return true;
   }

   AQString actionText()
   {
      if (m_mergedInsert)
         return m_mergedInsert->actionText();

      return "delete";
   }

   void undo()
   {
      if (m_mergedInsert)
         m_mergedInsert->undo();

      m_doc->pushData(m_pos, m_n, m_data, false);
      m_doc->restoreUndoneStateToCursors(this);
      m_doc->emit("documentChanged", m_doc);
   }

   void redo()
   {
      m_doc->deleteData(m_pos, m_n, false);
      m_doc->restoreDoneStateToCursors(this);

      if (m_mergedInsert)
         m_mergedInsert->redo();
      m_doc->emit("documentChanged", m_doc);
   }

   AQTextDoc *m_doc;
   int m_pos;
   int m_n;
   char *m_data;
   InsertCommand *m_mergedInsert;
};

AQTextCursor::AQTextCursor(AQTextDoc &doc)
   : AQFormatRange(doc.m_cursors.size() ? 1 : 2,
                        doc.m_cursors.size() ? 7 : 3, nullptr)
   , m_doc(doc)
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

   m_doc.pushData(m_state.pos, addLen, str);

   m_doc.emit("documentChanged", &m_doc);
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::insertBlock()
{
   if (hasSelection())
      deleteSelection();

   m_doc.pushData(m_state.pos, 1, "\n");

   m_doc.emit("documentChanged", &m_doc);
   m_doc.emit("cursorPositionChanged", &m_doc);
}

int AQTextCursor::position() const
{
   return m_state.pos;
}

int AQTextCursor::positionInBlock() const
{
   return m_state.posInBlock;
}

bool AQTextCursor::hasSelection() const
{
   return m_state.pos != m_state.anchorPos;
}

int AQTextCursor::selectionStart() const
{
   return m_state.pos < m_state.anchorPos ? m_state.pos : m_state.anchorPos;
}

int AQTextCursor::selectionEnd() const
{
   return m_state.pos > m_state.anchorPos ? m_state.pos : m_state.anchorPos;
}

void AQTextCursor::setPosition(int pos, bool keepAnchor)
{
   if (pos >=0 && pos <= m_doc.m_size) {
      m_state.pos = pos;
      m_state.posInBlock = 0;
      while(m_state.pos - m_state.posInBlock > 0
              && m_doc.m_data[m_state.pos - m_state.posInBlock - 1] != '\n')
         ++m_state.posInBlock;

      m_state.wishX = m_state.posInBlock;

      if (!keepAnchor) {
         m_state.anchorPos = m_state.pos;
         m_state.anchorInBlock = m_state.posInBlock;
      }
      m_doc.emit("cursorPositionChanged", &m_doc);
   }
}

#define isAlnum(p) (isalnum(m_doc.m_data[p]) || m_doc.m_data[p] == '_')

void AQTextCursor::select(AQTextCursor::SelectionType type)
{
   switch(type) {
   case WordUnderCursor: {;
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;

      bool doSpace = m_doc.m_data[m_state.pos] == ' '
                  || m_doc.m_data[m_state.pos] == '\n'
                  || m_doc.m_data[m_state.pos] == '\t'
                  || m_doc.m_data[m_state.pos] == 0;
      bool doAlnum = isAlnum(m_state.pos);

      while (m_state.anchorInBlock) {
         if (doSpace) {
            if(m_doc.m_data[m_state.anchorPos - 1] != ' '
                  && m_doc.m_data[m_state.anchorPos - 1] != '\n'
                  && m_doc.m_data[m_state.anchorPos - 1] != '\t') {
               if (m_state.anchorPos == m_state.pos) {
                  doSpace = false;
                  doAlnum = isAlnum(m_state.anchorPos - 1);
               }
               else
                  break;
            }
         } else if (doAlnum) {
            if (!isAlnum(m_state.anchorPos - 1))
               break;
         } else
            break;

         --m_state.anchorPos;
         --m_state.anchorInBlock;
      }

      while (m_doc.m_data[m_state.pos]) {
         if (doSpace) {
            if(m_doc.m_data[m_state.pos] != ' '
                  && m_doc.m_data[m_state.pos] != '\t')
               break;
         } else if (doAlnum) {
            if (!isAlnum(m_state.pos))
               break;
         } else
            break;

         ++m_state.pos;
         ++m_state.posInBlock;
      }

      if (!doSpace && !doAlnum) {
         m_state.pos = m_state.anchorPos + 1;
         m_state.posInBlock = m_state.anchorInBlock + 1;
      }
      m_state.wishX = m_state.posInBlock;
      break;
   }
   
   default:
      m_state.anchorPos = m_state.pos - m_state.posInBlock;
      m_state.anchorInBlock = 0;
      while(m_doc.m_data[m_state.pos] && m_doc.m_data[m_state.pos] != '\n') {
         ++m_state.pos;
         ++m_state.posInBlock;
      }      
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

   if (m_state.pos)
      m_doc.deleteData(m_state.pos-1, 1);

   m_doc.emit("documentChanged", &m_doc);
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::deleteRight()
{
   if (hasSelection()) {
      deleteSelection();
      return;
   }

   if (m_doc.m_size > m_state.pos)
      m_doc.deleteData(m_state.pos, 1);

   m_doc.emit("documentChanged", &m_doc);
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::deleteSelection()
{
   int n;
   int pos;
   if (m_state.pos > m_state.anchorPos) {
      n = m_state.pos - m_state.anchorPos;
      pos = m_state.anchorPos;
   } else {
      n = m_state.anchorPos - m_state.pos;
      pos = m_state.pos;
   }
   if (n == 0)
      return;
   m_doc.deleteData(pos, n);

   m_doc.emit("documentChanged", &m_doc);
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveNextChar(bool keepAnchor)
{
   if (m_doc.m_size > m_state.pos) {
      if (m_doc.m_data[m_state.pos] == '\n')
         m_state.posInBlock = 0;
      else
         ++m_state.posInBlock;
      ++m_state.pos;
   }
   m_state.wishX = m_state.posInBlock;

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePrevChar(bool keepAnchor)
{
   if (m_state.pos) {
      --m_state.pos;
      if (m_state.posInBlock == 0) {
         char *cptr = m_doc.m_data + m_state.pos - 1;
         while (cptr >= m_doc.m_data && *cptr != '\n')
            --cptr;
         ++cptr;
         m_state.posInBlock = m_doc.m_data + m_state.pos - cptr;
      } else
         --m_state.posInBlock;
   }
   m_state.wishX = m_state.posInBlock;

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveNextWord(bool keepAnchor)
{
   if (m_doc.m_size > m_state.pos) {
      if (isAlnum(m_state.pos)) {
         while (m_doc.m_size > m_state.pos && isAlnum(m_state.pos)) {
            ++m_state.posInBlock;
            ++m_state.pos;
         }
      } else {
         if (m_doc.m_data[m_state.pos] == '\n')
            m_state.posInBlock = 0;
         else
            ++m_state.posInBlock;
         ++m_state.pos;
      }
   }
   
   // skip whitespace except newline
   while (m_doc.m_size > m_state.pos && (
             m_doc.m_data[m_state.pos] == ' '
          || m_doc.m_data[m_state.pos] == '\t')) {
      ++m_state.posInBlock;
      ++m_state.pos;
   }
   m_state.wishX = m_state.posInBlock;

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePrevWord(bool keepAnchor)
{
   if (m_state.pos && m_state.posInBlock == 0) {
      --m_state.pos;
      char *cptr = m_doc.m_data + m_state.pos - 1;
      while (cptr >= m_doc.m_data && *cptr != '\n')
         --cptr;
      ++cptr;
      m_state.posInBlock = m_doc.m_data + m_state.pos - cptr;
   }

   // skip whitespace except newline
   while (m_state.posInBlock && (
             m_doc.m_data[m_state.pos-1] == ' '
          || m_doc.m_data[m_state.pos-1] == '\t')) {
      --m_state.posInBlock;
      --m_state.pos;
   }

   if (m_state.posInBlock) {
      if (isAlnum(m_state.pos-1)) {
         while (m_state.posInBlock && isAlnum(m_state.pos-1)) {
            --m_state.posInBlock;
            --m_state.pos;
         }
      } else {
         --m_state.posInBlock;
         --m_state.pos;
      }
   }
   
   m_state.wishX = m_state.posInBlock;

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveNextLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_state.pos);

   if (block < m_doc.m_numBlocks - 1) {
      m_state.pos = m_doc.m_blocks[block+1].m_pos;
      m_state.posInBlock = 0;
      while (m_doc.m_size > m_state.pos) {
         if (m_doc.m_data[m_state.pos] == '\n')
            break;
         if (m_state.posInBlock == m_state.wishX)
            break;
         ++m_state.pos;
         ++m_state.posInBlock;
      }
   }

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePrevLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_state.pos);

   if (block > 0 ) {
      m_state.pos = m_doc.m_blocks[block-1].m_pos;
      m_state.posInBlock = 0;
      while (m_doc.m_size > m_state.pos) {
         if (m_doc.m_data[m_state.pos] == '\n')
            break;
         if (m_state.posInBlock == m_state.wishX)
            break;
         ++m_state.pos;
         ++m_state.posInBlock;
      }
   }

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveStartOfLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_state.pos);

   m_state.pos = m_doc.m_blocks[block].m_pos;
   m_state.posInBlock = 0;
   m_state.wishX = m_state.posInBlock;

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::moveEndOfLine(bool keepAnchor)
{
   int block = m_doc.blockNumber(m_state.pos);

   m_state.pos = m_doc.m_blocks[block+1].m_pos - 1;
   m_state.posInBlock = m_doc.m_blocks[block+1].m_pos - m_doc.m_blocks[block].m_pos - 1;
   m_state.wishX = m_state.posInBlock;

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePageUp(int visibleCount, bool keepAnchor)
{
   int block = m_doc.blockNumber(m_state.pos);

   if (block > 0 ) {
      block -= visibleCount;
      if (block < 0)
         block = 0;
      m_state.pos = m_doc.m_blocks[block].m_pos;
      m_state.posInBlock = 0;
      while (m_doc.m_size > m_state.pos) {
         if (m_doc.m_data[m_state.pos] == '\n')
            break;
         if (m_state.posInBlock == m_state.wishX)
            break;
         ++m_state.pos;
         ++m_state.posInBlock;
      }
   }

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::movePageDown(int visibleCount, bool keepAnchor)
{
   int block = m_doc.blockNumber(m_state.pos);

   if (block < m_doc.m_numBlocks - 1) {
      block += visibleCount;
      if (block >= m_doc.m_numBlocks - 1)
         block = m_doc.m_numBlocks - 1;
      m_state.pos = m_doc.m_blocks[block].m_pos;
      m_state.posInBlock = 0;
      while (m_doc.m_size > m_state.pos) {
         if (m_doc.m_data[m_state.pos] == '\n')
            break;
         if (m_state.posInBlock == m_state.wishX)
            break;
         ++m_state.pos;
         ++m_state.posInBlock;
      }
   }

   if (!keepAnchor) {
      m_state.anchorPos = m_state.pos;
      m_state.anchorInBlock = m_state.posInBlock;
   }
   m_doc.emit("cursorPositionChanged", &m_doc);
}

void AQTextCursor::dataPushed(int pos, int n, TextCommandBase *cmd)
{
   State undone = m_state; 

   if (pos <= m_state.pos) {
      m_state.pos += n;

      char *cptr = m_doc.m_data + m_state.pos - 1;
      while (cptr >= m_doc.m_data && *cptr != '\n')
         --cptr;
      ++cptr;
      m_state.posInBlock = m_doc.m_data + m_state.pos - cptr;
   }
   if (pos <= m_state.anchorPos) {
      m_state.anchorPos += n;

      char *cptr = m_doc.m_data + m_state.anchorPos - 1;
      while (cptr >= m_doc.m_data && *cptr != '\n')
         --cptr;
      ++cptr;
      m_state.anchorInBlock = m_doc.m_data + m_state.anchorPos - cptr;
   }
   m_state.wishX = m_state.posInBlock;

   cmd->storeCursorStates(this, undone, m_state);
}

void AQTextCursor::dataDeleted(int pos, int n, TextCommandBase *cmd)
{
   State undone = m_state;

   if (pos < m_state.pos) {
      int move = aqMin(n, m_state.pos - pos);
      m_state.pos -= move;

      char *cptr = m_doc.m_data + m_state.pos - 1;
      while (cptr >= m_doc.m_data && *cptr != '\n')
         --cptr;
      ++cptr;
      m_state.posInBlock = m_doc.m_data + m_state.pos - cptr;
   }
   if (pos < m_state.anchorPos) {
      int move = aqMin(n, m_state.anchorPos - pos);
      m_state.anchorPos -= move;

      char *cptr = m_doc.m_data + m_state.anchorPos - 1;
      while (cptr >= m_doc.m_data && *cptr != '\n')
         --cptr;
      ++cptr;
      m_state.anchorInBlock = m_doc.m_data + m_state.anchorPos - cptr;
   }
   m_state.wishX = m_state.posInBlock;

   cmd->storeCursorStates(this, undone, m_state);
}

void AQTextCursor::restoreUndoneState(TextCommandBase *cmd)
{
   cmd->restoreUndoneState(this, m_state);
}

void AQTextCursor::restoreDoneState(TextCommandBase *cmd)
{
   cmd->restoreDoneState(this, m_state);
}

AQTextDoc::AQTextDoc(AQObject *parent)
   : AQObject(parent)
   , m_data(nullptr)
   , m_size(0)
   , m_latestCommand(nullptr)
   , m_defaultFont(GfxBase->DefaultFont)
   , m_highlighter(nullptr)
{
   m_capacity = 1000;
   m_data = new char[m_capacity];
   m_data[0] = 0;
   strcpy(m_data, "");
   m_size=strlen(m_data);
   m_blocks[0].m_pos = 0;
   m_blocks[1].m_pos = 1;
   m_highlightRanges[0] = nullptr;
   m_highlightRanges[1] = nullptr;
   m_numBlocks = 1;
//   updateBlocks(0, 0);
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
   if (file == 0)
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
   updateBlocks(0, m_size);

   emit("documentChanged", this);}

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
   updateBlocks(0, m_size);

   emit("documentChanged", this);
}

TextFont *AQTextDoc::defaultFont() const
{
   return m_defaultFont;
}

void AQTextDoc::setDefaultFont(TextFont *font)
{
   m_defaultFont = font;
}

AQString AQTextDoc::toString() const
{
   return AQString(m_data, m_size);
}

AQCommand *AQTextDoc::takeLatestCommand()
{
   AQCommand *retval = m_latestCommand;
   m_latestCommand = nullptr;
   return retval;
}

void AQTextDoc::setLatestCommand(AQCommand *cmd)
{
   if (m_latestCommand)
      delete m_latestCommand;

   m_latestCommand = cmd;

   emit("commandAvailable", this);
}

int AQTextDoc::height() const
{
   return m_numBlocks * lineHeight();
}

int AQTextDoc::lineHeight() const
{
   return m_defaultFont->tf_YSize;
}

int AQTextDoc::blockNumber(int pos) const
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

static char *strrstr(const char *haystack, char *haystackEnd,
                     const char *needle, const char *needleEnd)
{
   if (haystack == nullptr || needle == nullptr)
      return nullptr;

   for ( ; haystackEnd >= haystack; --haystackEnd) {
      const char *n;
      char *h;
      for (h = haystackEnd, n = needleEnd; n >= needle &&( *h == *n); --h, --n) {
         // So far it's matching
      }
      if (n + 1 == needle)
         return h + 1;
   }
   return nullptr;
}

static char *strcasestr(char *haystack, const char *needle)
{
   if (haystack == nullptr || needle == nullptr)
      return nullptr;

   for ( ;*haystack; ++haystack) {
      const char *n;
      const char *h;
      for (h = haystack, n = needle; *n &&( toupper(*h) == toupper(*n)); ++h, ++n) {
         // So far it's matching
      }

      if (*n == 0)
         return haystack;
   }
   return nullptr;
}

AQTextCursor *AQTextDoc::find(const AQString &sub, int from, int flags)
{
   if (sub.isEmpty())
      return nullptr;

   if (from < 0 || from >= m_size)
      return nullptr;

   char *pos = m_data + from;
   while (true) {
      if (flags & Backward) {
         pos = strrstr(m_data, pos, (char *)sub, (char *)sub + sub.size()-1);
      } else {
         if (flags & CaseInsensitively)
            pos = strcasestr(pos, (char *)sub);
         else
            pos = strstr(pos, (char *)sub);
      }
      if (pos == nullptr)
         return nullptr;

      if (flags & WholeWord) {
         char *pre = pos - 1;
         char *end = pos + sub.size() - 1;
         char *post = end + 1;
         if (((isalnum(*pre) || *pre=='_') && (isalnum(*pos) || *pos =='_'))
          || ((isalnum(*end) || *end=='_') && (isalnum(*post) || *post =='_')))
         {
            if (flags & Backward)
               --pos;
            else
               ++pos;

            continue;
         }
      }
      break;
   }

   AQTextCursor *cursor = new AQTextCursor(*this);
   cursor->setPosition(pos - m_data);
   cursor->setPosition(pos - m_data + sub.size(), true);
   return cursor;
}

AQTextBlock AQTextDoc::findBlockByLineNumber(int line) const
{ 
   if (line >= m_numBlocks)
      line = m_numBlocks - 1;

   return m_blocks[line];
}


void AQTextDoc::pushData(int pos, int n, char *chars, bool createCommand)
{
   InsertCommand *cmd = nullptr;

   if (createCommand) {
      cmd = new InsertCommand(this, pos, n, chars);
   }

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

   // Actually copy the data
   for (int i = 0; i < n; i++)
      m_data[pos + i] = chars[i];

   updateBlocks(pos, n);

   if (cmd) {
      for (int i = 0; i < m_cursors.size(); ++i)
         m_cursors[i]->dataPushed(pos, n, cmd);

      setLatestCommand(cmd);
   }
}

void AQTextDoc::deleteData(int pos, int n, bool createCommand)
{
   DeleteCommand *cmd = nullptr;

   if (createCommand) {
      cmd = new DeleteCommand(this, pos, n, m_data+pos);
   }

   for (int i = pos; i <  m_size - n + 1; ++i) {
         m_data[i] = m_data[i+n];
   }
   m_size -= n;

   updateBlocks(pos, -n);

   if (cmd) {
      for (int i = 0; i < m_cursors.size(); ++i)
         m_cursors[i]->dataDeleted(pos, n, cmd);

      setLatestCommand(cmd);
   }
}

void AQTextDoc::reHighlight(int b)
{
   if (b <= m_numBlocks)
      delete m_highlightRanges[b];
   m_highlightRanges[b] = nullptr;

   if (m_highlighter) {
      m_highlighter->setBlockNumber(b);
      m_highlighter->highlightBlock(AQString(&m_data[m_blocks[b].m_pos], m_blocks[b+1].m_pos - m_blocks[b].m_pos));
   }
}

void AQTextDoc::restoreUndoneStateToCursors(TextCommandBase *cmd)
{
   for (int i = 0; i < m_cursors.size(); ++i)
      m_cursors[i]->restoreUndoneState(cmd);
}

void AQTextDoc::restoreDoneStateToCursors(TextCommandBase *cmd)
{
   for (int i = 0; i < m_cursors.size(); ++i)
      m_cursors[i]->restoreDoneState(cmd);
}

void AQTextDoc::addCursor(AQTextCursor *cursor)
{
   m_cursors.push_back(cursor);
}

void AQTextDoc::removeCursor(AQTextCursor *cursor)
{
   m_cursors.erase(std::remove(m_cursors.begin(), m_cursors.end(), cursor), m_cursors.end());
}

void AQTextDoc::updateBlocks(int pos, int delta)
{  
   // Find first block that is changed
   int baseBlock = 0;

   while (pos && m_blocks[baseBlock+1].m_pos <= pos) {
      ++baseBlock;
   }

   // We assume that m_blocks[baseBlock] is correct;

   //Now calculate delta blocks
   int deltaBlocks = 0;
   char *ptr = m_data + pos;
   if (delta > 0) {
      int remaining = delta;
      do {
         while (remaining && *ptr && *ptr != '\n') {
            ++ptr;
            --remaining;
         }
         if (remaining && *ptr == '\n') {
            ++deltaBlocks;
            ++ptr;
            --remaining;
         }
      } while (remaining && *ptr);

      // update all subsequent blocks (always needed)
      int b = m_numBlocks;
      while (b > baseBlock) {
         m_blocks[b + deltaBlocks].m_pos = m_blocks[b].m_pos + delta;
         m_highlightRanges[b + deltaBlocks] = m_highlightRanges[b];
         --b;
      }

      while(b < baseBlock + deltaBlocks) // deref the ones we moved
         m_highlightRanges[++b] = nullptr;
      
      // Go through the added chars again to mark the new blocks
      remaining = delta;
      b = baseBlock;
      ptr = m_data + pos;
      do {
         while (remaining && *ptr && *ptr != '\n') {
            ++ptr;
            --remaining;
         }
         if (remaining && *ptr == '\n') {
            ++ptr;
            --remaining;
            ++b;
            m_blocks[b].m_pos = ptr - m_data;

            reHighlight(b-1);
         }
      } while (remaining>0 && *ptr);
      reHighlight(b);
      m_numBlocks += deltaBlocks;
   } else {
      while (m_blocks[baseBlock+deltaBlocks + 1].m_pos <= pos - delta)
         deltaBlocks++;

      // update all subsequent blocks (always needed)
      m_numBlocks -= deltaBlocks;
      int b = baseBlock + 1;
      while (b <= m_numBlocks) {
         m_blocks[b].m_pos = m_blocks[b + deltaBlocks].m_pos + delta;
         if (deltaBlocks && b < baseBlock + deltaBlocks)
            delete m_highlightRanges[b];
         m_highlightRanges[b] = m_highlightRanges[b+deltaBlocks];
         ++b;
      }
      reHighlight(baseBlock);
   }
}
