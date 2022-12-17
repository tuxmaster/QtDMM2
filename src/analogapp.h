#ifndef ANALOGAPP_HH
#define ANALOGAPP_HH

#include <QProcess>
#include <QApplication>
#include <analogdisplay.h>

class AnalogApp : public QApplication
{
  Q_OBJECT
      
  public:
    AnalogApp( int argc, char **argv );
    virtual ~AnalogApp();
    
  protected:
    AnalogDisplay *m_display;
    QProcess      *m_cdmm;
    
  protected slots:
    void readStdoutSLOT();
};

#endif // ANALOGAPP_HH
