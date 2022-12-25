//======================================================================
// File:        dmmdisplay.cpp
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

#include "dmmdisplay.h"
#include "unitstring.h"

#include <cmath>
#include <iostream>

#include "xpm/diode.xpm"
#include "xpm/diode_small.xpm"

QPixmap *DMMDisplay::s_diodePix = nullptr;
QPixmap *DMMDisplay::s_diodeSmallPix = nullptr;

DMMDisplay::DMMDisplay( QWidget *parent ) :
  QWidget( parent ),
  m_showTitle( true ),
  m_numCounts( 4000 ),
  m_numDigits( 4 ),
  m_bgValid( false ),
  m_uw( 0 ),
  m_scale( 1.0 ),
  m_showMinMax( true ),
  m_minValue( 1.0e40 ),
  m_maxValue( -1.0e40 ),
  m_hasValue( false ),
  m_decimalPoint( 0 ),
  m_showHistogram( true ),
  m_histogram( 0 ),
  m_barScale( 0.0 )
{
  //startTimer( 100 );
  m_titleHeight = 20;

  if (!s_diodePix)
  {
	s_diodePix = new QPixmap((const char **)diode_xpm);
	s_diodeSmallPix = new QPixmap((const char **)diode_small_xpm);
  }

  setContextMenuPolicy( Qt::ActionsContextMenu );

  m_value = "0";

  step = ((double)random()/(double)RAND_MAX) *
	  (random() > RAND_MAX/2 ? 1.0 : -1.0);
  value = 5.0 * ((double)random()/(double)RAND_MAX) *
	  (random() > RAND_MAX/2 ? 1.0 : -1.0);

  m_titleAction = new QAction( tr("Title"), this );
  m_titleAction->setCheckable( true );
  m_titleAction->setChecked( true );

  connect( m_titleAction, SIGNAL( toggled( bool ) ),
		   this, SLOT( showTitleSLOT( bool ) ));

  m_minMaxAction = new QAction( tr("Min/Max"), this );
  m_minMaxAction->setCheckable( true );
  m_minMaxAction->setChecked( true );

  connect( m_minMaxAction, SIGNAL( toggled( bool ) ),
		   this, SLOT( showMinMaxSLOT( bool ) ));

  m_resetAction = new QAction( tr("Reset"), this );

  connect( m_resetAction, SIGNAL( triggered() ),
		   this, SLOT( resetSLOT() ));

  addAction( m_titleAction );
  addAction( m_minMaxAction );
  addAction( m_resetAction );

  resetAverage();
}

DMMDisplay::~DMMDisplay()
{
}

void DMMDisplay::resetSLOT()
{
  resetMinMax();
  resetHistogram();
  resetAverage();

  setValue( m_value );
}

void DMMDisplay::resetMinMax()
{
  m_minValue = 1.0e40;
  m_maxValue = -1.0e40;
}

void DMMDisplay::resetAverage()
{
  m_averageSum = 0.0;
  m_averageCount = 0.0;
}

void DMMDisplay::showMinMaxSLOT( bool on )
{
  m_showMinMax = on;
  invalidate();
  calcSize();
  update();
}

void DMMDisplay::showTitleSLOT( bool on )
{
  m_showTitle = on;
  m_titleHeight = on ? 20 : 0;
  invalidate();
  calcSize();
  update();
}

void DMMDisplay::timerEvent( QTimerEvent * )
{
  value += step;

  if (std::abs(value) >= (double)numCounts() / 100.0)
  {
	value -= step;
	step *= -1.0;
  }

  if (positiveOnly() && value < 0.0)
  {
	value = 0.0;
	if (step < 0.0) step *= -1.0;
  }

  value = (double)(int)(value * 100.0);
  value /= 100.0;

  m_value=QString("%1").arg(value*10.0);
  setValue( m_value );
  update();
}

void DMMDisplay::createHistogram()
{
  delete [] m_histogram;
  m_histogram = new int [m_numCounts];

  resetHistogram();
}

void DMMDisplay::resetHistogram()
{
  m_histogramMax = 0;
  memset( m_histogram, 0, m_numCounts*sizeof(int) );
}

void DMMDisplay::addToHistogram( int counts )
{
  if (!m_histogram) return;

  int index = (int)(counts*m_barScale);
  m_histogram[index]++;
  if (m_histogram[index] > m_histogramMax) m_histogramMax = m_histogram[index];
}

void DMMDisplay::setCounts( int counts )
{
  m_numCounts = counts;
  m_numDigits = 3;

  createHistogram();

  if (m_numCounts>1000) ++m_numDigits;
  if (m_numCounts>10000) ++m_numDigits;
  if (m_numCounts>100000) ++m_numDigits;
  if (m_numCounts>1000000) ++m_numDigits;
  if (m_numCounts>10000000) ++m_numDigits;

  calcSize();

  update();
}

void DMMDisplay::setValue( const QString & value )
{
  m_value = value;

  double dValue = value.toDouble() * m_scale;

  // add leading zeros
  int digits = 0;
  for (int i=0; i<m_value.length(); ++i)
	if (m_value[i] != '.' && m_value[i] != '-')
	  ++digits;

  if (digits < numDigits())
  {
	bool negative = m_value[0] == '-';
	if (negative) m_value = m_value.mid(1);

	int len = numDigits()-digits;
	for (int i=0; i<len; ++i)
	{
	  m_value.prepend( "0" );
	}

	if (negative) m_value.prepend( "-" );
  }

  QString counts;
  for (int i=0; i<value.length(); ++i)
  {
	if (value[i] != '-' && value[i] != '.')
	{
	  counts += value[i];
	}
  }
  m_counts = counts.toInt();
  m_hasValue = true;

  m_averageSum += dValue;
  m_averageCount += 1.0;

  m_average = m_averageSum / m_averageCount;
  m_averageString=QString("%1").arg(m_average/m_scale );

  addToHistogram( m_counts );

  if (dValue < m_minValue)
  {
	m_minValue = dValue;
	m_minString = value;
	m_minCounts = m_counts;
	if (dValue<0.0) m_minCounts *= -1;
	m_minUnit = m_unit;
  }
  if (dValue > m_maxValue)
  {
	m_maxValue = dValue;
	m_maxString = value;
	m_maxCounts = m_counts;
	if (dValue<0.0) m_maxCounts *= -1;
	m_maxUnit = m_unit;
  }

  int decimalPoint = 0;

  for (int i=value.length(), dp=0; i>=0; --i, ++dp)
  {
	if (value[i] == '.')
	{
	  decimalPoint = dp;
	  break;
	}
  }

  if (m_decimalPoint != decimalPoint)
  {
	invalidate();
  }
  m_decimalPoint = decimalPoint;

  update();
}

void DMMDisplay::setUnit( const QString & unit )
{
  m_unit = unit;

  m_scale = 1.0;
  if (unit[0] == 'k') m_scale = 1.0e3;
  if (unit[0] == 'M') m_scale = 1.0e6;
  if (unit[0] == 'G') m_scale = 1.0e9;
  if (unit[0] == 'm') m_scale = 1.0e-3;
  if (unit[0] == 'u') m_scale = 1.0e-6;
  if (unit[0] == 'n') m_scale = 1.0e-9;
  if (unit[0] == 'p') m_scale = 1.0e-12;

  update();
}

void DMMDisplay::setMode( const QString & mode )
{
  m_mode = mode;

  update();
}

void DMMDisplay::computeUnitWidth()
{
  QFontMetrics fm( m_font );
  m_uw = fm.horizontalAdvance( "dBm" );
}

void DMMDisplay::setColor( const QColor & color )
{
  m_colors.setColor( color );
}

void DMMDisplay::resizeEvent( QResizeEvent * )
{
  invalidate();
}

bool DMMDisplay::positiveOnly() const
{
  if (m_mode == "AC" || m_mode == "FR" ||
	  m_mode == "CA" || m_mode == "OH") return true;

  return false;
}
