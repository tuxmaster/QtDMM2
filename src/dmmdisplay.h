//======================================================================
// File:        dmmdisplay.h
// Author:      Matthias Toussaint
// Created:     Fri Aug 17 23:49:56 CEST 2007
// Project:     QtDMM
// Description: Colors for display
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

#ifndef DISPLAY_HH
#define DISPLAY_HH

#include <QWidget>
#include <QAction>

#include <displaycolor.h>

class DMMDisplay : public QWidget
{
  Q_OBJECT
      
  public:
    DMMDisplay( QWidget *parent=0 );
    ~DMMDisplay();
       
    void setCounts( int counts );
    void setValue( const QString & );
    virtual void setUnit( const QString & );
    virtual void setMode( const QString & );
    void setColor( const QColor & );
    void setTitle( const QString & );
    
  public slots:
    void resetSLOT();
    void showMinMaxSLOT( bool );
    void showTitleSLOT( bool );

  protected:
    QAction     *m_minMaxAction;
    QAction     *m_resetAction;
    QAction     *m_titleAction;
    QString      m_title;
    bool         m_showTitle;
    int          m_titleHeight;
    int          m_numCounts;
    int          m_numDigits;
    QString      m_value;
    QString      m_unit;
    QString      m_mode;
    DisplayColor m_colors;
    QPixmap      m_bg;
    bool         m_bgValid;
    int          m_uw;
    QFont        m_font;
    double       m_scale;
    bool         m_showMinMax;
    double       m_minValue;
    double       m_maxValue;
    QString      m_minString;
    QString      m_maxString;
    QString      m_minUnit;
    QString      m_maxUnit;
    int          m_counts;
    int          m_minCounts;
    int          m_maxCounts;
    bool         m_hasValue;
    int          m_decimalPoint;
    double       m_averageSum;
    double       m_averageCount;
    double       m_average;
    QString      m_averageString;
    bool         m_showHistogram;
    int         *m_histogram;
    int          m_histogramMax;
    double       m_barScale;

    void timerEvent( QTimerEvent * );
    void resizeEvent( QResizeEvent * );

    void computeUnitWidth();
    
    int numDigits() const { return m_numDigits; }
    int numCounts() const { return m_numCounts; }
    bool positiveOnly() const;
    
    void invalidate() { m_bgValid = false; }
    
    virtual void calcSize() = 0;
    
    static QPixmap *s_diodePix;
    static QPixmap *s_diodeSmallPix;

    void createHistogram();
    void addToHistogram( int counts );
    
    void resetAverage();
    void resetMinMax();
    void resetHistogram();
    
    // DEBUG
    double step, value;
    
};

#endif // DISPLAY_HH
