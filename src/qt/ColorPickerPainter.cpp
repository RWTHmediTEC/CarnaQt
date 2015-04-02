/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/qt/ColorPickerPainter.h>
#include <QColor>
#include <QPainter>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorPickerPainter
// ----------------------------------------------------------------------------------

void ColorPickerPainter::paint( QPainter& p
                              , const QRect& rect
                              , const QColor& color
                              , bool ignoreAlpha )
{
    if( ignoreAlpha )
    {
        QColor c( color );
        c.setAlpha( 255 );
        p.fillRect( rect, c );
    }
    else
    {
        p.fillRect( rect, Qt::white );
        p.fillRect( rect, QBrush( Qt::black, Qt::Dense6Pattern ) );
        p.fillRect( rect, color );
    }
}



}  // namespace Carna :: qt

}  // namespace Carna
