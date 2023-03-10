#include "plotdata.h"
#include <iostream>
#include "plotwid.h"
#include "analogdisplay.h"
#include "digitaldisplay.h"

int main( int argc, char **argv )
{
  srand( time(0) );
  
  QApplication app( argc, argv );
  
  AnalogDisplay awid;
  awid.setUnit( "mV" );
  awid.setMode( "DC" );
  awid.setCounts( 1000 );
  awid.setColor( Qt::red );
  awid.show();
  
  AnalogDisplay awid2;
  awid2.setUnit( "Hz" );
  awid2.setMode( "FR" );
  awid2.setCounts( 2000 );
  awid2.setColor( Qt::green );
  awid2.show();
  
  AnalogDisplay awid3;
  awid3.setUnit( "C" );
  awid3.setMode( "TE" );
  awid3.setCounts( 3000 );
  awid3.setColor( Qt::blue );
  awid3.show();
  
  AnalogDisplay awid4;
  awid4.setUnit( "kOhm" );
  awid4.setMode( "OH" );
  awid4.setCounts( 4000 );
  awid4.setColor( Qt::cyan );
  awid4.show();
  
  AnalogDisplay awid5;
  awid5.setUnit( "V" );
  awid5.setMode( "DI" );
  awid5.setCounts( 5000 );
  awid5.setColor( Qt::magenta );
  awid5.show();
  
  AnalogDisplay awid6;
  awid6.setUnit( "uA" );
  awid6.setMode( "AC" );
  awid6.setCounts( 6000 );
  awid6.setColor( Qt::darkGray );
  awid6.show();
  
  AnalogDisplay awid7;
  awid7.setUnit( "mW" );
  awid7.setMode( "PO" );
  awid7.setCounts( 8000 );
  awid7.setColor( Qt::yellow );
  awid7.show();
  
  // dicketal
  DigitalDisplay dwid;
  dwid.setUnit( "mV" );
  dwid.setMode( "DC" );
  dwid.setCounts( 1000 );
  dwid.setColor( Qt::red );
  dwid.show();
  
  DigitalDisplay dwid2;
  dwid2.setUnit( "Hz" );
  dwid2.setMode( "FR" );
  dwid2.setCounts( 2000 );
  dwid2.setColor( Qt::green );
  dwid2.show();
  
  DigitalDisplay dwid3;
  dwid3.setUnit( "nF" );
  dwid3.setMode( "CA" );
  dwid3.setCounts( 3000 );
  dwid3.setColor( Qt::blue );
  dwid3.show();
  
  DigitalDisplay dwid4;
  dwid4.setUnit( "MOhm" );
  dwid4.setMode( "OH" );
  dwid4.setCounts( 4000 );
  dwid4.setColor( Qt::cyan );
  dwid4.show();
  
  DigitalDisplay dwid5;
  dwid5.setUnit( "V" );
  dwid5.setMode( "DI" );
  dwid5.setCounts( 5000 );
  dwid5.setColor( Qt::magenta );
  dwid5.show();
  
  DigitalDisplay dwid6;
  dwid6.setUnit( "uA" );
  dwid6.setMode( "AC" );
  dwid6.setCounts( 6000 );
  dwid6.setColor( Qt::darkGray );
  dwid6.show();
  
  DigitalDisplay dwid7;
  dwid7.setUnit( "C" );
  dwid7.setMode( "TE" );
  dwid7.setCounts( 8000 );
  dwid7.setColor( Qt::yellow );
  dwid7.show();
  
  PlotWid wid;
  wid.create( 4, 1000 );
  wid.setName( 0, "Spannung" );
  wid.setName( 1, "Strom" );
  wid.setName( 2, "Leistung" );
  wid.setName( 3, "Gedoens" );
  wid.setUnit( 0, "V" );
  wid.setUnit( 1, "A" );
  wid.setUnit( 2, "W" );
  wid.setUnit( 3, "G" );
  
  QDateTime dt = QDateTime::currentDateTime();
  QDateTime dt2 = QDateTime::currentDateTime();
  QDateTime dt3 = QDateTime::currentDateTime();
  
  for (int i=0; i<1000; ++i)
  {
    wid.addValue( 0, dt, 2.340f*(float)i );
    wid.addValue( 1, dt, 0.003*(float)(25-i) );
    
    dt = dt.addMSecs( 100 );
  }

  for (int i=0; i<1000; ++i)
  {
    wid.addValue( 2, dt2, 
                  wid.data(0)->at(i)->value*wid.data(1)->at(i)->value );
    
    dt2 = dt2.addMSecs( 100 );
  }

  for (int i=0; i<1000; ++i)
  {
    wid.addValue( 3, dt3, random() );
    
    dt3 = dt3.addMSecs( 100 );
  }
    
  wid.show();
  return app.exec();
}
