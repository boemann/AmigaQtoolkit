#include <stdio.h>

#include <AQLayout.h>
#include <AQButton.h>
#include <AQIcon.h>
#include <AQLineEdit.h>
#include <AQTextDoc.h>

#include "FindWidget.h"


FindWidget::FindWidget(AQTextEdit *textEdit, AQWidget *parent)
   : AQWidget(parent)
   , m_mode(0)
   , m_doc(nullptr)
   , m_caret(nullptr)
   , m_textEdit(textEdit)
   , m_findFlag(0)
   , m_posWhenStart(-1)
   , m_suspendUpdates(false)
{
   setBgPen(-2); // no bg but we draw ourself

   m_layout = new AQLayout(true);

   AQLayout *foldL = new AQLayout(false);
   AQButton *m_foldout = new AQButton(true, this);
   m_foldout->setIcon(AQIcon("down"));
   foldL->addWidget(m_foldout);
   Connect<FindWidget>(m_foldout, "clicked", this, &FindWidget::toggleReplaceMode);

   m_layout->addItem(foldL);

   AQLayout *textL = new AQLayout(false);
   m_findLine = new AQLineEdit(this);  // Find text
   m_findLine->setTooltip("Term to find");
   textL->addWidget(m_findLine);
   Connect<FindWidget>(m_findLine->document(), "documentChanged", this, &FindWidget::onFindLineChanged);
   m_replaceLine = new AQLineEdit(this); // Replace text
   m_replaceLine->setTooltip("Term to put instead");
   textL->addWidget(m_replaceLine);

   m_layout->addItem(textL);

   AQLayout *buttonsL = new AQLayout(false);

   AQLayout *upperbuttonsL = new AQLayout(true);

   AQButton *m_prevB = new AQButton(true, this); // Find Previous button
   m_prevB->setTooltip("Find next");
   m_prevB->setIcon(AQIcon("find-previous"));
   upperbuttonsL->addWidget(m_prevB);
   Connect<FindWidget>(m_prevB, "clicked", this, &FindWidget::findPrevious);

   m_nextB = new AQButton(true, this); // Find Next button
   m_nextB->setTooltip("Find next");
   m_nextB->setIcon(AQIcon("find-next"));
   upperbuttonsL->addWidget(m_nextB);
   Connect<FindWidget>(m_nextB, "clicked", this, &FindWidget::findNext);

   AQButton *b = new AQButton(true, this);
   b->setTooltip("Match case");
   b->setIcon(AQIcon("matchcase"));
   b->setCheckable(true);
   upperbuttonsL->addWidget(b);
   Connect<FindWidget>(b, "clicked", this, &FindWidget::onCaseSensitiveChanged);

   b = new AQButton(true, this);
   b->setIcon(AQIcon("wholeword"));
   b->setTooltip("Match whole word");
   b->setCheckable(true);
   upperbuttonsL->addWidget(b);
   Connect<FindWidget>(b, "clicked", this, &FindWidget::onWholeWordChanged);

   buttonsL->addItem(upperbuttonsL);

   AQLayout *lowerbuttonsL = new AQLayout(true);

   m_replaceB = new AQButton(true, this);
   m_replaceB->setTooltip("Replace next");
   m_replaceB->setIcon(AQIcon("replace"));
   lowerbuttonsL->addWidget(m_replaceB);
   Connect<FindWidget>(m_replaceB, "clicked", this, &FindWidget::replace);

   m_replaceAllB = new AQButton(true, this);
   m_replaceAllB->setTooltip("Replace all");
   m_replaceAllB->setIcon(AQIcon("replaceall"));
   lowerbuttonsL->addWidget(m_replaceAllB);
   Connect<FindWidget>(m_replaceAllB, "clicked", this, &FindWidget::replaceAll);

   buttonsL->addItem(lowerbuttonsL);

   m_layout->addItem(buttonsL);

   setLayout(m_layout);

   setVisible(false);
}

FindWidget::~FindWidget()
{
}

void FindWidget::toggleReplaceMode()
{
   m_mode = 3-m_mode;

   activate(false);
}
   
void FindWidget::setDocument(AQTextDoc *doc, AQTextCursor *caret, int mode)
{
   clearFindResults();
 
   if (m_mode)
      Disconnect<FindWidget>(m_doc, "documentChanged", this, &FindWidget::updateFindResults);

   m_doc = doc;
   m_caret = caret;
   m_mode = mode;

   activate(false);

   if (m_mode)
      Connect<FindWidget>(m_doc, "documentChanged", this, &FindWidget::updateFindResults);
}

int FindWidget::currentActiveMode() const
{
   return m_mode;
}

void FindWidget::findNext()
{
   AQString searchString = m_findLine->text();
   AQTextCursor *result = m_doc->find(m_findLine->text(), 
                 m_caret->selectionStart() + 1, m_findFlag);

   if (result) {
      m_caret->setPosition(result->selectionStart());
      m_caret->setPosition(result->selectionEnd(), true);
      m_textEdit->ensureCursorVisible();
      m_textEdit->update();
      delete result;
   }
   m_textEdit->setFocus();
}

void FindWidget::findPrevious()
{
   AQString searchString = m_findLine->text();
   AQTextCursor *result = m_doc->find(m_findLine->text(), 
                 m_caret->selectionEnd() - 2, AQTextDoc::Backward);

   if (result) {
      m_caret->setPosition(result->selectionStart());
      m_caret->setPosition(result->selectionEnd(), true);
      m_textEdit->ensureCursorVisible();
      m_textEdit->update();
      delete result;
   }
   m_textEdit->setFocus();
}

void FindWidget::replace()
{
   if (m_findLine->text() == m_caret->selectedText()) {
      m_caret->insertText(m_replaceLine->text());
   }
   findNext();
}

void FindWidget::replaceAll()
{
   m_suspendUpdates = true;

   for (int i = 0; i < m_findResults.size(); ++i) {
      if (m_findLine->text() == m_findResults[i]->selectedText())
         m_findResults[i]->insertText(m_replaceLine->text());
   }

   m_suspendUpdates = false;
   clearFindResults();
}

void FindWidget::onFindLineChanged()
{
   AQString searchString = m_findLine->text();
   AQTextCursor *result = m_doc->find(m_findLine->text(), 
                 m_caret->selectionStart(), m_findFlag);

   if (result) {
      m_caret->setPosition(result->selectionStart());
      m_caret->setPosition(result->selectionEnd(), true);
      m_textEdit->ensureCursorVisible();
      m_textEdit->update();
      delete result;
   }
   updateFindResults();
}


void FindWidget::onWholeWordChanged(int on)
{
   if (on)
      m_findFlag = m_findFlag | AQTextDoc::WholeWord;
   else
      m_findFlag = m_findFlag & ~AQTextDoc::WholeWord;

   updateFindResults();
}

void FindWidget::onCaseSensitiveChanged(int on)
{
   if (on)
      m_findFlag = m_findFlag | AQTextDoc::CaseInsensitively;
   else
      m_findFlag = m_findFlag & ~AQTextDoc::CaseInsensitively;

   updateFindResults();
}

void FindWidget::activateFind()
{
   if (m_mode == 0)
      Connect<FindWidget>(m_doc, "documentChanged", this, &FindWidget::updateFindResults);

   m_mode = 1;

   activate(true);

   m_findLine->setFocus();
}

void FindWidget::activateReplace()
{
   if (m_mode == 0)
      Connect<FindWidget>(m_doc, "documentChanged", this, &FindWidget::updateFindResults);

   m_mode = 2;

   activate(true);

   m_findLine->setFocus();
}

void FindWidget::activate(bool updateCaret)
{
   m_replaceLine->setVisible(m_mode == 2);
   m_replaceB->setVisible(m_mode == 2);
   m_replaceAllB->setVisible(m_mode == 2);

   if (!m_doc)
      return;

   setVisible(m_mode > 0);

   if (m_mode) {
      if (updateCaret) {
         m_suspendUpdates = true;
         if (!m_caret->hasSelection())
            m_caret->select(AQTextCursor::WordUnderCursor);
         m_findLine->setText(m_caret->selectedText());
         m_findLine->mainCursor()->select(AQTextCursor::Document);;
         m_suspendUpdates = false;
      }
      updateFindResults();
   }
}

void FindWidget::deactivate()
{
   m_textEdit->setFocus();
   setVisible(false);

   if (m_mode)
      Disconnect<FindWidget>(m_doc, "documentChanged", this, &FindWidget::updateFindResults);

   m_mode = 0;
   clearFindResults();
}


void FindWidget::updateFindResults()
{
   if (m_suspendUpdates)
      return;

   clearFindResults();

   AQString searchString = m_findLine->text();
   AQTextCursor *result = m_doc->find(searchString, 0, m_findFlag);
   while (result) {
      m_findResults.push_back(result);
      result = m_doc->find(searchString, result->selectionStart()+1, m_findFlag);
   }

   m_textEdit->update();
}

void FindWidget::clearFindResults()
{
   for (int i = 0; i < m_findResults.size(); ++i) {
      delete m_findResults[i];
   }
   m_findResults.clear();

   m_textEdit->update();
}

