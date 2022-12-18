#include "digitalapp.h"

#include <iostream>

DigitalApp::DigitalApp( int argc, char **argv ) :
  QApplication( argc, argv )
{
  m_display = new DigitalDisplay;
  m_display->setCounts( 4000 );
  m_display->setColor( Qt::green );
  m_display->show();

  QStringList arg;
  arg << "--port=/dev/ttyUSB1" << "--device=\"Voltcraft VC 820\"" << "--interval=0.2";

  m_cdmm = new QProcess( this );
  m_cdmm->setReadChannel( QProcess::StandardOutput );
  m_cdmm->setArguments(arg);
  m_cdmm->setProgram("cdmm");
  m_cdmm->start();

  connect( m_cdmm, SIGNAL( readyReadStandardOutput() ),
		   this, SLOT( readStdoutSLOT() ));
}

DigitalApp::~DigitalApp()
{
  delete m_display;
}

void DigitalApp::readStdoutSLOT()
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
