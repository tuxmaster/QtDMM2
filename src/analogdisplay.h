//======================================================================
// File:        analogdisplay.h
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

#ifndef ANALOGWID_HH
#define ANALOGWID_HH

#include <QPixmap>
#include <dmmdisplay.h>

class AnalogDisplay : public DMMDisplay
{
  Q_OBJECT
      
  public:
    AnalogDisplay( QWidget *parent=0 );
    virtual ~AnalogDisplay();
    
  public slots:
    void showValueSLOT( bool );
    
  protected:
    QAction *m_valueAction;
    QAction *m_minMaxAction;
    QAction *m_resetAction;
    bool     m_showValue;
    
    void paintEvent( QPaintEvent * );
    void drawBackground();
    
    virtual void calcSize();
    
};

#endif // ANALOGWID_HH
