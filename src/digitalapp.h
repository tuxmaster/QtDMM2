#ifndef DIGITALAPP_HH
#define DIGITALAPP_HH

#include <QtGui>
#include "digitaldisplay.h"

class DigitalApp : public QApplication
{
  Q_OBJECT

  public:
	DigitalApp( int argc, char **argv );
	virtual ~DigitalApp();

  protected:
	DigitalDisplay *m_display;
	QProcess       *m_cdmm;

  protected Q_SLOTS:
	void readStdoutSLOT();
};

#endif // DIGITALAPP_HH
