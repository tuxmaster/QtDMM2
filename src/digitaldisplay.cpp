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

#include "digitaldisplay.h"
#include "lcdnumber.h"
#include "unitstring.h"

#include <cmath>
#include <iostream>

DigitalDisplay::DigitalDisplay( QWidget *parent ) :
  DMMDisplay( parent ),
  m_showBar( true ),
  m_showDelta( true )
{
  calcSize();

  m_barAction = new QAction( tr("Bargraph"), this );
  m_barAction->setCheckable( true );
  m_barAction->setChecked( true );

  connect( m_barAction, SIGNAL( toggled( bool ) ),
		   this, SLOT( showBarSLOT( bool ) ));

  addAction( m_barAction );

  m_deltaAction = new QAction( tr("Delta Min/Max"), this );
  m_deltaAction->setCheckable( true );
  m_deltaAction->setChecked( true );

  connect( m_deltaAction, SIGNAL( toggled( bool ) ),
		   this, SLOT( showDeltaSLOT( bool ) ));

  addAction( m_deltaAction );

  m_averageAction = new QAction( tr("Average"), this );
  m_averageAction->setCheckable( true );
  m_averageAction->setChecked( true );

  connect( m_averageAction, SIGNAL( toggled( bool ) ),
		   this, SLOT( showAverageSLOT( bool ) ));

  addAction( m_averageAction );

  m_histogramAction = new QAction( tr("Histogram"), this );
  m_histogramAction->setCheckable( true );
  m_histogramAction->setChecked( true );

  connect( m_histogramAction, SIGNAL( toggled( bool ) ),
		   this, SLOT( showHistogramSLOT( bool ) ));

  addAction( m_histogramAction );

}

DigitalDisplay::~DigitalDisplay()
{
}

void DigitalDisplay::showBarSLOT( bool on )
{
  m_showBar = on;
  invalidate();
  calcSize();
  update();
}

void DigitalDisplay::showHistogramSLOT( bool on )
{
  m_showHistogram = on;
  invalidate();
  calcSize();
  update();
}

void DigitalDisplay::showDeltaSLOT( bool on )
{
  std::cerr << "show delta: " << on << std::endl;
  m_showDelta = on;
  invalidate();
  calcSize();
  update();
}

void DigitalDisplay::calcSize()
{
  LCDNumber lcd( false, numDigits() );
  m_smallFont = QFont( "Helvetica", 11, QFont::DemiBold );

  m_font = QFont( "Helvetica", 16, QFont::DemiBold );
  computeUnitWidth();

  m_offset = 10;
  m_minMaxOffset = 0;

  if (m_showMinMax)
  {
	QFontMetrics fm( m_smallFont );
	m_minMaxOffset = 16;//std::max( fm.width( "Min:" ), fm.width( "Max:" ) ) + 2;
	int minW = m_minMaxOffset + fm.width( "dBm" );
	LCDNumber slcd( true, numDigits() );

	m_offset = minW+12+slcd.width();
  }

  int minW = lcd.width()+12+m_uw+m_offset;
  setMinimumWidth( minW );

  int minH;
  if (m_showBar)
  {
	minH = lcd.height()+8+30+m_titleHeight;
  }
  else
  {
	minH = lcd.height()+8+m_titleHeight;
  }

  if (m_showDelta && m_showMinMax) minH += 20;

  if (m_showHistogram) minH += 32;

  setMinimumHeight( minH );
  resize( minW, minH );
}

void DigitalDisplay::drawBackground()
{
  m_bg = QPixmap( width(), height() );
  m_bg.fill( m_colors.background() );

  QPainter p;
  p.begin( &m_bg );

  if (m_showTitle)
  {
	p.setFont( QFont( "Helvetica", 9, QFont::DemiBold ));
	p.setPen( Qt::black );

	p.fillRect( 0, 0, width(), m_titleHeight, m_colors.background().darker( 115 ) );
	p.drawText( 4, 2, width()-8, m_titleHeight, Qt::AlignLeft | Qt::AlignVCenter,
				"Voltcraft VC820 @ /dev/ttyUSB0" );
  }

  if (m_showBar)
  {
	p.setFont( QFont( "Helvetica", 7 ) );

	p.setPen( QPen( Qt::black, 1.0 ));

	int step = 10;
	while (step*m_barScale<3)
	{
	  step *= 10;
	}

	for (int i=0; i<=numCounts(); i+= step)
	{
	  if (0==(i%(10*step)))
	  {
		QLineF line( 14.0+(i*m_barScale), height()-18, 14.0+(i*m_barScale), height()-12 );
		p.drawLine( line );
	  }
	  else if (0==(i%(5*step)))
	  {
		QLineF line( 14.0+(i*m_barScale), height()-17, 14.0+(i*m_barScale), height()-12 );
		p.drawLine( line );
	  }
	  else
	  {
		QLineF line( 14.0+(i*m_barScale), height()-15, 14.0+(i*m_barScale), height()-12 );
		p.drawLine( line );
	  }
	}

	while (step*m_barScale<20)
	{
	  step *= 10;
	}

	// tick numbers
	for (int i=0; i<=numCounts(); i+= step)
	{
	  QString str;
	  str.sprintf( "%d", 10*i/step );
	  QRectF rect( 14.0+(i*m_barScale)-30.0, height()-30, 60, 12 );
	  p.drawText( rect, Qt::AlignTop | Qt::AlignHCenter, str );
	}
  }

  p.end();

  m_bgValid = true;
}

void DigitalDisplay::resizeEvent( QResizeEvent *ev )
{
  LCDNumber lcd( false, numDigits() );
  if (width() != lcd.width()+12+m_uw+m_offset)
  {
	m_offset = width()-lcd.width()-12-m_uw;
  }

  m_barScale = (double)(width()-28) / (double)(numCounts()-1);

  invalidate();
  resetHistogram();
  DMMDisplay::resizeEvent( ev );
}

void DigitalDisplay::paintEvent( QPaintEvent * )
{
  if (!m_bgValid) drawBackground();

  QPainter p;
  p.begin( this );
  p.drawPixmap( 0, 0, m_bg );

  p.setRenderHint( QPainter::Antialiasing, true );
  p.setRenderHint( QPainter::TextAntialiasing, true );

  p.save();
  p.translate( 0, m_titleHeight );

  if (m_showMinMax)
  {
	LCDNumber slcd( true, numDigits() );

	p.save();
	p.setFont( QFont( "Helvetica", 13, QFont::DemiBold ) );
	//QString str = QChar( 0x00d8 );
	QString str = QChar( 0x2798 );
	p.drawText( 4, 6, m_offset, slcd.height(),
				Qt::AlignLeft | Qt::AlignBottom, str );
	//str = QChar( 0x03c3 );
	str = QChar( 0x279a );
	p.drawText( 4, 6+4+slcd.height(), m_offset, slcd.height(),
				Qt::AlignLeft | Qt::AlignBottom, str );
	p.restore();
	/*
	p.drawText( 4, 4, m_offset, slcd.height(),
				Qt::AlignLeft | Qt::AlignBottom, "Min:" );
	p.drawText( 4, 4+4+slcd.height(), m_offset, slcd.height(),
				Qt::AlignLeft | Qt::AlignBottom, "Max:" );
	*/
	//p.drawPixmap( 4, 10, *s_minPix );
	//p.drawPixmap( 4, 10+4+slcd.height(), *s_maxPix );

	slcd.draw( &p, 4+m_minMaxOffset, 4, m_minString );
	slcd.draw( &p, 4+m_minMaxOffset, 4+4+slcd.height(), m_maxString );

	p.drawText( 4+m_minMaxOffset+slcd.width()+4, 4, m_offset, slcd.height(),
				Qt::AlignLeft | Qt::AlignBottom, UnitString::toQString( m_minUnit ));
	p.drawText( 4+m_minMaxOffset+slcd.width()+4, 4+4+slcd.height(), m_offset, slcd.height(),
				Qt::AlignLeft | Qt::AlignBottom, UnitString::toQString( m_maxUnit ) );

	if (m_showDelta)
	{
	  /*
	  p.drawText( 4, 4+8+2*slcd.height(), m_offset, slcd.height(),
				  Qt::AlignLeft | Qt::AlignBottom, "Delt:" );
	  */
	  p.save();
	  p.setFont( QFont( "Helvetica", 13, QFont::DemiBold ) );
	  str = QChar( 0x0394 );
	  p.drawText( 6, 6+8+2*slcd.height(), m_offset, slcd.height(),
				  Qt::AlignLeft | Qt::AlignBottom, str );
	  p.restore();

	  double delta = 1000.0 * (m_maxValue-m_minValue);
	  QString str;
	  str.sprintf( "%g", delta );

	  slcd.draw( &p, 4+m_minMaxOffset, 4+8+2*slcd.height(), str );

	  p.drawText( 4+m_minMaxOffset+slcd.width()+4, 4+8+2*slcd.height(),
				  m_offset, slcd.height(),
				  Qt::AlignLeft | Qt::AlignBottom,
				  UnitString::toQString( "mV" ) );
	}
  }

  LCDNumber lcd( false, numDigits() );
  p.setPen( Qt::black );
  lcd.draw( &p, 4+m_offset, 4, m_value );

  QString str;
  p.setFont( m_font );
  if (m_mode == "AC" || m_mode == "DC")
  {
	p.drawText( m_offset+lcd.width()+8, 4, m_uw, lcd.height(),
				Qt::AlignLeft | Qt::AlignTop, m_mode );
  }
  else if (m_mode == "DI")
  {
	p.drawPixmap( m_offset+lcd.width()+8, 8, *s_diodePix );
  }
  p.drawText( m_offset+lcd.width()+8, 4, m_uw, lcd.height(),
			  Qt::AlignLeft | Qt::AlignBottom,
			  UnitString::toQString( m_unit ) );

  p.restore();

  if (m_showBar)
  {
	QRect rect( 13, height()-10, std::max( 1, (int)(m_counts*m_barScale) ), 4 );
	p.fillRect( rect, Qt::black );
  }
#if 1
  if (m_showHistogram && m_histogramMax)
  {
	p.setPen( QPen( Qt::black, 0.5 ) );
	int w = (int)(m_numCounts*m_barScale);

	p.drawLine( 13, height()-30, 13+w, height()-30 );
	if (w<m_numCounts)
	{
	  double scale = 30.0 / (double)log10(m_histogramMax);

	  for (int i=0; i<w; ++i)
	  {
		if (m_histogram[i])
		{
		  p.drawLine( 13+i, height()-30, 13+i,
					  height()-30-(int)(scale*log10((double)m_histogram[i])));
		}
	  }

	  QString str;
	  str.sprintf( "%g", m_averageCount );
	  p.setFont( QFont( "Helvetica", 7 ) );
	  p.drawText( 17, height()-38, str );
	}
  }
#endif
  p.end();
}
