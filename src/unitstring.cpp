//======================================================================
// File:        unitstring.h
// Author:      Matthias Toussaint
// Created:     Sat Aug 18 19:04:57 CEST 2007
// Project:     QtDMM
// Description: create unicode QString from unit
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
// Copyright 2006-2007 Matthias Toussaint
//======================================================================

#include "unitstring.h"

QString UnitString::toQString( const QString & ascii )
{
  int index = 0;

  QString ret;
  if (ascii[0] == 'k' ||
	  ascii[0] == 'm' ||
	  ascii[0] == 'M' ||
	  ascii[0] == 'G' ||
	  ascii[0] == 'n' ||
	  ascii[0] == 'p')
  {
	ret = ascii[0];
	++index;
  }
  else if (ascii[0] == 'u')
  {
	ret = "µ";
	++index;
  }

  if (ascii.mid( index ) == "Ohm")
	ret += "Ω";
  else if (ascii.mid( index ) == "C")
	ret += "nC";
  else
	  ret += ascii.mid( index );

  return ret;
}
