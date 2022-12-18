//======================================================================
// File:        lcdnumber.h
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

#ifndef LCDNUMBER_HH
#define LCDNUMBER_HH

#include <QtGui>

class LCDNumber
{
  public:
	LCDNumber( bool small, int numDigits );
	~LCDNumber();

	int height() const { return m_height + (m_small ? 2 : 4); }
	int width() const;

	void draw( QPainter *, int x, int y, const QString & ) const;

  protected:
	bool m_small;
	int  m_width;
	int  m_height;
	int  m_numDigits;

	static QPixmap *s_numbers;
	static QPixmap *s_numbers_small;

};

#endif // LCDNUMBER_HH
