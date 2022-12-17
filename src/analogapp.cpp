#include "analogapp.h"

#include <iostream>

AnalogApp::AnalogApp( int argc, char **argv ) :
  QApplication( argc, argv )
{
  m_display = new AnalogDisplay;
  m_display->setCounts( 4000 );
  m_display->setColor( Qt::green );
  m_display->show();

  m_cdmm = new QProcess( this );
  m_cdmm->setReadChannel( QProcess::StandardOutput );
  m_cdmm->start( "cdmm --port=/dev/ttyUSB1 --device=\"Voltcraft VC 820\" --interval=0.2" );

  connect( m_cdmm, SIGNAL( readyReadStandardOutput() ),
		   this, SLOT( readStdoutSLOT() ));
}

AnalogApp::~AnalogApp()
{
  delete m_display;
}

void AnalogApp::readStdoutSLOT()
{
  QString str = m_cdmm->readLine( 1024 );
  //std::cout << str.toLatin1().data();

  QStringList token = str.split( " " );

  if (token[0] == "V0:")
  {
	m_display->setMode( token[3] );
	m_display->setUnit( token[2] );
	m_display->setValue( token[1] );
  }
}
