#include <stdio.h>
#include <stdlib.h>

#include <AQApplication.h>
#include "DevStudio.h"

extern "C" void main(int argc, char **argv)
{
   AQApplication app;

   DevStudio devStudio;
   devStudio.show();

   app.exec();
   exit(0);
}



