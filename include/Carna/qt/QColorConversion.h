/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef QCOLORCONVERSION_H_0874895466
#define QCOLORCONVERSION_H_0874895466

/** \file   QColorConversion.h
  * \brief  Defines `QColor` and `base::Color` conversion routines.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/Color.h>
#include <QColor>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// toQColor
// ----------------------------------------------------------------------------------

inline QColor toQColor( const base::Color& color )
{
    return QColor( color.r, color.g, color.b, color.a );
}



// ----------------------------------------------------------------------------------
// toColor
// ----------------------------------------------------------------------------------

inline base::Color toColor( const QColor& qc )
{
    return base::Color( qc.red(), qc.green(), qc.blue(), qc.alpha() );
}



}  // namespace Carna :: qt

}  // namespace Carna

#endif // QCOLORCONVERSION_H_0874895466
