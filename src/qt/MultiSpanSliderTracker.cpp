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

#include <Carna/qt/MultiSpanSliderTracker.h>
#include <Carna/base/math/Span.h>
#include <QPainter>
#include <QFontMetrics>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSliderTracker
// ----------------------------------------------------------------------------------

const int MultiSpanSliderTracker::trackerWidth  = 40;
const int MultiSpanSliderTracker::trackerHeight = 8;


MultiSpanSliderTracker::~MultiSpanSliderTracker()
{
}


void MultiSpanSliderTracker::paint( const base::math::Span< int >& span
                                  , unsigned int span_index
                                  , Qt::Orientation orientation
                                  , QPainter& painter
                                  , Role mode )
{
    int val;

    if( mode == end )
    {
        val = span.last;
    }
    else
    if( mode == begin )
    {
        painter.scale( 1, -1 );

        val = span.first;
    }

 // draw ending tracker

    QPolygon polygon;
    polygon << QPoint( 0, 0 )
            << QPoint( trackerWidth, 0 )
            << QPoint( trackerWidth, trackerHeight )
            << QPoint( trackerWidth * 0.3, trackerHeight );

    painter.setPen( Qt::black );
    painter.setBrush( Qt::black );
    painter.drawPolygon( polygon );

 // draw label

    const QFontMetrics fm = painter.fontMetrics();
    const QString text = QString::number( val );
    const int text_w = fm.width( text );
    const int text_h = fm.height();
    const int text_y = trackerHeight / 2 - text_h / 2;

    if( mode == begin )
    {
        painter.scale( 1, -1 );
        painter.translate( 0, -trackerHeight );
    }

    painter.drawText( QRect( trackerWidth + 10, text_y, text_w, text_h ), 0, text );
}


bool MultiSpanSliderTracker::testHit( const QPoint& p, Qt::Orientation, Role mode )
{
    if( p.x() < 0 || p.x() > trackerWidth )
    {
        return false;
    }

    if( ( mode == end   && p.y() >= 0 && p.y() <=  trackerHeight )
     || ( mode == begin && p.y() <= 0 && p.y() >= -trackerHeight ) )
    {
        return true;
    }

    return false;
}



}  // namespace Carna :: qt

}  // namespace Carna
