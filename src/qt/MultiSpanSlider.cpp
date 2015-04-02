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

#include <Carna/qt/MultiSpanSlider.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>
#include <QPainter>
#include <QFontMetrics>
#include <QMouseEvent>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSlider
// ----------------------------------------------------------------------------------

MultiSpanSlider::MultiSpanSlider( Qt::Orientation orientation
                                , MultiSpanSliderTracker* tracker
                                , IntSpanPainter* spanPainter
                                , QWidget* parent )
    : QWidget( parent )
    , orientation( orientation )
    , tracker( tracker )
    , spanPainter( spanPainter )
    , first( 0 )
    , last( 1 )
    , tickInterval( 0 )
    , captionInterval( 0 )
    , grabbedIndex( -1 )
{
}


void MultiSpanSlider::paintEvent( QPaintEvent* )
{
    QPainter painter( this );

 // prepare

    int width, height;
    if( orientation == Qt::Horizontal )
    {
        painter.rotate( 90 );
        painter.translate( 0, this->height() );

        width  = this->height();
        height = this->width();
    }
    else
    {
        width  = this->width();
        height = this->height();
    }

    QFont font = painter.font();
    font.setPixelSize( 10 );
    painter.setFont( font );
    const QFontMetrics fm = painter.fontMetrics();

 // draw track line

    const int spacing = 10;
    const int text_axis_spacing = 10;

    const int max_label_width = std::max( fm.width( QString::number( this->first ) ), fm.width( QString::number( this->last ) ) );
    mvm = MultiSpanSliderModelViewMapping( this->first, this->last, height - spacing * 2, spacing, max_label_width + text_axis_spacing );

    painter.drawLine( mvm.getOffsetLeft(), mvm.valueToPixel( this->first )
                    , mvm.getOffsetLeft(), mvm.valueToPixel( this->last  ) );

    painter.drawLine( mvm.getOffsetLeft() - 4, mvm.valueToPixel( this->first )
                    , mvm.getOffsetLeft() + 4, mvm.valueToPixel( this->first ) );

    painter.drawLine( mvm.getOffsetLeft() - 4, mvm.valueToPixel( this->last )
                    , mvm.getOffsetLeft() + 4, mvm.valueToPixel( this->last ) );

 // draw ticks

    QRect captionRect( 0, -fm.height() / 2, max_label_width, fm.height() );
    if( tickInterval > 0 )
    {
        for( int val = this->first; val < this->last; val += tickInterval )
        {
            const int y = mvm.valueToPixel( val );

            painter.drawLine( mvm.getOffsetLeft() - 2, y, mvm.getOffsetLeft(), y );

            painter.drawText( captionRect.translated( 0, y ), Qt::AlignRight | Qt::AlignVCenter, QString::number( val ) );
        }
    }

 // draw trackers

    painter.translate( mvm.getOffsetLeft(), 0 );

    for( unsigned int i = 0; i < spans.size(); ++i )
    {
        base::math::Span< int >& span = spans[ i ];
        const int y0 = mvm.valueToPixel( span.first );
        const int y1 = mvm.valueToPixel( span.last  );

     // map beginning tracker to be at y=0 -------------------o
                                                           // |
        painter.save();                                    // |
        painter.translate( 0, y0 );                        // |
                                                           // |
     // map ending tracker to be at y=1 -----------o       // |
                                                // |       // |
        painter.scale( 1, y1 - y0 );            // |       // |
                                                // |       // |
        spanPainter->paint( painter, span, i ); // |       // |
                                                // |       // |
     // un-map ------------------------------------o----------o

        painter.restore();
    }
    for( unsigned int i = 0; i < spans.size(); ++i )
    {
        base::math::Span< int >& span = spans[ i ];
        const int y0 = mvm.valueToPixel( span.first );
        const int y1 = mvm.valueToPixel( span.last  );

     // map beginning tracker to be at y=0 -----------------------------o
                                                                     // |
        painter.save();                                              // |
        painter.translate( 0, y0 );                                  // |
                                                                     // |
     // paint beginning tracker                                      // |
                                                                     // |
        painter.save();                                              // |
        tracker->paint( span, i, orientation                         // |
                      , painter, MultiSpanSliderTracker::begin );    // |
        painter.restore();                                           // |
                                                                     // |
     // map ending tracker                                           // |
                                                                     // |
        painter.translate( 0, y1 - y0 );                             // |
        tracker->paint( span, i, orientation                         // |
                      , painter, MultiSpanSliderTracker::end );      // |
                                                                     // |
     // un-map ---------------------------------------------------------o

        painter.restore();
    }
}


void MultiSpanSlider::setFirst( int first )
{
    CARNA_ASSERT_EX( spans.empty() || spans.front().first >= first, "span out of global bounds" );

    this->first = first;
    this->update();
}


void MultiSpanSlider::setLast( int last )
{
    CARNA_ASSERT_EX( spans.empty() || spans.back().last <= last, "span out of global bounds" );

    this->last = last;
    this->update();
}


void MultiSpanSlider::setTickInterval( int tickInterval )
{
    this->tickInterval = tickInterval;
    this->update();
}


void MultiSpanSlider::setCaptionInterval( int captionInterval )
{
    this->captionInterval = captionInterval;
    this->update();
}


void MultiSpanSlider::setSpanPainter( IntSpanPainter* spanPainter )
{
    this->spanPainter.reset( spanPainter );
    this->update();
}


bool MultiSpanSlider::canTakeUpdatedSpan( unsigned int index, const base::math::Span< int >& span ) const
{
    return ( index <= 0 || spans[ index - 1 ].last < span.first )
        && ( index + 1 >= spans.size() || spans[ index + 1 ].first > span.last )
        && ( span.first < span.last )
        && ( span.first >= this->first && span.last <= this->last );
}


void MultiSpanSlider::updateSpan( unsigned int index, const base::math::Span< int >& span )
{
    CARNA_ASSERT( index <= 0 || getSpan( index - 1 ).last < span.first );
    CARNA_ASSERT( index + 1 >= getSpanCount() || getSpan( index + 1 ).first > span.last );
    CARNA_ASSERT( span.first < span.last );
    CARNA_ASSERT( span.first >= getFirst() );
    CARNA_ASSERT( span.last <= getLast() );

    spans[ index ] = span;

    emit spansChanged( *this );
    emit spansChanged();

    update();
}


unsigned int MultiSpanSlider::addSpan( const base::math::Span< int >& span )
{
    base::math::Span< int > adjusted_span = span;
    adjusted_span.first = base::math::clamp< int >( span.first, this->first, this->last );
    adjusted_span.last  = base::math::clamp< int >( span.last , this->first, this->last );

    CARNA_ASSERT_EX
        ( adjusted_span.first < adjusted_span.last
        , "span from "
            << adjusted_span.first
            << " to "
            << adjusted_span.last
            << " is invalid" );

    if( spans.empty() )
    {
        spans.push_back( adjusted_span );
        processNewSpan( spans[ 0 ], 0 );

        emit spansChanged( *this );
        emit spansChanged();

        return 0;
    }

    unsigned int at;
    for( at = 0; at < spans.size(); ++at )
    {
        CARNA_ASSERT_EX( adjusted_span.last != spans[ at ].first, "overlapping span" );

        if( spans[ at ].last > adjusted_span.first )
        {
            break;
        }
    }

    CARNA_ASSERT_EX( at >= spans.size() || spans[ at ].first > adjusted_span.last, "overlapping span" );

    spans.insert( spans.begin() + at, adjusted_span );
    processNewSpan( spans[ at ], at );

    emit spansChanged( *this );
    emit spansChanged();

    this->update();
    return at;
}


void MultiSpanSlider::removeAllSpans()
{
    while( !spans.empty() )
    {
        base::math::Span< int >& span = spans.front();

        processRemovedSpan( span, 0 );

        spans.erase( spans.begin() );
    }
    emit spansChanged();
    this->update();
}


void MultiSpanSlider::mousePressEvent( QMouseEvent* ev )
{
    prevMouseY = ev->y();
    spawnNewSpan = true;

    for( unsigned int i = 0; i < spans.size(); ++i )
    {
        for( unsigned int j = 0; j < 2; ++j )
        {
            int val;
            switch( j )
            {

            case 0:
                grabbedMode = MultiSpanSliderTracker::begin;
                val = spans[ i ].first;
                break;

            case 1:
                grabbedMode = MultiSpanSliderTracker::end;
                val = spans[ i ].last;
                break;

            default:
                CARNA_FAIL( "unimplemented tracker mode" );

            }

            const QPoint p( ev->x() - mvm.getOffsetLeft()
                          , ev->y() - mvm.valueToPixel( val ) );

            if( tracker->testHit( p, orientation, grabbedMode ) )
            {
                grabbedIndex = i;
                grabOffset = val - mvm.pixelToValue( ev->y() );

                processTrackerMousePress( spans[ grabbedIndex ], grabbedIndex, grabbedMode );

                return;
            }
        }
    }
}


void MultiSpanSlider::mouseMoveEvent( QMouseEvent* ev )
{
    if( grabbedIndex == -1 )
    {
        if( !spawnNewSpan )
        {
            return;
        }

        const int dy = prevMouseY - ev->y();

        if( dy == 0 )
        {
            return;
        }

        try
        {
            const int y0 = std::min( prevMouseY, ev->y() );
            const int y1 = std::max( prevMouseY, ev->y() );

            base::math::Span< int > new_span( mvm.pixelToValue( y0 ), mvm.pixelToValue( y1 ) );

            if( new_span.last < new_span.first )
            {
                return;
            }

            grabbedIndex = this->addSpan( new_span );
            grabbedMode = ( dy > 0 ? MultiSpanSliderTracker::begin : MultiSpanSliderTracker::end );
            grabOffset = 0;
            
            processTrackerMousePress( spans[ grabbedIndex ], grabbedIndex, grabbedMode );
        }
        catch( ... )
        {
            return;
        }
    }

    base::math::Span< int >& real_span = spans[ grabbedIndex ];

    const base::math::Span< int > orig_span = real_span;
    base::math::Span< int > new_span = real_span;

    const int val = mvm.pixelToValue( ev->y() ) + grabOffset;
    switch( grabbedMode )
    {

        case MultiSpanSliderTracker::begin:
            new_span.first = val;
            break;

        case MultiSpanSliderTracker::end:
            new_span.last = val;
            break;

        default:
            CARNA_FAIL( "unimplemented tracker mode" );

    }

 // check for collisions...

 // ...against bottom limit

    if( grabbedIndex == 0 && new_span.first < this->first )
    {
        new_span.first = this->first;
    }

 // ...against top limit

    if( grabbedIndex == spans.size() - 1 && new_span.last > this->last )
    {
        new_span.last = this->last;
    }

 // ...against lower span

    if( grabbedIndex > 0 && new_span.first <= spans[ grabbedIndex - 1 ].last )
    {
        new_span.first = spans[ grabbedIndex - 1 ].last + 1;
    }

 // ...against top span

    if( grabbedIndex < signed( spans.size() ) - 1 && new_span.last >= spans[ grabbedIndex + 1 ].first )
    {
        new_span.last = spans[ grabbedIndex + 1 ].first - 1;
    }

 // ...against itself

    if( new_span.last <= new_span.first )
    {
        processRemovedSpan( real_span, grabbedIndex );
        spans.erase( spans.begin() + grabbedIndex );
        grabbedIndex = -1;
        spawnNewSpan = false;
    }
    else
    {
        real_span = new_span;
        processModifiedSpan( real_span, grabbedIndex );
    }

    emit spansChanged( *this );
    emit spansChanged();

    this->update();
}


void MultiSpanSlider::mouseReleaseEvent( QMouseEvent* )
{
    grabbedIndex = -1;
}



}  // namespace Carna :: qt

}  // namespace Carna
