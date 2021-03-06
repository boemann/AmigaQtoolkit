#ifndef AQTEXTDOC_H
#define AQTEXTDOC_H

#include <vector.h>

#include <AQKernel.h>
#include <AQObject.h>
#include <AQString.h>

class AQTextDoc;
class AQCommand;

struct RastPort;
class TextCommandBase;
struct TextFont;
class AQSyntaxHighlighter;

class AQFormatRange
{
public:
   AQFormatRange(int fg, int bg, TextFont *font);
   virtual ~AQFormatRange();

   void setStartEnd(int start, int end);
   void render(RastPort *rp, char *data);

   inline AQFormatRange *next() const { return m_next; }

   void split(int at);
   void modifyWith(AQFormatRange *other);

private:
   int m_startInBlock;  // invalid when subclassed to cursor
   int m_endInBlock;     // invalid when subclassed to cursor
   int m_fgColor;
   int m_bgColor;
   TextFont *m_font;
   AQFormatRange *m_next;
};

class AQTextCursor : public AQFormatRange
{
public:
   AQTextCursor(AQTextDoc &doc);
   ~AQTextCursor();

   void insertText(const AQString &str);
   void insertBlock();

   enum SelectionType {
      WordUnderCursor,
      LineUnderCursor,
      BlockUnderCursor,
      Document
   };

   void setPosition(int pos, bool keepAnchor = false);
   void select(SelectionType type);
   AQString selectedText() const;
   void deleteLeft(); // traditional backspace button
   void deleteRight(); // traditional delet button
   void deleteSelection(); // if there is one
   void movePrevChar(bool keepAnchor = false);
   void moveNextChar(bool keepAnchor = false);
   void movePrevWord(bool keepAnchor = false);
   void moveNextWord(bool keepAnchor = false);
   void movePrevLine(bool keepAnchor = false);
   void moveNextLine(bool keepAnchor = false);
   void moveStartOfLine(bool keepAnchor = false);
   void moveEndOfLine(bool keepAnchor = false);
   void movePageUp(int visibleCount, bool keepAnchor = false);
   void movePageDown(int visibleCount, bool keepAnchor = false);

   int position() const;
   int positionInBlock() const;
   bool hasSelection() const;
   int selectionStart() const;
   int selectionEnd() const;

   friend class AQTextDoc;

private:
   struct State
   {
      State() : pos(), posInBlock(0), wishX(0), anchorPos(0), anchorInBlock(0){}
      int pos;
      int posInBlock;
      int wishX;
      int anchorPos;
      int anchorInBlock;
   } m_state;

   void dataPushed(int pos, int n, TextCommandBase *cmd);
   void dataDeleted(int pos, int n, TextCommandBase *cmd);
   void restoreUndoneState(TextCommandBase *cmd);
   void restoreDoneState(TextCommandBase *cmd);

   AQTextDoc &m_doc;
};

class AQTextBlock
{
public:
   int m_pos;
};

class AQTextDoc : public AQObject
{
public:
   AQTextDoc(AQObject *parent = nullptr);
   ~AQTextDoc();

   void loadFile(const AQString &filename);
   void setData(const AQString &text);
   AQString toString() const;

   bool saveFile(const AQString &filename) const;

   TextFont *defaultFont() const;
   void setDefaultFont(TextFont *font);

   // Command representing latest change - if you take it you own it
   // Listen to commandAvailable signal for notisfication
   AQCommand *takeLatestCommand();

   int height() const;
   int lineHeight() const;

   AQPoint pointOfPosition(int pos) const;
   int positionOfPoint(const AQPoint &p) const;

   void render(RastPort *rp, AQPoint docOffset, AQPoint botRight);
   void renderCursor(RastPort *rp, AQTextCursor *cursor,
                    AQPoint docOffset, AQPoint botRight);
   int blockNumber(int pos) const;

   enum FindFlags {
      Backward = 0x01,
      CaseInsensitively = 0x02,
      WholeWord = 0x04
   };
   AQTextCursor *find(const AQString &sub, int from, int flags = 0);

   AQTextBlock findBlockByLineNumber(int line) const;

   friend class AQTextCursor;
   friend class DeleteCommand;
   friend class InsertCommand;
   friend class AQSyntaxHighlighter;

private:
   void reHighlight(int block);
   void restoreUndoneStateToCursors(TextCommandBase *cmd);
   void restoreDoneStateToCursors(TextCommandBase *cmd);
   void addCursor(AQTextCursor *cursor);
   void removeCursor(AQTextCursor *cursor);

   void setLatestCommand(AQCommand *cmd);

   void pushData(int pos, int n, char *chars, bool createCommand = true);
   void deleteData(int pos, int n, bool createCommand = true);
   void updateBlocks(int start, int delta);

   void renderBlock(RastPort *rp, int x, int &yTop, int b);

   AQTextBlock m_blocks[5000];
   AQFormatRange *m_highlightRanges[5000];

   unsigned long m_numBlocks;

   char *m_data;
   unsigned long m_capacity;
   unsigned long m_size;
   vector<AQTextCursor *>m_cursors;
   int m_charWidth;
   AQCommand *m_latestCommand;
   TextFont *m_defaultFont;
   AQSyntaxHighlighter *m_highlighter;
};

#endif
