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

#ifndef UNITSTRING_HH
#define UNITSTRING_HH

#include <QString>

class UnitString
{
  public:
    static QString toQString( const QString & ascii );
};

#endif // UNITSTRING_HH
