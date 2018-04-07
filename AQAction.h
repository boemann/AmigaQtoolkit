#ifndef AQACTION_H
#define AQACTION_H

#include <AQObject.h>
#include <AQString.h>

struct IntuiMessage;


class AQAction : public AQObject
{
public:
   AQAction(AQObject *parent = nullptr);
   ~AQAction();

   void setEnabled(bool on);
   bool enabled() const;
   
   void setCheckable(bool c);
   bool isCheckable() const;

   void setChecked(bool c);
   bool isChecked() const;

   void joinMutexWith(AQAction *member);
   bool isMutexed() const;

   void setText(const AQString &text);
   AQString text() const;

   void setShortcut(const AQString &sc);
   AQString shortcut() const;
   AQString shortcutSansQualifier() const;
   bool matchShortcut(const IntuiMessage &msg);

   int numQualifiers() const { return m_numQualifiers;}

   bool hasCtrlQualifier() const { return m_scCtrl; }
   bool hasAmigaQualifier() const { return m_scAmiga; }
   bool hasAltQualifier() const { return m_scAlt; }
   bool hasShiftQualifier() const { return m_scShift; }

   void trigger();

private:
   AQString m_shortcut;
   AQString m_sansQualifier;
   AQString m_text;
   int m_numQualifiers;
   bool m_scCtrl;
   bool m_scAmiga;
   bool m_scAlt;
   bool m_scShift;
   char m_scCode;
   bool m_enabled;
   bool m_checkable;
   bool m_checked;
   ULONG m_scClass;
   AQAction *m_prevMutex;
   AQAction *m_nextMutex;
};
   
#endif
