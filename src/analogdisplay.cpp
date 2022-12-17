//======================================================================
// File:        analogdisplay.cpp
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

#include <analogdisplay.h>
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <cmath>
#include <lcdnumber.h>
#include <unitstring.h>

#include <iostream>

AnalogDisplay::AnalogDisplay( QWidget *parent ) :
  DMMDisplay( parent ),
  m_showValue( true )
{
  m_font = QFont( "Helvetica", 8, QFont::DemiBold );
  computeUnitWidth();
    
  m_valueAction = new QAction( tr("Value"), this );
  m_valueAction->setCheckable( true );
  m_valueAction->setChecked( true );
  
  connect( m_valueAction, SIGNAL( toggled( bool ) ),
           this, SLOT( showValueSLOT( bool ) ));
   
  addAction( m_valueAction );

  calcSize();
}

AnalogDisplay::~AnalogDisplay()
{
}

void AnalogDisplay::calcSize()
{
  setMinimumSize( 260, 100+m_titleHeight );
  resize( 260, 100+m_titleHeight );
}

void AnalogDisplay::showValueSLOT( bool on )
{
  m_showValue = on;
  update();
}

void AnalogDisplay::drawBackground()
{
  m_bg = QPixmap( width(), height() );
  
  QPainter p;
  p.begin( &m_bg );
  
  p.setRenderHint( QPainter::Antialiasing, true );
  p.setRenderHint( QPainter::TextAntialiasing, true );

  p.fillRect( rect(), m_colors.background() );
  
  p.setPen( m_colors.background().darker( 140 ) );
  p.setFont( QFont( "Helvetica", 6 ) );
  p.drawText( 0, m_titleHeight, width(), height()-10-m_titleHeight, 
              Qt::AlignCenter, "QtDMM 2" );
  p.drawText( 0, 10+m_titleHeight, width(), height()-10-m_titleHeight, 
              Qt::AlignCenter, "\251 M.Toussaint" );
  
  if (m_showTitle)
  {
    p.setFont( QFont( "Helvetica", 9, QFont::DemiBold ));
    p.setPen( Qt::black );
  
    p.fillRect( 0, 0, width(), m_titleHeight, m_colors.background().darker( 110 ) );
    p.drawText( 4, 2, width()-8, m_titleHeight, Qt::AlignLeft | Qt::AlignVCenter,
                "Voltcraft ME-32 @ /dev/ttyUSB1" );
  }
  
  QRect pRect = QRect( 0, 20, width()-40, height()-20-m_titleHeight );
  
  double r = pRect.height()*2.0;
  double aMax = asin( (pRect.width()/2.0) / r );
  
  p.translate( 0, m_titleHeight );
  
  if (positiveOnly())
  {
    double dAngle = 2*aMax;
    double ox = width()/2;
    double oy = pRect.y()+r;
    
    double aStep = dAngle/(numCounts()-1);
    double a=aMax;
    
    p.setPen( QPen( Qt::black, 0.5 ));
    
    int step = 1;
    
    while (aStep*(double)step < M_PI*(1.0/180.0))
    {
      step *= 10;
    }
    
    for (int i=0; i<=numCounts(); i+=step)
    {
      if (i==0 || i==numCounts())
      {
        p.setPen( QPen( Qt::black, 1.5 ));
        QLineF line( sin(a)*(r-6)+ox, oy-cos(a)*(r-6), sin(a)*(r+6)+ox, oy-cos(a)*(r+6) );
        p.drawLine( line );
        p.setPen( QPen( Qt::black, 0.5 ));
      }
      else if (0==(i%(5*step)))
      {
        p.setPen( QPen( Qt::black, 1.0 ));
        QLineF line( sin(a)*(r-6)+ox, oy-cos(a)*(r-6), sin(a)*(r+4)+ox, oy-cos(a)*(r+4) );
        p.drawLine( line );
        p.setPen( QPen( Qt::black, 0.5 ));
      }
      else
      {
        QLineF line( sin(a)*(r-6)+ox, oy-cos(a)*(r-6), sin(a)*(r)+ox, oy-cos(a)*(r) );
        p.drawLine( line );
      }
      a -= (double)step*aStep;
    }
    
    p.setFont( QFont( "Helvetica", 7 ) );
    a = -aMax;
    step *= 5;
    
    double scaleFac = 0.5;
    for (int i=m_decimalPoint; i<numDigits(); ++i) scaleFac *= 10;
    
    if (step*aStep*180.0/M_PI < 10.0) 
    {
      scaleFac *= 2.0;
      step *= 2;
    }
    
    for (int i=0; i<=numCounts(); i+=step)
    {
      QString str; 
      
      p.save();
      p.translate( ox, oy );
      p.rotate( (180.0*a)/M_PI );
      str.sprintf( "%d", (int)(scaleFac*i/step) );
      p.drawText( -15, (int)rintf(-r-18), 30, 15, Qt::AlignCenter, str );
      p.restore();
       
      a += (double)step*aStep;
    }
  }
  else
  {
    double dAngle = 2.0*aMax;
    double ox = width()/2;
    double oy = pRect.y()+r;
    
    double aStep = dAngle/(numCounts()-1);
    double a=-aMax;
    
    p.setPen( QPen( Qt::black, 0.5 ));
    
    int step = 1;
    
    while (aStep*(double)step < M_PI*(1.0/180.0))
    {
      step *= 10;
    }
    
    for (int i=0; i<=numCounts(); i+=step)
    {
      if (i==0 || i==numCounts() || i==numCounts()/2)
      {
        p.setPen( QPen( Qt::black, 1.5 ));
        QLineF line( sin(a)*(r-6)+ox, oy-cos(a)*(r-6), sin(a)*(r+6)+ox, oy-cos(a)*(r+6) );
        p.drawLine( line );
        p.setPen( QPen( Qt::black, 0.5 ));
      }
      else if (0==(i%(5*step)))
      {
        p.setPen( QPen( Qt::black, 1.0 ));
        QLineF line( sin(a)*(r-6)+ox, oy-cos(a)*(r-6), sin(a)*(r+4)+ox, oy-cos(a)*(r+4) );
        p.drawLine( line );
        p.setPen( QPen( Qt::black, 0.5 ));
      }
      else
      {
        QLineF line( sin(a)*(r-6)+ox, oy-cos(a)*(r-6), sin(a)*(r)+ox, oy-cos(a)*(r) );
        p.drawLine( line );
      }
      a += (double)step*aStep;
    }
    
    p.setFont( QFont( "Helvetica", 7 ) );
    a = 0.0;
    step *= 5;
    
    int scaleFac = 1;
    for (int i=m_decimalPoint; i<numDigits(); ++i) scaleFac *= 10;
    
    if (step*aStep*180.0/M_PI < 10.0) 
    {
      scaleFac *= 2;
      step *= 2;
    }

    for (int i=numCounts()/2; i<=numCounts(); i+=step)
    {
      QString str; 
      
      p.save();
      p.translate( ox, oy );
      p.rotate( (180.0*a)/M_PI );
      str.sprintf( "%d", scaleFac*(i-numCounts()/2)/step );
      p.drawText( -15, (int)rintf(-r-18), 30, 15, Qt::AlignCenter, str );
      p.restore();
      
      if (i!= numCounts()/2)
      {
        p.save();
        p.translate( ox, oy );
        p.rotate( -(180.0*a)/M_PI );
        str.sprintf( "-%d", scaleFac*(i-numCounts()/2)/step );
        p.drawText( -15, (int)rintf(-r-18), 30, 15, Qt::AlignCenter, str );
        p.restore();
      }
      
      a += (double)step*aStep;
    }
  }
  
  p.end();
  m_bgValid = true;
}

void AnalogDisplay::paintEvent( QPaintEvent * )
{
  if (!m_bgValid) drawBackground();
  
  QPainter p;
  p.begin( this );
  p.drawPixmap( 0, 0, m_bg );
  
  p.setRenderHint( QPainter::Antialiasing, true );
  p.setRenderHint( QPainter::TextAntialiasing, true );
  
  QRect pRect = QRect( 0, 20, width()-40, height()-20-m_titleHeight );
  
  double r = pRect.height()*2.0;
  double aMax = asin( (pRect.width()/2.0) / r );
  double vFac = aMax/1.0/(double)(numCounts()-1);
  
  double value = m_counts;
  double angle;
  if (m_value.toDouble() < 0) value *= -1.0;
  
  double ox = width()/2;
  double oy = pRect.y()+r;

  p.save();
  p.translate( 0, m_titleHeight );
  
  if (m_showMinMax && m_hasValue)
  {
    QColor color = m_colors.foreground();
    if (color.red() == color.green() && color.red() == color.blue())
    {
      p.setPen( QPen( color.darker( 140 ), 3 ));
    }
    else
    {
      p.setPen( QPen( color.lighter( 140 ), 3 ));
    }
    
    int start, span;
    
    if (positiveOnly())
    {
      start = 1440-(int)roundf(180.0*32.0*m_minCounts*vFac/M_PI-180.0*16.0*aMax/M_PI);
      span  = -(int)roundf(180.0*32.0*(m_maxCounts-m_minCounts)*vFac/M_PI );
    }
    else
    {
      start = 1440-(int)roundf(180.0*16.0*m_minCounts*vFac/M_PI);
      span  = -(int)roundf(180.0*16.0*(m_maxCounts-m_minCounts)*vFac/M_PI );
    }
    
    p.drawArc( (int)rintf( ox-(r-10) ), (int)rintf( oy-(r-10) ), 
               (int)rintf( 2*(r-10) ), (int)rintf( 2*(r-10) ),
               start, span );
  }
  
  // draw pointer
  if (positiveOnly())
  {
    angle = 2.0*value*vFac - aMax;
  }
  else
  {
    angle = value*vFac;
  }
        
  p.translate( ox, oy );
  p.rotate( angle*180.0/M_PI );
  
  QPointF left[4] = {
    QPointF(  0.0, -r+4.0 ),
    QPointF(  0.0, 0.0 ),
    QPointF( -2.5, 0.0 ),
    QPointF( -2.5, -r+18.0 )
  };
  
  QPointF right[4] = {
    QPointF(  0.0, -r+4.0 ),
    QPointF(  2.5, -r+18.0 ),
    QPointF(  2.5, 0.0 ),
    QPointF( -2.5, 0.0 )
  };
  
  QPointF outline[5] = {
    QPointF(  0.0, -r+4.0 ),
    QPointF(  2.5, -r+18.0 ),
    QPointF(  2.5, 0.0 ),
    QPointF( -2.5, 0.0 ),
    QPointF( -2.5, -r+18.0 )
  };
  
  p.setPen( Qt::NoPen );
  p.setBrush( m_colors.foreground().darker( 120 ) );
  p.drawConvexPolygon( right, 4 );
  p.setBrush( m_colors.foreground().lighter( 120 ) );
  p.drawConvexPolygon( left, 4 );

  p.setPen( QPen( Qt::black, 0.5 ) );
  p.setBrush( Qt::NoBrush );

  p.drawConvexPolygon( outline, 5 );
  p.restore();
  
  if (m_showValue)
  {
    // draw dark box and lcd
    QString str;
    p.setFont( m_font );
    LCDNumber lcd( true, numDigits() );
    int th = lcd.height()+8;
    int bw = lcd.width()+12;
    p.setPen( Qt::black );
    p.setBrush( m_colors.background().darker( 120 ) );
    int bx = (width()-bw-m_uw)/2;
    int by = height()-th;
    p.drawRect( bx, by,bw+m_uw, th );
  
    lcd.draw( &p, (width()-bw-m_uw)/2+4, height()-th+4, m_value );
    
    // show AC, DC or diode
    if (m_mode == "AC" || m_mode == "DC")
    {
      p.drawText( bx + bw-4, by+1, bw, th-2, 
                  Qt::AlignLeft | Qt::AlignTop, m_mode );
    }
    else if (m_mode == "DI")
    {
      p.drawPixmap( bx + bw-4, by+4, *s_diodeSmallPix );
    }
    p.drawText( bx + bw-4, by+1, bw, th-2, 
                Qt::AlignLeft | Qt::AlignBottom, UnitString::toQString( m_unit ) );
  }
  else
  {
    // just some chrome with unit written on it
    p.setPen( QPen( Qt::black, 0.5 ) );
    p.setBrush( m_colors.background().darker( 120 ) );
    p.drawEllipse( (int)rintf( ox-(r+25-pRect.height()-m_titleHeight)), 
                   (int)rintf( oy-(r+25-pRect.height()-m_titleHeight)), 
                   (int)rintf( 2*(r+25-pRect.height()-m_titleHeight)), 
                   (int)rintf( 2*(r+25-pRect.height()-m_titleHeight)) );
    
    p.setPen( Qt::black );
    p.setFont( QFont( "Helvetica", 10, QFont::DemiBold ));
    p.drawText( 0, height()-20, width(), 20, Qt::AlignCenter, 
                UnitString::toQString( m_unit ) );
  }
  
  p.end();
}
