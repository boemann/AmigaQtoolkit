
#include <AQString.h>

#include "AQClipboard.h"

#include <devices/clipboard.h>

#include <proto/exec.h>

#define MAKE_ID(a,b,c,d) ((a<<24L) | (b<<16L) | (c<<8L) | d)

#define ID_FORM MAKE_ID('F','O','R','M')
#define ID_FTXT MAKE_ID('F','T','X','T')
#define ID_CHRS MAKE_ID('C','H','R','S')

class AQClipboardPrivate {
public:
   struct MsgPort *mp;
   struct IOClipReq *ior;
   struct Hook hook;
   struct Task *task;
   int seen;

   bool readLong(ULONG *ldata)
   {
      ior->io_Command = CMD_READ;
      ior->io_Data    = (STRPTR)ldata;
      ior->io_Length  = 4L;

      DoIO( (struct IORequest *) ior);
      if (ior->io_Actual == 4)
         return ior->io_Error == 0;

      return false;
   }

   void readDone()
   {
      char buffer[256];

      ior->io_Command = CMD_READ;
      ior->io_Data    = (STRPTR)buffer;
      ior->io_Length  = 254;

      while (ior->io_Actual) {
         if (DoIO( (struct IORequest *) ior))
             break;
      }
   }

   bool writeLong(int *ldata)
   {
      ior->io_Command = CMD_WRITE;
      ior->io_Data    = (STRPTR)ldata;
      ior->io_Length  = 4L;
      DoIO( (struct IORequest *) ior);

      if (ior->io_Actual == 4)
         return ior->io_Error == 0;

      return false;
   }

   bool setText( const AQString &t)
   {
      int length = t.size();
      if (length & 1)
         ++length;

      ior->io_Offset = 0;
      ior->io_Error  = 0;
      ior->io_ClipID = 0;

      // Create the IFF header information
      writeLong((int *) "FORM");     // "FORM"
      length += 12;                  // + "[size]FTXTCHRS"
      writeLong(&length);            // total length
      writeLong((int *) "FTXT");     // "FTXT"
      writeLong((int *) "CHRS");     // "CHRS"
      int slen = t.size();
      writeLong(&slen);              // string length

      // Write string
      ior->io_Data    = (STRPTR)t;
      ior->io_Length  = t.size();
      ior->io_Command = CMD_WRITE;
      DoIO( (struct IORequest *) ior);

      // Pad if needed
      if (t.size() & 1) {
         ior->io_Data   = (STRPTR)"";
         ior->io_Length = 1;
         DoIO( (struct IORequest *) ior);
      }

      // Tell the clipboard.device we are done writing
      ior->io_Command = CMD_UPDATE;
      DoIO((struct IORequest *) ior);

      return ior->io_Error == 0;
   }

   bool isFTXTAvail()
   {
     ULONG cbuff[4];

     // initial set-up for Offset, Error, and ClipID

     ior->io_Offset = 0;
     ior->io_Error  = 0;
     ior->io_ClipID = 0;

      // Look for "FORM[size]FTXT"

     ior->io_Command = CMD_READ;
     ior->io_Data    = (STRPTR)cbuff;
     ior->io_Length  = 12;
     DoIO( (struct IORequest *) ior);

     // Check to see if we have at least 12 bytes */
     if (ior->io_Actual == 12)
        // Check to see if it starts with "FORM"
        if (cbuff[0] == ID_FORM &&cbuff[2] == ID_FTXT)
            return true;

     readDone();

     return false;
   }

   AQString text()
   {
      if (!isFTXTAvail())
         return AQString();

      AQString resString;
      while (true)
      {
         ULONG chunk;
         ULONG size;

         if (readLong(&chunk) && readLong(&size)) {
            if (chunk == ID_CHRS) {
               if (size) {
                   char *buf = new char[size];
                   ior->io_Data    = (STRPTR)buf;
                   ior->io_Length  = size;
                   DoIO( (struct IORequest *) ior);
                   buf[ior->io_Actual] = 0;
                   resString += AQString(buf, ior->io_Actual);
                   delete buf;
               }
            } else {
               if (size & 1)
                  ++size;    // if odd size, add pad byte

               ior->io_Offset += size;
            }
         } else
            break;
      }

      readDone();

      return resString;
   }
};

static int reports=0;

ULONG clipChangedHookEntry()
{
   ++reports;
}

AQClipboard::AQClipboard()
   : m_d(new AQClipboardPrivate)
{
   m_d->seen = 0;
   const int unit = 0;
   if (m_d->mp = CreatePort(0L,0))
      if (m_d->ior = (IOClipReq *)CreateExtIO(m_d->mp,sizeof(struct IOClipReq)))
          OpenDevice((UBYTE *)"clipboard.device", unit, (IORequest *)m_d->ior, 0L);

    // Setup the clip hook
    m_d->ior->io_Data = (char *) &m_d->hook;
    m_d->ior->io_Length = 1;
    m_d->ior->io_Command = CBD_CHANGEHOOK;

    m_d->task = FindTask (NULL);

    m_d->hook.h_Entry = clipChangedHookEntry;
    m_d->hook.h_SubEntry = nullptr;
    m_d->hook.h_Data = m_d;

    DoIO((IORequest *)m_d->ior);
}

AQClipboard::~AQClipboard()
{
   // Remove the hook
   m_d->ior->io_Data = (char *) &m_d->hook;
   m_d->ior->io_Length = 0;
   m_d->ior->io_Command = CBD_CHANGEHOOK;
   DoIO((IORequest *)m_d->ior);


   CloseDevice((IORequest *)m_d->ior);
   DeleteExtIO((IORequest *)m_d->ior);
   DeletePort(m_d->mp);

   delete m_d;
}

void AQClipboard::checkUpdates()
{
   int d = reports - m_d->seen;
   m_d->seen += d;

   if (d) {
      bool has = m_d->isFTXTAvail();
      if (has)
         m_d->readDone();

      emit("changed", has);
   }
}

void AQClipboard::setText(const AQString &t)
{
   m_d->setText(t);
}

AQString AQClipboard::text() const
{
   return m_d->text();
}


