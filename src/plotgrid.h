//======================================================================
// File:        plotgrid.h
// Author:      Matthias Toussaint
// Created:     Sun Feb 25 13:26:57 CET 2007
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

#ifndef PLOTGRID_HH
#define PLOTGRID_HH

#include <QtGui>

class PlotGrid
{
public:
  PlotGrid();
  ~PlotGrid();

  void paint( QPainter *, const QRect & rect, float min, float max,
			  unsigned start, unsigned length );

};

#endif // PLOTGRID_HH
