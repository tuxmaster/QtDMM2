//======================================================================
// File:        digitaldisplay.h
// Author:      Matthias Toussaint
// Created:     Fri Aug 17 23:49:56 CEST 2007
// Project:     QtDMM
// Description: Analog display
//----------------------------------------------------------------------
// This file  may  be used under  the terms of  the GNU  General Public
// License  version 2.0 as published   by the Free Software  Foundation
// and appearing  in the file LICENSE.GPL included  in the packaging of
// this file.
// 
// This file is provided AS IS with  NO WARRANTY OF ANY KIND, INCLUDING 
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE.
//----------------------------------------------------------------------
// Copyright 2007 Matthias Toussaint
//======================================================================

#ifndef DIGITALDISPLAY_HH
#define DIGITALDISPLAY_HH

#include <dmmdisplay.h>

class DigitalDisplay : public DMMDisplay
{
  Q_OBJECT
      
  public:
    DigitalDisplay( QWidget *parent=0 );
    virtual ~DigitalDisplay();
    
  protected:    
    QAction *m_barAction;
    QAction *m_deltaAction;
    QAction *m_averageAction;
    QAction *m_histogramAction;
    QFont    m_smallFont;
    int      m_offset;
    int      m_minMaxOffset;
    bool     m_showBar;
    bool     m_showDelta;
    
    void paintEvent( QPaintEvent * );
    void resizeEvent( QResizeEvent * );
    
    void drawBackground();
   
    virtual void calcSize();

  protected slots:
    void showBarSLOT( bool );
    void showDeltaSLOT( bool );
    void showHistogramSLOT( bool );
};

#endif // DIGITALDISPLAY_HH
