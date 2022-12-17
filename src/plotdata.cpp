//======================================================================
// File:        plotdata.cpp
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

#include <plotdata.h>
#include <iostream>

PlotData::PlotData( int size ) :
  m_ringBuffer( 0 )
{
  resize( size );
}

PlotData::PlotData( const QString & name, int size ) :
    m_name( name ),
  m_ringBuffer( 0 )
{
  resize( size );
}

PlotData::~PlotData()
{
  delete [] m_ringBuffer;
}

void PlotData::resize( int size )
{
  delete [] m_ringBuffer;
  
  m_size = size;
  m_ringBuffer = new PlotItem [size];
  
  reset();
}

void PlotData::reset()
{
  m_numItems = 0;
  m_timeOffset = m_start = m_write = 0;
  m_min = m_max = 0.0f;
}

PlotItem *PlotData::at( int index ) const
{
  if (index>=m_numItems) return 0;
  
  return &m_ringBuffer[(index+m_start) % m_size];
}

void PlotData::addValue( const QDateTime & dt, float value )
{
  if (0 == m_numItems) m_startTime = dt;
  
  if (value < m_min) m_min = value;
  if (value > m_max) m_max = value;
  
  if (m_numItems && (m_write == m_start))
  {
    if (++m_start >= m_size) m_start = 0;
    m_timeOffset = m_ringBuffer[m_start].time;
  }
  
  m_ringBuffer[m_write].time  = dsecsToStart( dt );
  m_ringBuffer[m_write].value = value;
  
  if (++m_write >= m_size) m_write = 0;
  if (m_numItems < m_size) ++m_numItems;
}

unsigned PlotData::dsecsToStart( const QDateTime & dt ) const
{
  int days  = m_startTime.daysTo( dt );
  int msecs = m_startTime.time().msecsTo( dt.time() );
  
  return days*864000 + (msecs/100);
}

QDateTime PlotData::startDateTime() const
{
  return m_startTime.addMSecs( (qint64)timeOffset()*(qint64)100 );
}
