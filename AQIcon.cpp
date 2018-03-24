
#include <graphics/gfxbase.h>

#ifdef __GNUC__
#include <proto/graphics.h>
#include <proto/layers.h>
#include <proto/icon.h>
#else
#include <pragma/graphics_lib.h>
#include <pragma/layers_lib.h>
#include <pragma/icon_lib.h>
#endif

#include <AQString.h>

#include "AQIcon.h"

class AQIconPrivate {
public:
   AQIconPrivate();
   ~AQIconPrivate();

   void init(int width, int height, int depth);
   void drawOnMask(bool d);
   void genMask();

   int refs;
   bool null;
   int width;
   BitMap bitMap;
   BitMap bitMapMask;
   BitMap bitMapFakeMask;
   RastPort rastPort;
   int storedPen;
};

AQIconPrivate::AQIconPrivate()
{
   refs = 1;
   null = true;
   storedPen = -1;
}

AQIconPrivate::~AQIconPrivate()
{
   if (!null) {
      /* Free the BitPlanes drawing area. */
      for(int plane = 0; plane < bitMap.Depth; plane++) {
         if (bitMap.Planes[plane])
            FreeRaster(bitMap.Planes[plane], width, bitMap.Rows);
      }
      if (bitMapMask.Planes[0])
         FreeRaster(bitMapMask.Planes[0], width, bitMap.Rows);
   }
}

void AQIconPrivate::init(int w, int height, int depth)
{
   width = w;

   InitBitMap(&bitMap, depth, width, height);
   InitBitMap(&bitMapMask, 1, width, height);
   InitBitMap(&bitMapFakeMask, depth, width, height);

   /* Set the plane pointers to nullptr so cleanup will know if they were used. */
   for(int i = 0; i < depth; ++i)
      bitMap.Planes[i] = nullptr;

   bitMapMask.Planes[0] = (PLANEPTR)AllocRaster(width, height);
   BltClear((UBYTE *)bitMapMask.Planes[0], bitMapMask.BytesPerRow * bitMapMask.Rows, 1L);

   for (int i = 0; i < depth; ++i) {
      bitMapFakeMask.Planes[i] = bitMapMask.Planes[0];
      bitMap.Planes[i] = (PLANEPTR)AllocRaster(width, height);
      if (bitMap.Planes[i] == nullptr)
         return;
//      BltClear((UBYTE *)bitMap.Planes[i], bitMap.BytesPerRow * bitMap.Rows, 1L);
   }

  InitRastPort(&rastPort);
  rastPort.BitMap = &bitMap;
}

void AQIconPrivate::drawOnMask(bool d)
{
   if (d && storedPen != -1)
      return;

   if (storedPen == -1 && !d)
      return;
   
   if (d) {
      storedPen = rastPort.FgPen;
      rastPort.BitMap = &bitMapMask;
      SetAPen(&rastPort, 1);
   } else {
      rastPort.BitMap = &bitMap;
      SetAPen(&rastPort, storedPen);
      storedPen = -1;
   }
}

void AQIconPrivate::genMask()
{
  BltBitMap(&bitMap, 0, 0, &bitMapFakeMask, 0 ,0, width, bitMap.Rows, 0xE0, 0xFF, nullptr);
}

AQIcon::AQIcon()
   : m_d(new AQIconPrivate)
{
}

AQIcon::AQIcon(const AQIcon &other)
   : m_d(other.m_d)
{
   m_d->refs++;
}

void convertFromChars(RastPort *rp, char str[][17])
{
   for (int y = 0; y <8; ++y) {
      for (int x = 0; x < 16; ++x) {
         if (str[y][x] == ' ')
            SetAPen(rp, 0);
         else
            SetAPen(rp, str[y][x] - '0');

         WritePixel(rp,x,y);
      }
   }
}

AQIcon::AQIcon(const AQString &fileName)
   : m_d(new AQIconPrivate)
{
   m_d->null = false;

   m_d->init(16, 8, 2);

   if (fileName == AQString("drawer")) {
      SetRast(&m_d->rastPort, 0);
      SetAPen(&m_d->rastPort, 2);
      RectFill(&m_d->rastPort, 0, 0, 15 , 5);
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 0, 0);
      Draw(&m_d->rastPort, 15, 0);
      Draw(&m_d->rastPort, 15, 5);
      Draw(&m_d->rastPort, 0, 5);
      Draw(&m_d->rastPort, 0, 0);

      Move(&m_d->rastPort, 4, 2);
      Draw(&m_d->rastPort, 4, 3);
      Draw(&m_d->rastPort, 11, 3);
      Draw(&m_d->rastPort, 11, 2);

      m_d->genMask();
   }

   if (fileName == AQString("AmigaTick")) {
      SetRast(&m_d->rastPort, 0);
      SetAPen(&m_d->rastPort, 2);
      RectFill(&m_d->rastPort, 0, 0, 15, 5);
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 1, 3);
      Draw(&m_d->rastPort, 3, 5);

      Move(&m_d->rastPort, 2, 3);
      Draw(&m_d->rastPort, 4, 5);

      Move(&m_d->rastPort, 4, 3);
      Draw(&m_d->rastPort, 6, 5);
      Draw(&m_d->rastPort, 11, 0);

      Move(&m_d->rastPort, 5, 3);
      Draw(&m_d->rastPort, 6, 4);
      Draw(&m_d->rastPort, 10, 0);

      Move(&m_d->rastPort, 8, 5);
      Draw(&m_d->rastPort, 13, 0);
      Move(&m_d->rastPort, 9, 5);
      Draw(&m_d->rastPort, 14, 0);

      m_d->genMask();
   }

   if (fileName == AQString("back")) {
      SetRast(&m_d->rastPort, 0);
      
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 1, 4);
      Draw(&m_d->rastPort, 13, 4);

      Move(&m_d->rastPort, 1, 4);
      Draw(&m_d->rastPort, 7, 1);
      Move(&m_d->rastPort, 1 ,4);
      Draw(&m_d->rastPort, 7, 7);

      m_d->genMask();
   }
   if (fileName == AQString("forward")) {
      SetRast(&m_d->rastPort, 0);
      
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 1, 4);
      Draw(&m_d->rastPort, 13, 4);

      Move(&m_d->rastPort, 13, 4);
      Draw(&m_d->rastPort, 7, 1);
      Move(&m_d->rastPort, 13 ,4);
      Draw(&m_d->rastPort, 7, 7);

      m_d->genMask();
   }
   if (fileName == AQString("levelup")) {
      SetRast(&m_d->rastPort, 0);
      
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 7, 1);
      Draw(&m_d->rastPort, 7, 7);

      Move(&m_d->rastPort, 1, 4);
      Draw(&m_d->rastPort, 7, 1);
      Move(&m_d->rastPort, 13 ,4);
      Draw(&m_d->rastPort, 7, 1);

      m_d->genMask();
   }
   if (fileName == AQString("right")) {
      SetRast(&m_d->rastPort, 0);
      
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 1, 0);
      Draw(&m_d->rastPort, 1, 4);
      Move(&m_d->rastPort, 2, 0);
      Draw(&m_d->rastPort, 2, 4);
      Move(&m_d->rastPort, 3, 1);
      Draw(&m_d->rastPort, 3, 3);
      Move(&m_d->rastPort, 4, 1);
      Draw(&m_d->rastPort, 4, 3);

      Move(&m_d->rastPort, 5, 2);
      Draw(&m_d->rastPort, 6, 2);

      m_d->genMask();
   }
   if (fileName == AQString("up")) {
      SetRast(&m_d->rastPort, 0);
      
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 0, 4);
      Draw(&m_d->rastPort, 6, 4);
      Move(&m_d->rastPort, 1, 3);
      Draw(&m_d->rastPort, 5, 3);
      Move(&m_d->rastPort, 2, 2);
      Draw(&m_d->rastPort, 4, 2);
      Move(&m_d->rastPort, 3, 1);
      Draw(&m_d->rastPort, 3, 1);

      m_d->genMask();
   }
   if (fileName == AQString("down")) {
      SetRast(&m_d->rastPort, 0);
      
      SetAPen(&m_d->rastPort, 1);
      Move(&m_d->rastPort, 0, 1);
      Draw(&m_d->rastPort, 6, 1);
      Move(&m_d->rastPort, 1, 2);
      Draw(&m_d->rastPort, 5, 2);
      Move(&m_d->rastPort, 2, 3);
      Draw(&m_d->rastPort, 4, 3);
      Move(&m_d->rastPort, 3, 4);
      Draw(&m_d->rastPort, 3, 4);

      m_d->genMask();
   }
   if (fileName == AQString("wholeword")) {
      char bit[8][17] = {
      "              ",
      " 111111111111 ",
      "  1111  11    ",
      " 11  11 11    ",
      " 111111 1111  ",
      " 11  11 11 11 ",
      " 11  11 1111  ",
      " 111111111111 "};
      convertFromChars(&m_d->rastPort, bit);

      m_d->genMask();
   }
   if (fileName == AQString("matchcase")) {
      char bit[8][17] = {
      "              ",
      "  1111        ",
      " 11  11       ",
      " 11  11  1111 ",
      " 111111     11",
      " 11  11  11111",
      " 11  11 11  11",
      " 11  11  11111"};
      convertFromChars(&m_d->rastPort, bit);

      m_d->genMask();
   }
   if (fileName == AQString("application")) {
      char bit[8][17] = {
      "    1111111   ",
      " 1111111      ",
      "  11  11      ",
      "       11     ",
      "        11    ",
      "         11   ",
      "          11  ",
      "              "};
      convertFromChars(&m_d->rastPort, bit);

      m_d->genMask();
   }
   if (fileName == AQString("blank")) {
      char bit[8][17] = {
      "              ",
      "              ",
      "              ",
      "              ",
      "              ",
      "              ",
      "              " ,
      "              ",};
      convertFromChars(&m_d->rastPort, bit);

      m_d->genMask();
   }
}

AQIcon::~AQIcon()
{
   if (--m_d->refs == 0)
      delete m_d;
}

bool AQIcon::isNull() const
{
   return m_d->null;
}

AQIcon &AQIcon::operator=(const AQIcon &other)
{
   m_d->refs--;
   if (m_d->refs == 0)
      delete m_d;
   m_d = other.m_d;
   m_d->refs++;

   return *this;
}

void AQIcon::paint(RastPort *rp, AQPoint pos, AQIcon::Size s)
{
   BltMaskBitMapRastPort(&m_d->bitMap, 0, 0, rp, pos.x, pos.y, 16, 8, 0xE0, m_d->bitMapMask.Planes[0]);
}
