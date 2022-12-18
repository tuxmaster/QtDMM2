//======================================================================
// File:        plotscale.h
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

#ifndef PLOTSCALE_HH
#define PLOTSCALE_HH

#include <QtGui>

class PlotScale
{
public:
  PlotScale();
  PlotScale( int length, float min, float max );
  ~PlotScale();

  void setup( int length, float min, float max );

  void paint( QPainter *p, int width,
			  const QString & name, const QString & unit );

  float min() const { return m_min; }
  float max() const { return m_max; }
  float step() const { return m_step; }
  float factor() const { return m_fac; }

protected:
  int m_length;
  float m_step;
  float m_min;
  float m_max;
  float m_fac;

  float fitStep( float );
  float fitToStep( float value, float step, bool min ) const;
  QString getFactor( float *fac ) const;

};

#endif // PLOTSCALE_HH
