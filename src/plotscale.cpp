//======================================================================
// File:        plotscale.cpp
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

#include "plotscale.h"
#include <cmath>
#include <iostream>

PlotScale::PlotScale()
{
}

PlotScale::PlotScale( int length, float min, float max )
{
  setup( length, min, max );
}

PlotScale::~PlotScale()
{
}

void PlotScale::setup( int length, float min, float max )
{
  m_length = length;
  m_step = (max-min)/(length/32);
  m_step = fitStep( m_step );

  m_min = fitToStep( min, m_step, true );
  m_max = fitToStep( max, m_step, false );

  //std::cerr << "min=" << m_min << " max=" << m_max << " step=" << m_step
  //    << std::endl;
}

void PlotScale::paint( QPainter *p, int width, const QString & name, const QString & unit )
{
  float fac = (float)m_length / (m_max-m_min);

  p->drawLine( 0, 0, 0, m_length );
  p->drawLine( width, 0, width, m_length );

  int numTicks = lrintf( (m_max-m_min) / m_step ) + 1;

  float pos = m_max;

  //std::cerr << "painting size=" << m_length << " ticks=" << numTicks
  //    << " min=" << m_min << " max=" << m_max << " step=" << m_step << std::endl;

  float textFactor;
  QString unitScale = getFactor( &textFactor );

  p->fillRect( 1, 1, width-1, m_length-1, Qt::white );

  QString str;
  for (int i=0; i<numTicks; ++i)
  {
	int yPos = lrintf( (m_max-pos)*fac );
	p->drawLine( -5, yPos, 0, yPos );
	str=QString("%1").arg( pos*textFactor );
	if (0==i)
	{
	  p->drawText( -100, yPos-5, 90, 20, Qt::AlignVCenter|Qt::AlignRight,
					str );
	  p->drawLine( 1, yPos, width, yPos );
	}
	else if (i==numTicks-1)
	{
	  p->drawText( -100, yPos-15, 90, 20, Qt::AlignVCenter|Qt::AlignRight,
					str );
	  p->drawLine( 1, yPos, width, yPos );
	}
	else
	{
	  p->drawText( -100, yPos-10, 90, 20, Qt::AlignVCenter|Qt::AlignRight,
					str );
	  p->save();
	  p->setPen( Qt::gray );
	  p->drawLine( 1, yPos, width-1, yPos );
	  p->restore();
	}

	pos -= m_step;
  }

  str = name;
  str += " [";
  str += unitScale;
  str += unit;
  str += "]";

  p->save();
  p->rotate( -90.0 );
  p->translate( -m_length, -70 );
  p->drawText( 0, 0, m_length, 30, Qt::AlignCenter, str );
  p->restore();
}

QString PlotScale::getFactor( float *fac ) const
{
  float max = std::max( fabsf( m_min ), fabsf( m_max ) );
  QString unit = "p";

  if (max > 1e-12f) { *fac = 1e12f; unit = "p"; }
  if (max > 1e-9f) { *fac = 1e9f; unit = "n"; }
  if (max > 1e-6f) { *fac = 1e6f; unit = "µ"; }
  if (max > 1e-3f) { *fac = 1e3f; unit = "m"; }
  if (max > 1.0f) { *fac = 1.0f; unit = ""; }
  if (max > 1e3f) { *fac = 1e-3f; unit = "k"; }
  if (max > 1e6f) { *fac = 1e-6f; unit = "M"; }
  if (max > 1e9f) { *fac = 1e-9f; unit = "G"; }
  if (max > 1e12f) { *fac = 1e-9f; unit = "T"; }

  return unit;
}

float PlotScale::fitToStep( float value, float step, bool min ) const
{
  //std::cerr << "going to fit " << value << " with " << step << std::endl;
  float fac = value/step;
  int iFac = (int)( fac );

  //std::cerr << "    fac=" << fac <<  " iFac=" << iFac << std::endl;

  if (min)  // get smaller
  {
	if ((float)iFac > fac)
	{
	  return step*(float)(iFac-1);
	}
  }
  else if ((float)iFac < fac)
  {
	return step*(float)(iFac+1);
  }

  return value;
}

float PlotScale::fitStep( float step )
{
  m_fac = 1.0f;

  // normalize
  do
  {
	if (step > 10.0f)
	{
	  step /= 10.0f;
	  m_fac *= 10.0f;
	}
	else if (step < 1.0f)
	{
	  step *= 10.0f;
	  m_fac /= 10.0f;
	}
  }
  while (step < 1.0f || step > 10.0f);

  // fit into 1-1.5-2-2.5-5-10 grid
  if (step > 1.0f && step <= 1.5f) step = 1.5f;
  else if (step > 1.5f && step <= 2.0f) step = 2.0f;
  else if (step > 2.0f && step <= 2.5f) step = 2.5f;
  else if (step > 2.5f && step <= 5.0f) step = 5.0f;
  else if (step > 5.0f) step = 10.0f;

  return step*m_fac;
}

