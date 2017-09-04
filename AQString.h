#ifndef AQSTRING_H
#define AQSTRING_H

#include <dos/dos.h>

#include <AQKernel.h>

class AQString
{
   class Private {
   public:
      Private() : size(0), refs(1), data(nullptr) {}
      ~Private() {delete [] data;}
      int size;
      int refs;
      char *data;
   };

public:
   AQString();
   AQString(const AQString &o);
   AQString(const char *, int len = -1);
   AQString(const UBYTE *s);
   AQString(const BSTR s);
   AQString(char c);

   static AQString number(int i);

   ~AQString();

   int size() const {return m_d->size;}
   bool isEmpty() const {return (m_d->size == 0) || (m_d->data[0]==0);}
   bool isNull() const {return m_d->data == nullptr;}
    
   bool endsWith(const AQString &ending) const;
   bool contains(const AQString &sub) const;

   AQString right(int n) const;

   operator char*() const;
   bool operator==(const AQString &other) const;
   bool operator<(const AQString &other) const;
   AQString &operator=(const AQString &other);
   AQString &operator+=(const AQString &other);
   char operator[](int i) const;

private:
   Private *m_d;
};

const AQString operator+(const AQString &s1, const AQString &s2);

   
#endif
