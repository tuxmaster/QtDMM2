//======================================================================
// File:        plotdata.h
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

#ifndef PLOTDATA_HH
#define PLOTDATA_HH

#include <QtCore>

/** \brief Implements a single data value for the circular buffer

	The data items are tagged with a 10th second offset to the start time.
 */
struct PlotItem
{
  /** \brief Offset in 10th of a second to the start time of the circular buffer
  */
  unsigned time;  // ten'th of seconds since start
  /** \brief The value itself
  */
  float    value;
};

/** \brief Implements a circular buffer for the multimeter data

	Data has a name, a unit and a start date and time. The data items
	are tagged with a 10th second offset to the start time.
  */
class PlotData
{
public:
  /// ctor
  PlotData( int size=1000 );
  /// ctor
  PlotData( const QString & name, int size=1000 );
  /// dtor
  ~PlotData();

  /** \brief Set the name for the data
  */
  void setName( const QString & name ) { m_name = name; }
  /** \brief Set the unit for the data
  */
  void setUnit( const QString & unit ) { m_unit = unit; }

  /** \brief Retrieve name of data
  */
  QString name() const { return m_name; }
  /** \brief Retrieve unit of data
  */
  QString unit() const { return m_unit; }
  /** \brief Resize the circular buffer and clear it
	  \param size New size for the buffer
  */
  void resize( int size );
  /** \brief Reset (clear) the circular buffer
   */
  void reset();
  /** \brief Insert a value into the circular buffer
	  \param dt Date and time of measurement
	  \param value The value itself
  */
  void addValue( const QDateTime & dt, float value );
  /** \brief Return number of items in buffer
  */
  int numItems() const { return m_numItems; }
  /** \brief Return pointer to item at position in buffer.
	  \param index
	  \returns pointer to item or 0 if out of bounds
   */
  PlotItem *at( int index ) const;
  /** \brief Return the time offset of the first item in 10th of seconds
			 relative to the start time
  */
  unsigned timeOffset() const { return m_timeOffset; }
  /** \brief Return the start time of the data (corrected with the offset)
  */
  QDateTime startDateTime() const;
  float minValue() const { return m_min; }
  float maxValue() const { return m_max; }

protected:
  QString   m_name;
  QString   m_unit;
  QDateTime m_startTime;
  unsigned  m_timeOffset;
  PlotItem *m_ringBuffer;
  float     m_min;
  float     m_max;
  int       m_size;
  int       m_numItems;
  int       m_start;
  int       m_write;

  unsigned dsecsToStart( const QDateTime & dt ) const;

};

typedef QList<PlotData *> PlotDataList;

#endif // PLOTDATA_HH
