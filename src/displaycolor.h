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

#ifndef DISPLAYCOLOR_HH
#define DISPLAYCOLOR_HH

#include <QColor>

class DisplayColor
{
  public:
    DisplayColor();
    ~DisplayColor();
    
    void setColor( const QColor & );
    QColor background() const { return m_background; }
    QColor foreground() const { return m_foreground; }
    
  protected:
    QColor m_background;
    QColor m_foreground;

};

#endif // DISPLAYCOLOR_HH
