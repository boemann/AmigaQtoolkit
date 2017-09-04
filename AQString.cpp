#include "AQString.h"
#include <string.h>
#include <stdio.h>

AQString::AQString()
   : m_d(new Private())
{ 
}

AQString::AQString(const AQString &o)
 : m_d(o.m_d)
{
    ++(m_d->refs);
}

AQString::AQString(const char *cstr, int len)
   : m_d(new Private())
{
   if (len == -1)
     len = strlen(cstr);
   else
      len = aqMin(strlen(cstr), len);

   if (len <= 0)
      return; // a null string

   m_d->size = len;
   m_d->data = new char[m_d->size + 1];
   strncpy(m_d->data, cstr, len);
   m_d->data[m_d->size] = 0;   
}

AQString::AQString(const UBYTE *cstr)
   : m_d(new Private())
{
   Private *p = m_d;
   m_d->size = strlen(reinterpret_cast<const char *>(cstr));
   m_d->data = new char[m_d->size + 1];
   strcpy(m_d->data, reinterpret_cast<const char *>(cstr));
}

AQString::AQString(const BSTR s)
   : m_d(new Private())
{
   char *bstr = (char*)BADDR(s);
   m_d->size = *(bstr++);
   m_d->data = new char[m_d->size + 1];
   strncpy(m_d->data, bstr, m_d->size);
   m_d->data[m_d->size] = 0;
}

AQString::AQString(char c)
   : m_d(new Private())
{
   m_d->size = 1;
   m_d->data = new char[m_d->size + 1];
   m_d->data[0] = c;
   m_d->data[1] = 0;
}

AQString AQString::number(int i)
{
   char t[20];
   sprintf(t, "%d", i);
   return AQString(t);
}

AQString::~AQString()
{
   Private *p = m_d;
   if (--m_d->refs == 0)
      delete m_d;
}

AQString &AQString::operator=(const AQString &other)
{
   m_d->refs--;
   if (m_d->refs == 0)
      delete m_d;

   m_d = other.m_d;
   m_d->refs++;

   return *this;
}
 
char AQString::operator[](int pos) const
{
   if (pos >= 0 && pos < m_d->size)
      return m_d->data[pos];
      
   return 0;
}

AQString::operator char*() const
{
   return m_d->data;
}

bool AQString::endsWith(const AQString &ending) const
{
   if (ending.isEmpty())
      return true;

   if (isNull())
      return false;

   if (ending.m_d->size > m_d->size)
      return false;

   return strcmp(&m_d->data[m_d->size - ending.m_d->size], ending.m_d->data) == 0;
}

bool AQString::contains(const AQString &sub) const
{

   if (sub.isEmpty())
      return true;

   if (isEmpty())
      return false;

   if (sub.m_d->size > m_d->size)
      return false;

   return strstr(m_d->data, sub.m_d->data) != nullptr;
}

bool AQString::operator==(const AQString &other) const
{
   if (m_d == other.m_d)
      return true;

   if (isNull() && other.isNull())
      return true;
      
   if (isNull() || other.isNull())
      return false;
      
   return strcmp(m_d->data, other.m_d->data) == 0;
}

bool AQString::operator<(const AQString &other) const
{
   if (m_d == other.m_d)
      return false;

   if (other.isNull())
      return false;
      
   if (isNull())
      return true;
      
   return strcmp(m_d->data, other.m_d->data) < 0;
}

AQString &AQString::operator+=(const AQString &o)
{
   Private *newD = new Private;
    
   newD->size = size() + o.size();
   newD->data = new char[newD->size + 1];
   strncpy(newD->data, m_d->data, size());
   strncpy(&newD->data[size()], o.m_d->data, o.size());
   newD->data[newD->size] = 0;

   if (--m_d->refs == 0)
      delete m_d;

   m_d = newD;

   return *this;
}

const AQString operator+(const AQString &s1, const AQString &s2)
{
   AQString ret(s1);
   ret += s2;
   return ret;
}
