#include "AQString.h"
#include <string.h>
#include <stdio.h>

#include <proto/dos.h>

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

   if (len < 0)
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
   return indexOf(sub) != -1;
}

int AQString::indexOf(const AQString sub) const
{
   if (sub.isEmpty())
      return 0;

   if (isEmpty())
      return -1;

   if (sub.m_d->size > m_d->size)
      return -1;

   char *pos = strstr(m_d->data, sub.m_d->data);
   if (pos == nullptr)
      return -1;
   return  pos - m_d->data;
}

int AQString::lastIndexOf(const AQString sub, int from) const
{
   if (from >= 0)
      return -1;

   if (sub.isEmpty())
      return -1;

   if (isEmpty())
      return -1;

   if (sub.m_d->size > m_d->size)
      return -1;

   char *ps1 = m_d->data + m_d->size + 1 + from;
   char *pour;
   char *psub;
   while (ps1 != m_d->data) {
      --ps1;
      for (pour = ps1, psub = sub.m_d->data; ;) {
         if (*(pour++) != *(psub++))
            break;
         else
            if (*psub == 0)
               return ps1 - m_d->data;
      }
   }
   return -1;
}

AQString AQString::left(int n) const
{
   if (isEmpty())
      return AQString();

   return AQString(m_d->data, n);
}

AQString AQString::mid(int position, int n) const
{
   if (isEmpty())
      return AQString();

   if (position < 0 || position >= m_d->size)
      return AQString();

   return AQString(m_d->data + position, n);
}

AQString AQString::right(int n) const
{
   if (isEmpty())
      return AQString();

   if (n > m_d->size)
      return *this;
   return AQString(m_d->data + m_d->size - n);
}

const int maxdigits[36] = {31,15,15,9,9,9,7,9,9,8,8,8,8,8,7,7,7};

int AQString::toInt(bool *ok = nullptr, int base=10) const
{
   if (ok)
      *ok = false;

   if (isEmpty())
      return 0;

   int i = 0;
   bool negative = false;
   if (m_d->data[i] == '+')
      ++i;
   else if (m_d->data[i] == '-') {
      negative = true;
      ++i;
   } else if (m_d->data[i] < '0' || m_d->data[i] > ('0' + base - 1))
      return 0;

   int number = 0;
   while (m_d->data[i] >= '0' && m_d->data[i] < ('0' + base)) {
      number *= base;
      number += m_d->data[i] - '0';
      ++i;
   }
   
   if (ok)
      *ok = true;

   return negative ? -number : number;
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

AQString qCdUp(const AQString &path)
{
   char *data = path;
   char *endPtr = strrchr(data, '/');
   if (endPtr == nullptr) {
      endPtr = strrchr(data, ':');

      if (endPtr == nullptr)
         return AQString();
      ++endPtr;
   }

   return AQString(data, endPtr-data);
}

AQString qFileName(const AQString &path)
{
   char *data = path;
   char *startPtr = strrchr(data, '/');

   if (startPtr == nullptr)
      startPtr = strrchr(data, ':');

   if (startPtr == nullptr)
      return data;

   return startPtr+1;
}

bool qIsFolder(const AQString &name)
{
   bool result = false;
   BPTR lock = Lock(name, SHARED_LOCK);
   if (lock) {
      FileInfoBlock *fib = (FileInfoBlock *)AllocDosObject(DOS_FIB, nullptr);
      if (fib) {
         Examine(lock, fib);
         result = fib->fib_DirEntryType > 0;
         FreeDosObject(DOS_FIB, fib);
      }
      UnLock(lock);
   }
   return result;
}
