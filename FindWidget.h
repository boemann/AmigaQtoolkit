#include <AQWidget.h>
#include <vector.h>

class AQLayout;
class AQTextEdit;
class AQLineEdit;
class AQButton;
class AQTextCursor;

class FindWidget : public AQWidget
{
public:
   FindWidget(AQTextEdit *textEdit, AQWidget *parent = nullptr);
   ~FindWidget();

   void toggleReplaceMode();
   void setWholeWordMode(int on);
   
   void setDocument(AQTextDoc *doc, AQTextCursor *caret, int activeMode);
   int currentActiveMode() const;

   void findNext();
   void findPrevious();
   void replace();
   void replaceAll();

   void onFindLineChanged();
   void onWholeWordChanged(int on);
   void onCaseSensitiveChanged(int on);

   void activateFind();
   void activateReplace();
   void deactivate();

private:
   void updateFindResults();
   void clearFindResults();
   void activate(bool updateCaret);

   int m_mode;
   
   AQLayout *m_layout;
   AQLineEdit *m_findLine;   
   AQLineEdit *m_replaceLine;
   AQButton *m_nextB;
   AQButton *m_replaceB;
   AQButton *m_replaceAllB;
  
   AQTextDoc *m_doc;
   AQTextCursor *m_caret;
   vector<AQTextCursor *> m_findResults;
   AQTextEdit *m_textEdit;
   int m_findFlag;
   int m_posWhenStart;
   bool m_suspendUpdates;
};
   
