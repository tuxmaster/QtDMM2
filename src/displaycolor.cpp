//======================================================================
// File:        displaycolor.h
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


#include "displaycolor.h"

DisplayColor::DisplayColor() :
  m_background( Qt::white ),
  m_foreground( Qt::black )
{
}

DisplayColor::~DisplayColor()
{
}

void DisplayColor::setColor( const QColor & color )
{
  int min = std::min( color.red(), std::min( color.green(), color.blue() ));

  int off = 220-min;
  m_background = QColor( std::min( 255, color.red()+off ),
						 std::min( 255, color.green()+off ),
						 std::min( 255, color.blue()+off ) );

  int max = std::max( color.red(), std::max( color.green(), color.blue() ));

  double fac = 150.0 / (double)max;
  m_foreground = QColor( (int)((double)color.red()*fac),
						 (int)((double)color.green()*fac),
						 (int)((double)color.blue()*fac) );
}
