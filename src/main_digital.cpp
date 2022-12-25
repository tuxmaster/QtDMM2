#include "plotdata.h"
#include "plotwid.h"
#include "analogdisplay.h"
#include "digitaldisplay.h"
#include "digitalapp.h"

int main( int argc, char **argv )
{
  DigitalApp app( argc, argv );
  return app.exec();
}
