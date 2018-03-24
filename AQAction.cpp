
#include <intuition/intuition.h>
#include <ctype.h>

#include <AQWidget.h>

#include <AQAction.h>
#include <stdio.h>

AQAction::AQAction(AQObject *parent)
   : AQObject(parent)
   , m_scCtrl(false)
   , m_scAmiga(false)
   , m_scAlt(false)
   , m_scShift(false)
   , m_scCode(0)
   , m_scClass(IDCMP_VANILLAKEY)
   , m_enabled(true)
{
} 

AQAction::~AQAction()
{
}

void AQAction::setEnabled(bool on)
{
   m_enabled = on;
}

bool AQAction::enabled() const
{
   return m_enabled;
}

void AQAction::setText(const AQString &text)
{
   m_text = text;
}

AQString AQAction::text() const
{
   return m_text;
}

void AQAction::setShortcut(const AQString &sc)
{
   m_shortcut = sc;
   m_scCtrl = sc.contains("Ctrl");
   m_scAmiga = sc.contains("Amiga");
   m_scAlt = sc.contains("Alt");
   m_scShift = sc.contains("Shift");

   m_scClass = IDCMP_VANILLAKEY;

   if (sc[sc.size() - 2] == '+')
      m_scCode = tolower(sc[sc.size() - 1]);
   else if (sc.contains("Esc"))
      m_scCode = 27;
   else {
      m_scClass = IDCMP_RAWKEY;
      if (sc.contains("F1"))
         m_scCode = AQWidget::Key_F1;
      else if (sc.contains("F2"))
         m_scCode = AQWidget::Key_F2;
      else if (sc.contains("F3"))
         m_scCode = AQWidget::Key_F3;
      else if (sc.contains("F4"))
         m_scCode = AQWidget::Key_F4;
      else if (sc.contains("F5"))
         m_scCode = AQWidget::Key_F5;
      else if (sc.contains("F6"))
         m_scCode = AQWidget::Key_F6;
      else if (sc.contains("F7"))
         m_scCode = AQWidget::Key_F7;
      else if (sc.contains("F8"))
         m_scCode = AQWidget::Key_F8;
      else if (sc.contains("F9"))
         m_scCode = AQWidget::Key_F9;
      else if (sc.contains("F10"))
         m_scCode = AQWidget::Key_F10;
      else if (sc.contains("Help"))
         m_scCode = AQWidget::Key_Help;
      else
         m_scCode = 0;
   }
}

AQString AQAction::shortcut() const
{
   return m_shortcut;
}

bool AQAction::matchShortcut(const IntuiMessage &msg)
{
   if (!m_scShift != !(msg.Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT)))
      return false;
   if (!m_scAmiga != !(msg.Qualifier & IEQUALIFIER_RCOMMAND))
      return false;
   if (!m_scAlt != !(msg.Qualifier & (IEQUALIFIER_LALT | IEQUALIFIER_RALT)))
      return false;
   if (!m_scCtrl != !(msg.Qualifier & IEQUALIFIER_CONTROL))
      return false;

   if (msg.Class == m_scClass) {
      ULONG code = m_scClass == IDCMP_VANILLAKEY ? tolower(msg.Code) : msg.Code;

      if (code == m_scCode) {
         trigger();
         return true;
      }
   }

   return false;
}

void AQAction::trigger()
{
   if (m_enabled)
      emit("triggered");
}



