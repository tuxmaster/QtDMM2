//======================================================================
// File:        plotwid.h
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

#ifndef PLOTWID_HH
#define PLOTWID_HH

#include <QtWidgets>
#include "plotdata.h"

class PlotWid : public QWidget
{
  Q_OBJECT
public:
  PlotWid( QWidget *parent=nullptr );
  virtual ~PlotWid();

  void create( int numItems, int size );
  void resize( int size );
  void reset();
  int numItems() const { return m_data.count(); }

  void setName( int index, const QString & name );
  void setUnit( int index, const QString & unit );
  void addValue( int index, const QDateTime & dt, float value );

  PlotData *data( int index );

protected:
  PlotDataList m_data;

  void paintEvent( QPaintEvent *ev );
  void freeData();
};

#endif // PLOTWID_HH
