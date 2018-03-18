#ifndef AQTEXTDOC_H
#define AQTEXTDOC

#include <vector.h>

#include <AQKernel.h>
#include <AQObject.h>
#include <AQString.h>

class AQTextDoc;
struct RastPort;

class AQTextCursor
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
   void dataPushed(int pos, int n);
   void dataDeleted(int pos, int n);

   int m_pos;
   int m_posInBlock;
   int m_wishX;
   int m_anchorPos;
   int m_anchorInBlock;
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

   bool isModified() const;
   void setModified(bool m = true);

   int height() const;
   int lineHeight() const;

   void render(RastPort *rp, AQPoint docOffset, AQPoint botRight);
   int blockNumber(int pos);
   AQTextBlock findBlockByLineNumber(int line) const;

   int positionOfPoint(const AQPoint &p) const;

   friend class AQTextCursor;

private:
   void addCursor(AQTextCursor *cursor);
   void removeCursor(AQTextCursor *cursor);

   void pushData(int pos, int n);
   void deleteData(int pos, int n);
   void updateBlocks();

   AQTextBlock m_blocks[5000];
   unsigned long m_numBlocks;

   char *m_data;
   unsigned long m_capacity;
   unsigned long m_size;
   vector<AQTextCursor *>m_cursors;
   int m_lineHeight;
   int m_charWidth;
   bool m_modified;
};

#endif
