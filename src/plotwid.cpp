//======================================================================
// File:        plotwid.cpp
// Author:      Matthias Toussaint
// Created:     Sun Feb 25 10:41:55 CET 2007
// Project:     QtDMM
// Description: Qt based multimeter readout program
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

#include "plotwid.h"
#include "plotscale.h"
#include <cmath>
#include <iostream>

PlotWid::PlotWid( QWidget *parent ) : QWidget( parent )
{
}

PlotWid::~PlotWid()
{
  freeData();
}

void PlotWid::freeData()
{
  for (PlotDataList::iterator it=m_data.begin(); it!=m_data.end(); ++it)
	delete *it;
  m_data.clear();
}

void PlotWid::create( int numItems, int size )
{
  freeData();
  for (int i=0; i<numItems; ++i)
	m_data += new PlotData( size );
}

void PlotWid::resize( int size )
{
  for (PlotDataList::iterator it=m_data.begin(); it!=m_data.end(); ++it)
	(*it)->resize( size );
}

void PlotWid::reset()
{
  for (PlotDataList::iterator it=m_data.begin(); it!=m_data.end(); ++it)
	(*it)->reset();
}

PlotData *PlotWid::data( int index )
{
  return m_data.at( index );
}

void PlotWid::setName( int index, const QString & name )
{
  data( index )->setName( name );
}

void PlotWid::setUnit( int index, const QString & unit )
{
  data( index )->setUnit( unit );
}

void PlotWid::addValue( int index, const QDateTime & dt, float value )
{
  data( index )->addValue( dt, value );
}

void PlotWid::paintEvent( QPaintEvent * )
{
  if (numItems() == 0) return;

  QElapsedTimer t;
  t.start();

  QPainter p;
  p.begin( this );
  p.setFont( QFont( "Helvetica", 9 ) );
  p.setRenderHint( QPainter::TextAntialiasing, true );
  //p.setRenderHint( QPainter::Antialiasing, true );

  int textHeight = p.fontMetrics().height();
  int h = (height()-textHeight*numItems())/numItems();

  PlotScale grid;

  QPoint *arr = new QPoint [data(0)->numItems()];

  for (int i=0; i<numItems(); ++i)
  {
	grid.setup( h, data(i)->minValue(), data(i)->maxValue() );

	float yFac = (float)h / (grid.max()-grid.min());
	float xFac = (float)(width()-80) / (float)data(i)->numItems();

	p.save();
	p.translate( 70, 5+h*i+i*textHeight );
	grid.paint( &p, width()-80, data(i)->name(), data(i)->unit() );

	for (int x=0; x<data(i)->numItems(); ++x)
		arr[x] = QPoint( lrintf( (float)x*xFac ), lrintf( (grid.max()-data(i)->at(x)->value)*yFac ) );

	//p.setRenderHint( QPainter::Antialiasing, true );
	p.setPen( QPen( Qt::blue, 1.5 ) );
	p.drawPolyline( arr, data(i)->numItems() );
	//p.setRenderHint( QPainter::Antialiasing, false );

	p.restore();
  }

  p.end();

  std::cerr << "paint: " << t.elapsed() << " msec" << std::endl;
}
