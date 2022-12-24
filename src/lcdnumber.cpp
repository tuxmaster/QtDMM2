//======================================================================
// File:        lcdnumber.cpp
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

#include "lcdnumber.h"
#include "xpm/numbers.xpm"
#include "xpm/numbers_small.xpm"

QPixmap *LCDNumber::s_numbers = nullptr;
QPixmap *LCDNumber::s_numbers_small = nullptr;

LCDNumber::LCDNumber( bool small, int numDigits ) :
  m_small( small ),
  m_numDigits( numDigits )
{
  if (!s_numbers)
  {
	s_numbers = new QPixmap( (const char **)numbers_xpm );
	s_numbers_small = new QPixmap( (const char **)numbers_small_xpm );
  }

  if (small)
  {
	m_width = 12;
	m_height = 21;
  }
  else
  {
	m_width = 34;
	m_height = 60;
  }
}

LCDNumber::~LCDNumber()
{
}

int LCDNumber::width() const
{
  int gap = m_small ? 6 : 8;
  int minus = m_small ? 8 : 24;

  return m_numDigits * m_width + minus + m_numDigits*gap;
}

void LCDNumber::draw( QPainter *p, int x, int y, const QString & digits ) const
{
  int mw = 24;
  int mh = 8;
  int dotoff = 7;
  int cw = 34;
  int ch = 60;
  int gap = 8;
  int dotw = 6;
  int doth = 8;

  QPixmap *pix = s_numbers;

  if (m_small)
  {
	mw = 8;
	mh = 3;
	dotoff = 5;
	cw = 12;
	ch = 21;
	gap = 6;
	dotw = doth = 4;

	pix = s_numbers_small;
  }

  int xo = 0;

  // check if an offset is needed
  int num = 0, i=0;
  while (i<digits.length())
  {
	if (digits[i] != '.' && digits[i] != '-') ++num;
	++i;
  }

  while (num < m_numDigits) { ++num; xo += cw+gap; }

  if (digits[0] == '-')
  {
	p->fillRect( x+xo, y+(ch-mh)/2, mw, mh, Qt::black );
	++i;
  }

  xo += mw+gap;
  i=0;

  while (i<digits.length())
  {
	int off = (int)digits[i].toLatin1() - (int)'0';

	if (off >= 0 && off <= 9)
	{
	  p->drawPixmap( x+xo, y, *pix, off*cw, 0, cw, ch );
	  xo += cw+gap;
	}
	else if (digits[i] == '.')
	{
	  p->fillRect( x+xo-dotoff, y+ch-doth/2, dotw, doth, Qt::black );
	}
	++i;
  }
}
